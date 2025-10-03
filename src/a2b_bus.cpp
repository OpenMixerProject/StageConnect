/*
  StageConnect Library
  https://github.com/OpenMixerProject/StageConnect

  Licensed under GPL license
  
  StageConnect Library: Main-File for general A2B communication
*/

#include <string.h>
#include "a2b_bus.h"
#include "AD2425_inc.h"


Ca2b_bus_242x::Ca2b_bus_242x(uint8_t a2b_id, uint8_t I2C_address, Ci2c_com* com_int)
{
  //NOTE: I2C interface expects address right justified.
  I2C_master = I2C_address >> 1;
  I2C_node = (I2C_address | 0x02) >> 1;
  com_int_ = com_int;

  new_structure = false;
  no_nodes_ = 0;
  chain_id_ = a2b_id;
  respnd_cycles_ = A2B_RESPND_CYCLES_INIT;
  control_reg = 0;
  master_a2b_ = 0;

}//end function


void Ca2b_bus_242x::reset_bus(void)
{
  no_nodes_ = 0;

  control_reg = master_a2b_ ? AD242X_CONTROL_MSTR : 0;

  control_reg |= AD242X_CONTROL_SOFTRST;
  com_int_->i2c_write(I2C_master, AD242X_CONTROL_REG, control_reg);
  control_reg &= ~AD242X_CONTROL_SOFTRST;

  com_int_->i2c_write(I2C_master, AD242X_CONTROL_REG, control_reg);


  com_int_->i2c_write(I2C_master, AD242X_SWCTL_REG,  0);  

  for(int node_id=0; node_id < A2B_MAX_NO_NODES; node_id++)
    memset((uint8_t*)&nodes[node_id],0,sizeof(Snode));
}

int Ca2b_bus_242x::is_master_running(void)
{
  //PLL syncs to FS signal
  //wait for PLL to be locked (7.5ms), interrupt type = 0xFF (Master Running)
  
  irq_event(&temp_byte);

  if((temp_byte == AD2425_INTTYPE_MSTR_RUNNING)){
    A2B_DEBUG_A2B("A2B_DBG: ID%d  master running\n", chain_id_)

    //enable interrupt, copy from eval sniff
    com_int_->i2c_write(I2C_master, AD242X_INTMSK0_REG, 0xff);
    //com_int_->i2c_write(I2C_master, AD242X_INTMSK1_REG, 0xff);
    com_int_->i2c_write(I2C_master, AD242X_INTMSK2_REG, 0x0f);

    //???
    com_int_->i2c_write(I2C_master, AD242X_INTPND2_REG, AD242X_INTPND2_DSCDONE);

    //write number of response cycles
    com_int_->i2c_write(I2C_master, AD242X_RESPCYCS_REG, respnd_cycles_);


    //set new structure bit in control register
    set_newstruct_master();


    com_int_->i2c_write(I2C_master, AD242X_I2SGCFG_REG, AD242X_I2SGCFG_EARLY | AD242X_I2SGCFG_TDM8);

    //write switch enable switch
    com_int_->i2c_write(I2C_master, AD242X_SWCTL_REG,  (A2B_SW_MODE_DEF << AD242X_SWCTL_MODE_POS) | AD242X_SWCTL_ENSW );

    time_out = 2;
    
    return A2B_NO_ERROR;
  }

  time_out--;

  if (!time_out)
    return A2B_TIME_OUT_ERROR;      //NODE not found!
  
  return A2B_BUS_BUSY;

}

void Ca2b_bus_242x::set_as_master(void)
{
  reset_bus();

  //set master bit on control register
  control_reg = AD242X_CONTROL_MSTR ;
  com_int_->i2c_write(I2C_master, AD242X_CONTROL_REG, control_reg);

  time_out = 2;
}



uint8_t Ca2b_bus_242x::irq_event(uint8_t* event_id)
{
  //0 = no interrupt
  //1 => master
  //>=16 => slave id plus 16!!
  uint8_t source = 0;

  //read IRQ bit
  com_int_->i2c_read(I2C_master, AD242X_INTSTAT_REG , &temp_byte);
  if (!temp_byte) {
    *event_id = 0;
    return source;
  }
  //find out int source
  com_int_->i2c_read(I2C_master, AD242X_INTSRC_REG , &temp_byte);
  if (temp_byte & AD242X_INTSRC_MSTINT)
    source = 1;
  else if (temp_byte & AD242X_INTSRC_SLVINT) {
    source = 0x10 | (temp_byte & AD242X_INTSRC_INODE_MSK) ;
  }


  com_int_->i2c_read(I2C_master, AD242X_INTTYPE_REG , &temp_byte);

  *event_id = temp_byte;
  A2B_DEBUG_A2B("A2B_DBG: ID%d source=%x IRQ = %x \n", chain_id_, source, temp_byte)

  return source;
}

int Ca2b_bus_242x::discover_1st_node(bool start)
{
  uint8_t node_id = 0;

  if(start){
    //NOTE: no switch functionality on this function, no phantom power
    nodes[node_id].linked = false;

    //write the max number of discovery cycles
    com_int_->i2c_write(I2C_master, AD242X_DISCVRY_REG, respnd_cycles_);

    return A2B_BUS_BUSY;
  }

  irq_event(&temp_byte);

  if((temp_byte == AD2425_INTTYPE_DSCDONE)){
    //end discovery mode
    control_reg |= AD242X_CONTROL_ENDDSC;
    com_int_->i2c_write(I2C_master, AD242X_CONTROL_REG, control_reg);
    control_reg &= ~AD242X_CONTROL_ENDDSC;

    //set node address to 0
    com_int_->i2c_write(I2C_master, AD242X_NODEADR_REG, node_id & AD242X_NODEADR_NONE_MSK); // | AD242X_NODEADR_PERI


    //get node info
    com_int_->i2c_read(I2C_node, AD242X_VENDOR_REG, &nodes[node_id].vendor);
    com_int_->i2c_read(I2C_node, AD242X_PRODUCT_REG, &nodes[node_id].product);
    com_int_->i2c_read(I2C_node, AD242X_VERSION_REG, &nodes[node_id].version);
    com_int_->i2c_read(I2C_node, AD242X_CAPABILITY_REG, &temp_byte) ;
    nodes[node_id].i2c_cap = temp_byte ? true : false;

    //I2C configuration. the node need to be configured according to the audio sample rate.
    //it is used locally to create the I2C clock
    com_int_->i2c_write(I2C_node, AD242X_I2CCFG_REG, 0);


    //write switch enable switch
    com_int_->i2c_write(I2C_node, AD242X_SWCTL_REG,  (A2B_SW_MODE_DEF << AD242X_SWCTL_MODE_POS) | AD242X_SWCTL_ENSW);


    nodes[node_id].linked = true;

    no_nodes_++;
    last_node_ = no_nodes_ - 1;

    time_out = 2;

    return A2B_NO_ERROR;
  }

  time_out--;

  if (!time_out) {
   //end discovery mode
    control_reg |= AD242X_CONTROL_ENDDSC;
    com_int_->i2c_write(I2C_master, AD242X_CONTROL_REG, control_reg);
    control_reg &= ~AD242X_CONTROL_ENDDSC;

    A2B_DEBUG_A2B("A2B_DBG: ID%d search first node time_out ! \n", chain_id_)
    return A2B_TIME_OUT_ERROR;      //NODE not found!

  }
  
  return A2B_BUS_BUSY;

}//end function


int Ca2b_bus_242x::discover_more_nodes(uint8_t node_id, bool start)
{
  uint8_t node_pre = node_id - 1;

  if(start){
    //NOTE: no switch functionality on this function, no phantom power
    if ((node_id >= A2B_MAX_NO_NODES) || (node_id < 1))
      return A2B_MAX_NODE_ERROR;

    if (!nodes[node_pre].linked)
      return A2B_NODE_NOT_LINKED_ERROR;    //node -1 is not linked


    nodes[node_id].linked = false;

    //set node address to previous node
    com_int_->i2c_write(I2C_master, AD242X_NODEADR_REG, node_pre & AD242X_NODEADR_NONE_MSK); // | AD242X_NODEADR_PERI

    //write interrupt mask to enable discovery done
    com_int_->i2c_write(I2C_node, AD242X_INTMSK0_REG, 0x7f);
    //com_int_->i2c_write(I2C_node, AD242X_INTMSK1_REG, 0x7f);
    com_int_->i2c_write(I2C_node, AD242X_INTMSK2_REG, 0x0f);

    //write switch enable switch
    com_int_->i2c_write(I2C_node, AD242X_SWCTL_REG, (A2B_SW_MODE_DEF << AD242X_SWCTL_MODE_POS) | AD242X_SWCTL_ENSW);

    //write the max number of discovery cycles
    com_int_->i2c_write(I2C_master, AD242X_DISCVRY_REG, respnd_cycles_ - (4 * node_id));

    return A2B_BUS_BUSY;
  }

  irq_event(&temp_byte);

  if((temp_byte == AD2425_INTTYPE_DSCDONE)){
    //end discovery mode
    control_reg |= AD242X_CONTROL_ENDDSC;
    com_int_->i2c_write(I2C_master, AD242X_CONTROL_REG, control_reg);
    control_reg &= ~AD242X_CONTROL_ENDDSC;

    //set node address to node_id
    com_int_->i2c_write(I2C_master, AD242X_NODEADR_REG, node_id & AD242X_NODEADR_NONE_MSK); // | AD242X_NODEADR_PERI

    //get node info
    com_int_->i2c_read(I2C_node, AD242X_VENDOR_REG, &nodes[node_id].vendor);
    com_int_->i2c_read(I2C_node, AD242X_PRODUCT_REG, &nodes[node_id].product);
    com_int_->i2c_read(I2C_node, AD242X_VERSION_REG, &nodes[node_id].version);
    com_int_->i2c_read(I2C_node, AD242X_CAPABILITY_REG, &temp_byte) ;
    nodes[node_id].i2c_cap = temp_byte ? true : false;

    //I2C configuration. the node need to be configured according to the audio sample rate.
    //it is used locally to create the I2C clock
    com_int_->i2c_write(I2C_node, AD242X_I2CCFG_REG, 0);

    //write switch mode, no phantom power
    com_int_->i2c_write(I2C_node, AD242X_SWCTL_REG, (A2B_SW_MODE_DEF << AD242X_SWCTL_MODE_POS) | AD242X_SWCTL_ENSW);

    nodes[node_id].linked = true;
    A2B_DEBUG_A2B("A2B_DBG: ID%d node=%d found vendor=%x \n", chain_id_, node_id, nodes[node_id].vendor)

    no_nodes_++;
    last_node_ = no_nodes_ - 1;

    time_out = 2;

    return A2B_NO_ERROR;
  }

  time_out--;

  if (!time_out) {
    //end discovery mode
    control_reg |= AD242X_CONTROL_ENDDSC;
    com_int_->i2c_write(I2C_master, AD242X_CONTROL_REG, control_reg);
    control_reg &= ~AD242X_CONTROL_ENDDSC;

    A2B_DEBUG_A2B("A2B_DBG: ID%d search node=%d time_out ! \n", chain_id_, node_id)
    return A2B_TIME_OUT_ERROR;      //NODE not found!
  }
    
  return A2B_BUS_BUSY;

}//end function


int Ca2b_bus_242x::configure_node_downstream(uint8_t node_id)
{
  //if reg is not used, give parameter as zero
  //NOTE SG: broadcast is not used, the mask and other registers are more flexible

  int status = check_node_id(node_id);
  if (status)
    return status;


  //set node address to node_id
  com_int_->i2c_write(I2C_master, AD242X_NODEADR_REG, node_id & AD242X_NODEADR_NONE_MSK); // | AD242X_NODEADR_PERI

  // B - SIDE
  //downstream slots: defines the number of data slots
  //which are passed downstream (B-side) .
  com_int_->i2c_write(I2C_node, AD242X_DNSLOTS_REG, nodes[node_id].downslotsbypass & AD242X_DNSLOTS_MSK);

  //If any of the bits in the Downstream Data RX Mask registers are set, the
  //AD242X_LDNSLOTS register defines the number of data slots which are added by the local node during the
  //downstream portion of the superframe.
  //When the AD242X_LDNSLOTS.DNMASKEN bit is set, these bits select which downstream slots are consumed by
  //the local node and placed in the TX frame buffer.

  com_int_->i2c_write(I2C_node, AD242X_DNMASK0_REG, nodes[node_id].downslots_msk[0]);
  com_int_->i2c_write(I2C_node, AD242X_DNMASK1_REG, nodes[node_id].downslots_msk[1]);
  com_int_->i2c_write(I2C_node, AD242X_DNMASK2_REG, nodes[node_id].downslots_msk[2]);
  com_int_->i2c_write(I2C_node, AD242X_DNMASK3_REG, nodes[node_id].downslots_msk[3]);


  // A - SIDE
  //local downstream slots: If all of the Downstream Data RX Mask registers are 0 (default condition),
  //the AD242X_LDNSLOTS register defines the number of data slots which are captured by the
  //local node during the downstream portion of the super-frame. These data slots are consumed by the node and not
  //passed on downstream to the next node.
  com_int_->i2c_write(I2C_node, AD242X_LDNSLOTS_REG, (nodes[node_id].downslots_local & AD242X_LDNSLOTS_MSK) | AD242X_LDNSLOTS_DNMASKEN );


  //new structure bit setting needed
  new_structure = true;

  A2B_DEBUG_A2B("DBG_A2B: ID%d node_id = %d ,downslotsbypass=%d, downslots_local=%d downslots_msk=%08x \n",chain_id_ ,node_id, nodes[node_id].downslotsbypass, nodes[node_id].downslots_local, *(uint32_t*)&nodes[node_id].downslots_msk )
  return A2B_NO_ERROR;
}//end function


int Ca2b_bus_242x::configure_node_upstream(uint8_t node_id)
{
  int status = check_node_id(node_id);
  if (status)
    return status;

  //set node address to node_id
  com_int_->i2c_write(I2C_master, AD242X_NODEADR_REG, node_id & AD242X_NODEADR_NONE_MSK); // | AD242X_NODEADR_PERI

  // B - SIDE
  //upslots: defines the number of data slots which are passed upstream (B-side).
  //In the AD242x, a slave node transmits (AD242X_UPSLOTS + AD242X_LUPSLOTS) upstream data slots.
  com_int_->i2c_write(I2C_node, AD242X_UPSLOTS_REG, nodes[node_id].upslotsbypass & AD242X_UPSLOTS_MSK);



  // A - SIDE
  //local upslots: defines the number of data slots which are added by the local
  //node during the upstream portion of the superframe.
  com_int_->i2c_write(I2C_node, AD242X_LUPSLOTS_REG, nodes[node_id].upslots_local & AD242X_LUPSLOTS_MSK);



  //Upstream mask: defines the upstream data slots which are received from the A2B bus by the local node.
  com_int_->i2c_write(I2C_node, AD242X_UPMASK0_REG, nodes[node_id].upslots_msk[0]);
  com_int_->i2c_write(I2C_node, AD242X_UPMASK1_REG, nodes[node_id].upslots_msk[1]);
  com_int_->i2c_write(I2C_node, AD242X_UPMASK2_REG, nodes[node_id].upslots_msk[2]);
  com_int_->i2c_write(I2C_node, AD242X_UPMASK3_REG, nodes[node_id].upslots_msk[3]);

  //new structure bit setting needed
  new_structure = true;


  A2B_DEBUG_A2B("DBG_A2B: ID%d node_id = %d ,upslotsbypass=%d, upslots_local=%d upslots_msk=%08x \n",chain_id_ , node_id, nodes[node_id].upslotsbypass, nodes[node_id].upslots_local, *(uint32_t*)&nodes[node_id].upslots_msk )


  return A2B_NO_ERROR;
}//end function


int Ca2b_bus_242x::configure_master_slots(void)
{
  com_int_->i2c_write(I2C_master, AD242X_DNSLOTS_REG, downstream_slots_ & AD242X_DNSLOTS_MSK);
  com_int_->i2c_write(I2C_master, AD242X_UPSLOTS_REG, upstream_slots_ & AD242X_UPSLOTS_MSK);


  com_int_->i2c_write(I2C_master, AD242X_SLOTFMT_REG, AD242X_SLOTFMT_DNSIZE_24 | AD242X_SLOTFMT_UPSIZE_24);
  com_int_->i2c_write(I2C_master, AD242X_DATCTL_REG, AD242X_DATCTL_UPS | AD242X_DATCTL_DNS );
  com_int_->i2c_write(I2C_master, AD242X_I2SRRATE_REG, 0x01 );

  //new structure bit setting needed
  new_structure = true;

  return A2B_NO_ERROR;
}//end function

//int Ca2b_bus_242x::configure_master_i2c(void);
//int Ca2b_bus_242x::configure_node_i2c((uint8_t node_id);

int Ca2b_bus_242x::configure_master_i2s(uint8_t config)
{

  com_int_->i2c_write(I2C_master, AD242X_I2SGCFG_REG, config); //AD242X_I2SGCFG_EARLY | AD242X_I2SGCFG_TDM8

  com_int_->i2c_write(I2C_master, AD242X_I2SCFG_REG,  AD242X_I2SCFG_TX0EN |
                      AD242X_I2SCFG_RX0EN |
                      AD242X_I2SCFG_TX1EN |
                      #ifndef A2B_MASTER_TXBCLK_RISING
                        AD242X_I2SCFG_TXBCLKINV |
                      #endif
                      AD242X_I2SCFG_RX1EN );

  return A2B_NO_ERROR;
}//end function


int Ca2b_bus_242x::configure_node_i2s(uint8_t node_id, uint8_t config, bool loopback)
{
  int status = check_node_id(node_id);
  if (status)
    return status;

  //set node address to node_id
  com_int_->i2c_write(I2C_master, AD242X_NODEADR_REG, node_id & AD242X_NODEADR_NONE_MSK);

  com_int_->i2c_write(I2C_node, AD242X_I2SGCFG_REG, config); // AD242X_I2SGCFG_EARLY | AD242X_I2SGCFG_TDM8

  com_int_->i2c_write(I2C_node, AD242X_I2SCFG_REG, AD242X_I2SCFG_TX0EN |
                      AD242X_I2SCFG_RX0EN |
                      AD242X_I2SCFG_TX1EN |
                      AD242X_I2SCFG_TXBCLKINV |
                      AD242X_I2SCFG_RX1EN  );

  com_int_->i2c_write(I2C_node, AD242X_PINCFG_REG, AD242X_PINCFG_DRVSTR );

  //just for debugging
  if (loopback)
    com_int_->i2c_write(I2C_node, AD242X_I2STEST_REG, AD242X_I2STEST_BUSLOOPBK);

  return A2B_NO_ERROR;
}//end function




uint8_t Ca2b_bus_242x::get_version(void)
{
  uint8_t temp_byte = 0;
  com_int_->i2c_read(I2C_master, AD242X_VERSION_REG, &temp_byte);

  return temp_byte;

}//end function

uint8_t Ca2b_bus_242x::get_product_id(void)
{
  uint8_t temp_byte = 0;
  com_int_->i2c_read(I2C_master, AD242X_PRODUCT_REG, &temp_byte);

  return temp_byte;
}//end function

uint8_t Ca2b_bus_242x::get_vendor_id(void)
{
  uint8_t temp_byte = 0;
  com_int_->i2c_read(I2C_master, AD242X_VENDOR_REG, &temp_byte);

  return temp_byte;
}//end function

void Ca2b_bus_242x::set_newstruct_master(void)
{
  //set new structure bit in control register
  control_reg |= AD242X_CONTROL_NEWSTRCT;
  com_int_->i2c_write(I2C_master, AD242X_CONTROL_REG, control_reg);

}//end function


int Ca2b_bus_242x::node_linked(uint8_t node_id, bool clear)
{
  if (node_id >= A2B_MAX_NO_NODES)
    return A2B_MAX_NODE_ERROR;

  if (clear) {
    nodes[node_id].linked = false;
    return A2B_NODE_NOT_LINKED_ERROR;
  }

  return nodes[node_id].linked ? A2B_NODE_LINKED : A2B_NODE_NOT_LINKED_ERROR ;

}//end function



int Ca2b_bus_242x::node_i2c_write(uint8_t node_id, uint8_t rmt_i2c_add, uint8_t reg, uint8_t* data, uint8_t length)
{
  int status = check_node_id(node_id);
  if (status)
    return status;


  
  //write node address on NODEADR reg (MASTER)
  com_int_->i2c_write(I2C_master, AD242X_NODEADR_REG, (node_id & AD242X_NODEADR_NONE_MSK));

  //write remote I2C address on CHIP reg (NODE)
  com_int_->i2c_write(I2C_node, AD242X_CHIP_REG, (rmt_i2c_add & AD242X_CHIP_MSK));

  //set PERI bin on NODEADR reg (MASTER)
  com_int_->i2c_write(I2C_master, AD242X_NODEADR_REG, (node_id & AD242X_NODEADR_NONE_MSK) | AD242X_NODEADR_PERI);


  //write data
  if (length > 1)
    com_int_->i2c_write_array(I2C_node, reg, data , length );
  else 
    com_int_->i2c_write(I2C_node, reg, data[0]);

  //clear NODEADR reg (MASTER)
  com_int_->i2c_write(I2C_master, AD242X_NODEADR_REG, 0);


  return A2B_NO_ERROR;
}//end function



int Ca2b_bus_242x::node_i2c_read(uint8_t node_id, uint8_t rmt_i2c_add, uint8_t reg, uint8_t* data, uint8_t length)
{
  int status = check_node_id(node_id);
  if (status)
    return status;

  
  //write node address on NODEADR reg (MASTER)
  com_int_->i2c_write(I2C_master, AD242X_NODEADR_REG, (node_id & AD242X_NODEADR_NONE_MSK));

  //write remote I2C address on CHIP reg (NODE)
  com_int_->i2c_write(I2C_node, AD242X_CHIP_REG, (rmt_i2c_add & AD242X_CHIP_MSK));

  //set PERI bin on NODEADR reg (MASTER)
  com_int_->i2c_write(I2C_master, AD242X_NODEADR_REG, (node_id & AD242X_NODEADR_NONE_MSK) | AD242X_NODEADR_PERI);

  //read data
  if (length > 1)
    status = com_int_->i2c_read_array(I2C_node, reg, data , length );
  else  
    status = com_int_->i2c_read(I2C_node, reg, data);


  //clear NODEADR reg (MASTER)
  com_int_->i2c_write(I2C_master, AD242X_NODEADR_REG, 0);

  if(status)
    return A2B_I2C_ERROR;

  return A2B_NO_ERROR;
}//end function



//return true when a restart discovery round is necesary
int Ca2b_bus_242x::calc_respnd_cycles(void)
{
  uint32_t max;
  uint32_t min;

  min = (downstream_slots_ * (A2B_DEF_BIT_RES+1) + 64) / 4 + 1 + 4 * (no_nodes_ - 1);  //+1 -> parity; 64 synch header and control
  max = 248 - (upstream_slots_ * (A2B_DEF_BIT_RES+1) + 64) / 4;

  A2B_DEBUG_A2B("response cyles  min=%d max=%d act=%d \n",min,max,respnd_cycles_)


  if(min > max)
    return A2B_RESPCLY_ERROR;

  if(respnd_cycles_ < min || respnd_cycles_ > max){
    respnd_cycles_ = (max + min) >> 1;      //TBD
    //respnd_cycles_ = min + 2;     //TBD
    //respnd_cycles_ = max - 5;     //TBD
    return A2B_RESTART_DISCVY;
  }

  return A2B_NO_ERROR;

}//end function

int Ca2b_bus_242x::check_node_id(uint8_t node_id)
{
  if (node_id >= A2B_MAX_NO_NODES)
    return A2B_MAX_NODE_ERROR;

  if ((node_id + 1) > no_nodes_ ) //invalid node id
    return A2B_NODE_ID_INVALID;

  if (!nodes[node_id].linked)
    return A2B_NODE_NOT_LINKED_ERROR;

  return A2B_NO_ERROR;
}//end function


void Ca2b_bus_242x::reset_comint(void)
{
  com_int_->reset();
}

//void Ca2b_bus_242x::configure_com_buffers(uint8_t* tx_buf_p, uint8_t* rx_buf_p, uint8_t size) {
//  empty function. Will be overwritten from slave
//}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////  Private




