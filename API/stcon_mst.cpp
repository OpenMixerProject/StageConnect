#include <string.h>
#include "a2b_bus.h"
#include "AD2425_inc.h"
#include "stcon.h"



enum {
  dis_idle,
  dis_wait_sync,
  dis_wait_mstr,
  dis_1_node,
  dis_wait_1_node,
  dis_n_node,
  dis_wait_n_node,
  dis_end,
};


Csc_master::Csc_master(A2B_Ssc_node* def, U8 local_id, U8 I2C_address, Ci2c_com* com_int): Ca2b_bus_242x(local_id, I2C_address, com_int)
{
  //memcpy((U8*)&l_node_def, (U8*)def, sizeof(A2B_Ssc_node));
  l_node_def = def;
  fw_buf_=0;
  fw_size_=0;
  chain_pwr_ = 0;
  chain_runnig_= false;
  is_44_1_= false;
  ch_alloc_changed_= false;
  local_id_= local_id;
  data_rdy_on_slv_ = 0;
  dis_status_ = dis_idle;


}


/**
 * \brief modify node definition
 *  use this function to modify any parameter in the node definition
 *
 */
void Csc_master::modify_node(A2B_Ssc_node* def)
{
  l_node_def = def;
}//end method


bool Csc_master::sc_main_cyclic(bool reset)
{
  int status;

  if (reset) {
    chain_runnig_= false;
    dis_status_ = dis_idle;
  }

  event_handler();

  if (chain_runnig_){
    send_link_stat();
    check_nodes_stat();
    return chain_runnig_;
  }

  status = start_discovery_rnd();
  status_g = status;
  A2B_DEBUG_SC("DBG_SC: M%d discover nodes= %d \n",local_id_, status)
  if (status <= 0) return chain_runnig_;

  status = configure_chain();
  A2B_DEBUG_SC("DBG_SC:  M%d configure chain= %d \n",local_id_, status)
  if(status) return chain_runnig_;
//
  //
  status = configure_master();
  A2B_DEBUG_SC("DBG_SC:  M%d configure Master= %d \n",local_id_, status)
  if (status) return chain_runnig_;

  chain_runnig_= true;
  ch_alloc_changed_= true;

  return chain_runnig_;
}

/**
 * \brief start discovery round
 * this function search for nodes connected to the master
 *
 * \return number of nodes found or error code
 */
int Csc_master::start_discovery_rnd(void)
{
  int status;
  int stat_return = 0;
  static U8 cnt = 3;

  switch (dis_status_) {
    case dis_idle:
      set_as_master();
      dis_status_ = dis_wait_sync;
      cnt = 3;
    break;

    case dis_wait_sync:
      if(cnt) 
        cnt--;
      else  
        dis_status_ = dis_wait_mstr;
    break;

    case dis_wait_mstr:
      status = is_master_running();
      if (status == A2B_TIME_OUT_ERROR){
        dis_status_ = dis_idle;
        stat_return =  A2B_MST_INIT_ERROR;    //a2b ic could not be configured as master
      }
      else if (status == A2B_NO_ERROR){
        dis_node_idx_ = 0;
        dis_status_ = dis_1_node;
      }
      
    break;

    case dis_1_node:
      discover_1st_node(true);
      dis_status_ = dis_wait_1_node;
    break;

    case dis_wait_1_node:
      status = discover_1st_node(false);
      if (status == A2B_TIME_OUT_ERROR){
        dis_status_ = dis_idle;
        stat_return = A2B_TIME_OUT_ERROR;    
      }
      else if (status == A2B_NO_ERROR){
        if (get_node_def_int(&sc_nodes_def[0], 0)){
            dis_status_ = dis_idle;
            stat_return = A2B_I2C_ERROR;
        }else{
          sc_nodes_def[0].control = 0 ; 
          dis_node_idx_++;
          dis_status_ = dis_n_node;
        }
      }

    break;

    case dis_n_node:
      discover_more_nodes(dis_node_idx_, true);
      dis_status_ = dis_wait_n_node;
    break;

    case dis_wait_n_node:
      status = discover_more_nodes(dis_node_idx_, false);

      if (status == A2B_TIME_OUT_ERROR){
        dis_status_ = dis_end;

        balance_streams();

        status = calc_respnd_cycles();
        check_chain_power();

        if (status){
          dis_status_ = dis_idle;
          stat_return = A2B_RESPCLY_ERROR;
        }else
          stat_return = dis_node_idx_;    //discovery ended

      } else if (status == A2B_NO_ERROR){
        if (get_node_def_int(&sc_nodes_def[dis_node_idx_], dis_node_idx_)){
            dis_status_ = dis_idle;
            stat_return = A2B_I2C_ERROR;
        }else{
          sc_nodes_def[dis_node_idx_].control = 0 ; 
          dis_node_idx_++;
          dis_status_ = dis_n_node;
        }
      }

    break; 

    case dis_end:
      stat_return = dis_node_idx_;
    break; 
    
    default:
    break;
  }//end case 


  return stat_return;
}//end method


int Csc_master::check_link(void)
{
  U8 control = l_node_def->control;
  int ret = A2B_NODE_NOT_LINKED_ERROR;

  //clear flags
  l_node_def->control &= ~(A2B_CTL_REG_LINK | A2B_CTL_REG_CRC_E | A2B_CTL_REG_SFM_E);

  if (control & A2B_CTL_REG_SFM_E)
    ret = A2B_NODE_NOT_LINKED_ERROR;

  else if (no_nodes_){
    if (control & A2B_CTL_REG_CRC_E)
      ret =  A2B_NODE_LINKED_WITH_ERR;
    else
      ret =  A2B_NODE_LINKED;
  } 


  return ret;

}//end method

int Csc_master::check_link_node(U8 node_id)
{
  U8 control;
  int ret = A2B_NODE_NOT_LINKED_ERROR;

  if (node_id >= no_nodes_)
  return A2B_NODE_NOT_LINKED_ERROR;

  control = sc_nodes_def[node_id].control;

  if (control & A2B_CTL_REG_SFM_E)
    ret = A2B_NODE_NOT_LINKED_ERROR;

  else if (no_nodes_){
    if (control & A2B_CTL_REG_CRC_E)
      ret =  A2B_NODE_LINKED_WITH_ERR;
    else
      ret =  A2B_NODE_LINKED;
  } 


  return ret;

}//end method


/**
 * \brief configure a chain
 *
 *
 */
int Csc_master::configure_chain(void)
{
  int status;
  U8 node_idx;

  //check links
  for (node_idx = 0; node_idx < no_nodes_; node_idx++) {
    status = node_linked(node_idx, false);

    if (status != A2B_NODE_LINKED)
      return status;
  }



  for (node_idx = 0; node_idx < no_nodes_; node_idx++) {

    status = configure_node_downstream(node_idx);
    if (status) return status;

    status = configure_node_upstream(node_idx);
    if (status) return status;


    status = configure_node_i2s(node_idx, sc_nodes_def[node_idx].tdm_config, sc_nodes_def[node_idx].tdm_loop_back);
    if (status) return status;

  }


  return A2B_NO_ERROR;

}//

/**
 * \brief configure master
 *
 *
 */
int Csc_master::configure_master(void)
{
  int status;

  //check links
  status = node_linked(0, false);

  if (status != A2B_NODE_LINKED)
    return status;


  status = configure_master_i2s(l_node_def->tdm_config);
  if (status) return status;

  status = configure_master_slots();
  if (status) return status;

  set_newstruct_master();

  return A2B_NO_ERROR;

}//


int Csc_master::check_chain_power(void)
{
  int link;
  U8 node_id;

  chain_pwr_ = 0;

  for (node_id = 0; node_id < no_nodes_; node_id++) {
    //check links
    link = node_linked(node_id, false);
    if (link != A2B_NODE_LINKED)
      return link;

    //get power
    if (!sc_nodes_def[node_id].node_pwr)
      chain_pwr_ = 0;
    else
      chain_pwr_ += sc_nodes_def[node_id].node_pwr;

    if (chain_pwr_ <= A2B_MAX_SC_PWR){
      sc_nodes_def[node_id].control |= A2B_CTL_REG_EN_PWR;
    } else {
      A2B_DEBUG_SC("DBG_SC: MAX chain power reached!\n")
      break;
    } 


  }

  A2B_DEBUG_SC("DBG_SC: Chain power = %d,%d W\n", chain_pwr_/10, chain_pwr_%10)

  return A2B_NO_ERROR;

}//end method

/**
 * \brief send message
 *  use this function to send a message to node
 *
 *  \param node_id
 *  \param data  pointer to data
 *  \param length number of bytes to be set
 *  \return >0 number of bytes send  or error code
 *
 */
int Csc_master::send_message(U8 node_id, U8* data, U8 length)
{
  U8 temp;

  if (node_id >= no_nodes_)
    return A2B_NODE_ID_INVALID;

  //check if rx buffer in slave node if free
  node_i2c_read(node_id, A2B_DEF_I2C_ADD, get_reg_add(&l_node_def->rx_buf_no_bytes), &temp, 1);

  if (temp)
    return 0;       //buffer is busy

  //check length
  if (length >= sc_nodes_def[node_id].buffers_size)
    length = sc_nodes_def[node_id].buffers_size;

  node_i2c_write(node_id, A2B_DEF_I2C_ADD, get_reg_add(&l_node_def->rx_buf_p), data, length);

  //inform node of data
  node_i2c_write(node_id, A2B_DEF_I2C_ADD, get_reg_add(&l_node_def->rx_buf_no_bytes), &length, 1);

  return length;


}//end method

/**
 * \brief get message
 *  use this function to get a message from node
 *
  * \param node_id
 *  \param target  pointer to data
 *  \return >=0 number of byte received. or error code
 *
 */
int Csc_master::get_message(U8 node_id, U8* target, U8 max_len)
{
  U8 no_bytes = 0;

  if (node_id >= no_nodes_)
    return A2B_NODE_ID_INVALID;

  if ( !(data_rdy_on_slv_ & (1 << node_id)) )
    return 0;

  data_rdy_on_slv_ &= ~(1 << node_id);

  //check if rx buffer in slave node if free
  node_i2c_read(node_id, A2B_DEF_I2C_ADD, get_reg_add(&l_node_def->tx_buf_no_bytes), &no_bytes, 1);

  if (!no_bytes)
    return 0;       //buffer is empty

  if (no_bytes >= max_len)
    no_bytes = max_len;

  //get data
  node_i2c_read(node_id, A2B_DEF_I2C_ADD, get_reg_add(&l_node_def->tx_buf_p), target, no_bytes);

  //inform node of data reading
  //temp = 0 ;
  //node_i2c_write(node_id, A2B_DEF_I2C_ADD, get_reg_add(&l_node_def->tx_buf_no_bytes), &temp, 1);


  return no_bytes;


}//end method


int Csc_master::send_link_stat(void)
{
  //NOTE: flag need to be set regularly by master
  int link;
  U8 node_id;
  U8 power = 0;
  U8 samp_rate = 0;

  for (node_id = 0; node_id < no_nodes_; node_id++){
    //check links
    link = node_linked(node_id, false);
    if (link != A2B_NODE_LINKED)
      return link;


    node_i2c_write(node_id, A2B_DEF_I2C_ADD, get_reg_add(&l_node_def->control), &sc_nodes_def[node_id].control  , 1);

    power = (sc_nodes_def[node_id].control & A2B_CTL_REG_EN_PWR) ? A2B_CTL_REG_EN_PWR : 0;
    samp_rate = is_44_1_ ? A2B_CTL_REG_44_1 : 0 ;

    if (sc_nodes_def[node_id].control & A2B_CTL_REG_LINK)
      sc_nodes_def[node_id].control = 0  | power | samp_rate;  
    else
      sc_nodes_def[node_id].control = A2B_CTL_REG_LINK | power | samp_rate; 

  }

  return A2B_NO_ERROR;

}//end method


int Csc_master::check_nodes_stat(void)
{
  
  int link;
  U8 node_id;
  U8 status_l;

  for (node_id = 0; node_id < no_nodes_; node_id++){
    //check links
    link = node_linked(node_id, false);
    if (link != A2B_NODE_LINKED)
      return link;

    node_i2c_read(node_id, A2B_DEF_I2C_ADD, get_reg_add(&l_node_def->status), &status_l  , 1);

    if (status_l != sc_nodes_def[node_id].status){
      sc_nodes_def[node_id].status = status_l;

      //status is handle here locally
      //restart discovery round
      if (sc_nodes_def[node_id].status & A2B_STAT_REG_RDIS){
        chain_runnig_= false;       //by clearing this flag a new discorvery start is forced
        dis_status_ = dis_idle;
      }

      // check for data  
      if (sc_nodes_def[node_id].status & A2B_STAT_REG_BUF_F)
        data_rdy_on_slv_ |= (1 << node_id);    


      //clear flags  TODO think about deleteing on slave after it is read
      //sc_nodes_def[node_id].status &= ~(A2B_STAT_REG_RDIS | A2B_STAT_REG_BUF_F);
     // node_i2c_write(node_id, A2B_DEF_I2C_ADD, get_reg_add(&l_node_def->status), &sc_nodes_def[node_id].status  , 1);
    }


  }

  return A2B_NO_ERROR;

}//end method


int Csc_master::change_node_audio_def(U8 node_id, U8 downslots4node, U8 upslots_local)
{
  //NOTE: flag need to be set regularly by master
  int status;

  //check links
  status = node_linked(node_id, false);
  if (status != A2B_NODE_LINKED)
    return status;

  sc_nodes_def[node_id].downslots4node = downslots4node;
  sc_nodes_def[node_id].upslots_local = upslots_local;

  node_i2c_write(node_id, A2B_DEF_I2C_ADD, get_reg_add(&l_node_def->downslots4node), &sc_nodes_def[node_id].downslots4node  , 1);
  node_i2c_write(node_id, A2B_DEF_I2C_ADD, get_reg_add(&l_node_def->upslots_local), &sc_nodes_def[node_id].upslots_local  , 1);

  sc_nodes_def[node_id].control |= A2B_CTL_REG_DEF_U;

  return A2B_NO_ERROR;
}//end method


bool Csc_master::get_ch_alloc_flag(void)
{
  bool temp = ch_alloc_changed_;
  ch_alloc_changed_ = false;
  return temp;
}

U8 Csc_master::get_ch_alloc(U32 *lut)
{
  U8 ch_cnt = 0;
  U8 node_idx;
  U8 ch_idx;
  U32 *lut_p = lut;
  //A2B_DEBUG_SC("DBG_SC:  call upstream_slots_=%d \n", upstream_slots_);

  for (node_idx = 0; node_idx < no_nodes_; node_idx++){
    for (ch_idx = 0; ch_idx < nodes[node_idx].upslots_local; ch_idx++){
      *lut_p++ = upstream_slots_ - nodes[node_idx].upslots_local + ch_idx - ch_cnt;

      //A2B_DEBUG_SC("DBG_SC:  node_idx = %d  ch_idx = %d  alloc = %d\n",node_idx, ch_idx, upstream_slots_ - nodes[node_idx].upslots_local + ch_idx - ch_cnt);
    }
    ch_cnt += nodes[node_idx].upslots_local;
  }

  return upstream_slots_;

}//end method



void Csc_master::event_handler()
{ //source
  //0 = no interrupt
  //1 => master
  //>=16 => slave id plus 16!!
  U8 source;
  U8 source_node;
  U8 int_type;

  if (!chain_runnig_)
    return;

  source = irq_event(&int_type);

  if (!source)        //no event
    return;

  source_node = source & 0x0f;

  if (source_node >= A2B_MAX_NO_NODES)        
    return;

  switch (int_type) {
  case AD2425_INTTYPE_HDCNTERR:               // 0
    break;

  case AD2425_INTTYPE_DDERR:                // 1  //data decoded error parity error in slot
    if (source == 1)               //master
      l_node_def->control |= A2B_CTL_REG_CRC_E ;
    else
      sc_nodes_def[source_node].control |= A2B_CTL_REG_CRC_E ;

    break;

  case AD2425_INTTYPE_CRCERR:                 //  2 // crc error in th SFM
    if (source == 1)               //master
      l_node_def->control |= A2B_CTL_REG_CRC_E ;
    else
      sc_nodes_def[source_node].control |= A2B_CTL_REG_CRC_E ;
    break;

  case AD2425_INTTYPE_DPERR:                //  3
    if (source == 1)               //master
      l_node_def->control |= A2B_CTL_REG_CRC_E ;
    else
      sc_nodes_def[source_node].control |= A2B_CTL_REG_CRC_E ;
    break;

  case AD2425_INTTYPE_BECOVF:                 //  4
    break;

  case AD2425_INTTYPE_SRFERR:                 //  5 //super frame error. connection may be lost
    if (source == 1)               //master
      l_node_def->control |= A2B_CTL_REG_SFM_E ;
    else
      sc_nodes_def[source_node].control |= A2B_CTL_REG_SFM_E ;  

    chain_runnig_= false;
    dis_status_ = dis_idle;
    break;

  case AD2425_INTTYPE_SRFCRCERR:              //  6 // Slave Only
    break;
  case AD2425_INTTYPE_PWRERR0:                //  9 // Cable Terminal Shorted to Ground
    break;
  case AD2425_INTTYPE_PWRERR1:                //  10  // Cable Terminal Shorted to VBat
    break;
  case AD2425_INTTYPE_PWRERR2:                //  11  // Cable Terminals Shorted Together
    break;
  case AD2425_INTTYPE_PWRERR3:                //  12  // Cable Disconnected or Open Circuit
    break;
  case AD2425_INTTYPE_PWRERR4:                //  13  // Cable is Reverse Connected
    break;
  case AD2425_INTTYPE_PWRERR5:                //  15  // Indeterminate Fault
    //chain_runnig_= false;
    break;
  case AD2425_INTTYPE_IO0PND:                 //  16  // Slave Only
    break;
  case AD2425_INTTYPE_IO1PND:                 //  17  //
    break;
  case AD2425_INTTYPE_IO2PND:                 //  18  //
    break;
  case AD2425_INTTYPE_IO3PND:                 //  19  //
    break;
  case AD2425_INTTYPE_IO4PND:                 //  20  //
    break;
  case AD2425_INTTYPE_IO5PND:                 //  21  //
    break;
  case AD2425_INTTYPE_IO6PND:                 //  22  //
    break;
  case AD2425_INTTYPE_IO7PND:                 //  23  //
    break;
  case AD2425_INTTYPE_DSCDONE:                //  24  // Master Only
    break;
  case AD2425_INTTYPE_I2CERR:                 //  25  // Master Only
    reset_comint();
    chain_runnig_= false;
    dis_status_ = dis_idle;
    break;
  case AD2425_INTTYPE_ICRCERR:                //  26  // Master Only
    break;
  case AD2425_INTTYPE_PWRERR6:                //  41  // Non-Localized Short to Ground
    break;
  case AD2425_INTTYPE_PWRERR7:                //  42  // on-Localized Short to VBat
    break;
  case AD2425_INTTYPE_MB0_FULL:               //  48  //
    break;
  case AD2425_INTTYPE_MB0_EMPTY:              //  49  //
    break;
  case AD2425_INTTYPE_MB1_FULL:               //  50  //
    break;
  case AD2425_INTTYPE_MB1_EMPTY:              //  51  //
    break;
  case AD2425_INTTYPE_Interrupt_Messaging_error:                 //  0x80
    break;
  case AD2425_INTTYPE_Slave_INTTYPE_Read_Error:                 //  253 // Master Only
    break;
  case AD2425_INTTYPE_Standby_Done:                 //  254 // Master Only
    break;
  case AD2425_INTTYPE_MSTR_RUNNING:                 //  255 // Master Only
    break;

  default:
    break;
  }

}//end function

void Csc_master::conf_fw_update(U8* fw_p, U32 fw_len)
{
  fw_buf_ = fw_p;
  fw_size_ = fw_len;
}

void Csc_master::myxtoa_byte(U8 *str, U8 var)
{
  int pos = 0;
  //U8 byte = var;

  for (pos = 1; pos >= 0; pos--, var = (var >> 4)) {
    if ((var & 0xf) <= 0x9) str[pos] = '0' + (var & 0xf);
    else                  str[pos] = 'A' + (var & 0xf) - 10;
  }

}

void Csc_master::update_allnodes(void)
{
  U8 node_id;
  
  for(node_id = 0; node_id < no_nodes_; node_id++)
    update_node(node_id);
}



int Csc_master::update_node(U8 node_id)
{
  static U8* l_fw = fw_buf_;
  U32 byte_cnt = 0;
  U8 frame_size = 64;
  int status;
  U8 cmd;
  U8 temp;
  U8 key = '~';
  U32 length = fw_size_;

  l_fw = fw_buf_;

  if (!fw_buf_ || !fw_size_)
    return A2B_NO_UPDATE_BUF_P;

  //check links
  status = node_linked(0, false);
  if (status != A2B_NODE_LINKED)
    return status;

  if (node_id >= no_nodes_)
    return A2B_NODE_ID_INVALID;

  A2B_WAIT_FUNCTION(10); //wait for a2b link to stablish

  //reset module
  node_i2c_write(node_id, A2B_DEF_I2C_ADD, get_reg_add(&l_node_def->rst_key), &key  , 1);

  sc_nodes_def[node_id].control |= A2B_CTL_REG_RST_M;
  node_i2c_write(node_id, A2B_DEF_I2C_ADD, get_reg_add(&l_node_def->control), &sc_nodes_def[node_id].control  , 1);
  sc_nodes_def[node_id].control = 0;


  //wait for module to boot
  A2B_WAIT_FUNCTION(400);    //bootloader wait 1 sec for start command


  //start command
  cmd = 'S';
  temp = 'R';  //raw data not encryption
  node_i2c_write(node_id, A2B_DEF_I2C_ADD, cmd, &temp, 1);

  cmd = 'F';
  while (1 ) {

    if (frame_size >= length)
      frame_size = length;


    node_i2c_write(node_id, A2B_DEF_I2C_ADD, cmd, l_fw, frame_size);
    l_fw += frame_size;

    //wait every 2kB
    byte_cnt += frame_size;
    if (byte_cnt >= 2048) {
      byte_cnt = 0;
      A2B_WAIT_FUNCTION(200);      //enough to decrypt and flash
    }


    length -= frame_size;

    if (!length)
      break;
  }

  //stop
  cmd = 'P';
  temp = 0;
  node_i2c_write(node_id, A2B_DEF_I2C_ADD, cmd, &temp, 1);

  return A2B_NO_ERROR;

}


///////////////////////////////////////////////////////////////////////////////////
//  private
U8 Csc_master::get_reg_add(U8* offset)
{
  return (U8)(offset - &l_node_def->api_ver);
}//end method


int Csc_master::get_node_def_int(A2B_Ssc_node* def, U8 node_id)
{

#ifdef A2B_UPDATE_NODE
  //if (node_id == 0)
    update_node(node_id);

  A2B_WAIT_FUNCTION(1000);
#endif

  return node_i2c_read(node_id, A2B_DEF_I2C_ADD, 0, (U8*)def, sizeof(A2B_Ssc_node) - 4); //the buffer pointers and status should not be read

}//end method


void Csc_master::balance_streams(void)
{
  //NOTE: current version does not support local audio slot to go downstream

  U8 bc_total_down=0;
  U8 node_idx;
  U8 total_down = 0;
  U32 mask = 0;
  U8* mask_p = (U8*)&mask;
  S8 total_up = 0;
  U8 up_max = l_node_def->upslots_local;

  downstream_slots_ = 0;
  upstream_slots_ = 0;

  //calculate the number of down and up stream required
  for (node_idx = 0; node_idx < no_nodes_; node_idx++){

    if (sc_nodes_def[node_idx].bcst_mode ){
      if (sc_nodes_def[node_idx].downslots4node > bc_total_down)
        bc_total_down = sc_nodes_def[node_idx].downslots4node;
    }
    else
      total_down += sc_nodes_def[node_idx].downslots4node;
  

    if (bc_total_down > total_down)
      total_down = bc_total_down;

    if (total_down > l_node_def->downslots_local )
      total_down = l_node_def->downslots_local;       //max. master downstream audio slots

    if (( upstream_slots_ + total_down) > 32)
      total_down = 32 - upstream_slots_;     //base class variable

    // check the total number of allocated channels
    downstream_slots_ = total_down;     //base class variable


    // the downstream audio data for node
    //NOTE: current version does not support local audio slots to go downstream
    nodes[node_idx].downslots_local = 0;                    //base class variable
  

    /***********************************************************************************/
    if ((downstream_slots_ + upstream_slots_ + up_max) > 32)
      up_max = 32 - (downstream_slots_ + upstream_slots_);     //base class variable



    // the master upstream value need to be calculated according to the total upstream
    // slots coming from all node
    // calculate the total number of upstream slots required
    if ((sc_nodes_def[node_idx].upslots_local + total_up) > up_max){
      total_up += up_max;
    }
    else
      total_up += sc_nodes_def[node_idx].upslots_local;
      
    upstream_slots_ = total_up;             //base class variable

    //A2B_DEBUG_SC("DBG_SC:  call upstream_slots_=%d \n", upstream_slots_);
    //A2B_DEBUG_SC("DBG_SC:  call downstream_slots_=%d \n", downstream_slots_);
    //A2B_DEBUG_SC("DBG_SC:  call up_max=%d \n", up_max);
    //A2B_DEBUG_SC("DBG_SC:  call total_up=%d \n", total_up);

  }//end main for


  // configure down mask for each node according to chain position
  total_down = 0;
  for (node_idx = 0; node_idx < no_nodes_; node_idx++) {

    if (sc_nodes_def[node_idx].bcst_mode){
      mask = calc_mask((sc_nodes_def[node_idx].downslots4node <= downstream_slots_) ? sc_nodes_def[node_idx].downslots4node : downstream_slots_);
    }else{
      if ((sc_nodes_def[node_idx].downslots4node +  total_down) > downstream_slots_) //l_node_def->downslots_local)
        total_down = 0;       //if more slots are needed start from 0

      mask = calc_mask(sc_nodes_def[node_idx].downslots4node) << total_down;

      total_down += sc_nodes_def[node_idx].downslots4node;
      if (total_down > downstream_slots_) //l_node_def->downslots_local)
        total_down = 0;       //if more slots are needed start from 0
    }

    memcpy(nodes[node_idx].downslots_msk, mask_p, 4);       //base class variable

  }

  // the downstream audio slots is the same as the master downstream slots
  for (node_idx = 0; node_idx < last_node_; node_idx++) {
    nodes[node_idx].downslotsbypass = downstream_slots_;    //base class variable
  }
  nodes[last_node_].downslotsbypass = 0;              //base class variable



  // the upstream audio slots it depends on the node position, and the first node need to provide with all the expected
  // upstream audio slots
  for (node_idx = 0; node_idx <= last_node_; node_idx++) {    

      if (total_up >= sc_nodes_def[node_idx].upslots_local)
        nodes[node_idx].upslots_local = sc_nodes_def[node_idx].upslots_local;     //base class variable
      else
        nodes[node_idx].upslots_local = total_up;     

      total_up -= sc_nodes_def[node_idx].upslots_local;     //base class variable
      if (total_up < 0)
        total_up = 0;

      nodes[node_idx].upslotsbypass = total_up;                   //base class variable
  }

  // the upstream audio data for node
  //NOTE: current version does not use of audio slots from upstream
  for (node_idx = 0; node_idx < no_nodes_; node_idx++) {
    mask = 0;
    memcpy(nodes[node_idx].upslots_msk, mask_p, 4);       //base class variable
  }

}//end method


void Csc_master::balance_downstream(void)
{
  //NOTE: current version does not support local audio slot to go downstream

  U8 bc_total_down=0;
  U8 node_idx;
  U8 total_down = 0;
  U32 mask = 0;
  U8* mask_p = (U8*)&mask;

  // the master downstream value need to be calculated according to the total downstream
  // slots coming from all node
  // calculate the total number of downstream slots required
  for (node_idx = 0; node_idx < no_nodes_; node_idx++){
    if (sc_nodes_def[node_idx].bcst_mode ){
      if (sc_nodes_def[node_idx].downslots4node > bc_total_down)
        bc_total_down = sc_nodes_def[node_idx].downslots4node;
    }
    else
      total_down += sc_nodes_def[node_idx].downslots4node;
  }

  if (bc_total_down > total_down)
    total_down = bc_total_down;


  if (total_down > l_node_def->downslots_local )
    total_down = l_node_def->downslots_local;       //max. master downstream audio slots

  // check the total number of allocated channels
  downstream_slots_ = total_down;     //base class variable


  // configure down mask for each node according to chain position
  total_down = 0;
  for (node_idx = 0; node_idx < no_nodes_; node_idx++) {

    if (sc_nodes_def[node_idx].bcst_mode){
      mask = calc_mask((sc_nodes_def[node_idx].downslots4node <= downstream_slots_) ? sc_nodes_def[node_idx].downslots4node : downstream_slots_);
    }else{
      if ((sc_nodes_def[node_idx].downslots4node +  total_down) > downstream_slots_) //l_node_def->downslots_local)
        total_down = 0;       //if more slots are needed start from 0

      mask = calc_mask(sc_nodes_def[node_idx].downslots4node) << total_down;

      total_down += sc_nodes_def[node_idx].downslots4node;
      if (total_down > downstream_slots_) //l_node_def->downslots_local)
        total_down = 0;       //if more slots are needed start from 0
    }

    memcpy(nodes[node_idx].downslots_msk, mask_p, 4);       //base class variable

  }

  // the downstream audio slots is the same as the master downstream slots
  for (node_idx = 0; node_idx < last_node_; node_idx++) {
    nodes[node_idx].downslotsbypass = downstream_slots_;    //base class variable
  }
  nodes[last_node_].downslotsbypass = 0;              //base class variable


  // the downstream audio data for node
  //NOTE: current version does not support local audio slots to go downstream
  for (node_idx = 0; node_idx < no_nodes_; node_idx++) {
    nodes[node_idx].downslots_local = 0;                    //base class variable
  }

}//end method

void Csc_master::balance_upstream(void)
{
  S8 node_idx;
  S8 total_up = 0;
  U32 mask = 0;
  U8* mask_p = (U8*)&mask;
  U8 up_max = l_node_def->upslots_local;

  if ((downstream_slots_ + up_max) > 32)
    up_max = 32 - downstream_slots_;     //base class variable


  // the master upstream value need to be calculated according to the total upstream
  // slots coming from all node
  // calculate the total number of upstream slots required
  for (node_idx = 0; node_idx < no_nodes_; node_idx++){
    if ((sc_nodes_def[node_idx].upslots_local + total_up) >= up_max){
      total_up = up_max;
      break;
    }
    else
      total_up += sc_nodes_def[node_idx].upslots_local;
      
  }
   upstream_slots_ = total_up;             //base class variable

  // the upstream audio slots it depends on the node position, and the first node need to provide with all the expected
  // upstream audio slots
  for (node_idx = 0; node_idx <= last_node_; node_idx++) {    

      if (total_up >= sc_nodes_def[node_idx].upslots_local)
        nodes[node_idx].upslots_local = sc_nodes_def[node_idx].upslots_local;     //base class variable
      else
        nodes[node_idx].upslots_local = total_up;     

      total_up -= sc_nodes_def[node_idx].upslots_local;     //base class variable
      if (total_up < 0)
        total_up = 0;

      nodes[node_idx].upslotsbypass = total_up;                   //base class variable
  }

  // the upstream audio data for node
  //NOTE: current version does not use of audio slots from upstream
  for (node_idx = 0; node_idx < no_nodes_; node_idx++) {
    mask = 0;
    memcpy(nodes[node_idx].upslots_msk, mask_p, 4);       //base class variable
  }


}//end method

void Csc_master::clear_streams(void)
{
  //NOTE: current version does not support local audio slot to go downstream
  U8 node_idx;
  U32 mask = 0;
  U8* mask_p = (U8*)&mask;
  int status;

    //clear downstream
    downstream_slots_ = 0;                                          //base class variable

    //clear upstream
    upstream_slots_ = 0;                                  //base class variable

  for (node_idx = 0; node_idx < no_nodes_; node_idx++) {
    memcpy(nodes[node_idx].downslots_msk, mask_p, 4);       //base class variable
        nodes[node_idx].downslotsbypass = 0;                        //base class variable
        nodes[node_idx].downslots_local = 0;                      //base class variable
                
        nodes[node_idx].upslotsbypass = 0;                        //base class variable
        nodes[node_idx].upslots_local = 0;                //base class variable

    memcpy(nodes[node_idx].upslots_msk, mask_p, 4);           //base class variable
  }

  status = configure_chain();
  A2B_DEBUG_SC("DBG_SC:  M%d configure chain= %d \n",local_id_, status)

  
  status = configure_master();
  A2B_DEBUG_SC("DBG_SC:  M%d configure Master= %d \n",local_id_, status)

}//end method

U32 Csc_master::calc_mask(U8 no_slots)
{
  U32 mask = 0;

  if (no_slots > 32)
    return mask;

  for (int i = 0; i < no_slots; i++)
    mask |= (1 << i);


  return mask;

}//end function


int Csc_master::get_num_active_nodes()
{
  return no_nodes_;
}


bool Csc_master::get_node_def(A2B_Ssc_node* def, U8 node_id)
{
  if (node_id < 0 || node_id >= no_nodes_) return false;
  *def = sc_nodes_def[node_id]; // this is a structure copy
  return true;
}
