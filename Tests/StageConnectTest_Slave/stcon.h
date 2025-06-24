#pragma once

#include "stcon_user.h"
#include "a2b_bus.h"
#include "AD2425_inc.h"
#include "types.h"

//#define A2B_UPDATE_NODE  1

/*----------------   macros   -----------------*/
// #define A2B_MAX_BUF_SIZE    256
// #define A2B_SC_VER          01
// #define A2B_SC_BID          00
#define A2B_DEF_I2C_ADD     0x3d
#define A2B_MAX_SC_PWR      180   //  18W  12*1.5A
#define A2B_SLV_LINK_CNT    20    //  2 seconds function should be called every 100ms


/*----------------   strcuts and enums   -----------------*/
struct A2B_Ssc_node {
  U8 api_ver;   //current version= 1
  U8 brand_id;
  U8 product_id[2]; //product_id[1] bit 7 is 1 for SC module
  U8 downslots4node;  //slave only, audio slots coming from A-side consumed by the node, 
  U8 upslots4node;  //slave only,audio slots coming from B-side consumed by the node, 
  U8 downslots_local; //audio slots going to B-side provided by the node, max downslots on master
  U8 upslots_local; //audio slots going to A-side provided by the node, max upslots on master
  U8 tdm_config;    //see note 1
  U8 tdm_loop_back; //slave only, for debugging
  U8 bcst_mode;   //slave only, the nodes receives audio from ch1 to ch_n
  U8 node_pwr;    //slave only. gives the node power in W x 10. e.g. 125 = 12.5W
  U8 status;      // 0x0C read for info on slave node.  see note below 
  U8 control;     // 0x0D write to control slave node.  see note below 
  U8 rst_key;     //must be '~' in order to apply reset   
  U8 buffers_size;  //slave only, tx and rx buffer size in  bytes  max. 2^16!!
  U8 rx_buf_no_bytes; //slave only, how many bytes were received
  U8 dummy0;
  U8 rx_buf_p;    //slave only, pointer to rx buffer
  U8 tx_buf_no_bytes; //slave only, how many bytes are to be transmitted
  U8 dummy1;
  U8 tx_buf_p;    //slave only, pointer to tx buffer
  U8 stuffing[2];    //SG NOTE: keep the length align 4
};


/*----------------   Clases   -----------------*/
class Csc_master: public Ca2b_bus_242x
{
  public:
    Csc_master(A2B_Ssc_node* def, U8 local_id, U8 I2C_address);
    
    void reset(void){master_a2b_= 1; reset_bus();}
    void modify_node(A2B_Ssc_node* def); // reset required
    void event_handler(void);
    int  start_discovery_rnd(void);
    int  configure_chain(void);
    int  configure_master(void);
    int  send_message(U8 node_id, U8* data, U8 length);
    int  get_message(U8 node_id, U8* target, U8 max_len);

    // remote update
    void conf_fw_update(U8* fw_p, U32 fw_len);
    int  update_node(U8 node_id);
    void update_allnodes(void);

    // bus status and control
    int  send_link_stat(void);
    int  check_nodes_stat(void);
    int  change_node_audio_def(U8 node_id, U8 downslots4node, U8 upslots_local);
    int  get_num_active_nodes();
    bool get_node_def(A2B_Ssc_node* def, U8 node_id);

    bool sc_main_cyclic(bool reset);
    int  check_link(void);
    int check_link_node(U8 node);

    void clear_streams(void);
    int  check_chain_power(void);

    void samp_rate_is_44_1(bool en){is_44_1_ = en;}

    // channel upstream allocation
    bool get_ch_alloc_flag(void);
    U8   get_ch_alloc(U32 *lut);

    int status_g;
    
  private:
    int  get_node_def_int(A2B_Ssc_node* def, U8 node_id);
    void balance_upstream(void);
    void balance_downstream(void);
    void balance_streams(void);
    // void balance_downstream_bdcs(void);
    void myxtoa_byte(U8 *str, U8 var);

    bool chain_runnig_;
    bool is_44_1_;
    bool ch_alloc_changed_;

    // remote update
    U8*  fw_buf_;
    U32  fw_size_;

    U8   get_reg_add(U8* ofset);
    U32  calc_mask(U8 no_slots);


    A2B_Ssc_node* l_node_def;
    A2B_Ssc_node sc_nodes_def[A2B_MAX_NO_NODES];

    U8   chain_pwr_;
    U8   local_id_;
    U8   dis_status_;
    U8   dis_node_idx_;

    U16  data_rdy_on_slv_; // zero is no data, one bit per note
};


class Csc_slave: public Ca2b_bus_242x
{
  public:
    Csc_slave(A2B_Ssc_node* def, U8 local_id, U8 I2C_address);
    
    void event_handler(void);
    void reset(void){master_a2b_= 0; reset_bus();}

    //I2C com interface
    U8   read_reg(void);
    void set_reg_add(U8 add);
    void write_reg(U8 data);

    // buffered communication
    void configure_com_buffers(U8* tx_buf_p, U8* rx_buf_p, U8 size);
    int  send_message(U8* data, U8 length);
    int  get_message(U8* target, U8 max_len);

    //bus status and control
    int  check_link(void);
    bool check_ctl_reg_bit(U8 bit_def);
    void set_status_reg_bit(U8 bit_def);

  private:
    // buffered communication
    void write_to_buf(U8 data);
    U8   read_from_buf( void);


    A2B_Ssc_node* l_node_def;
    U8*  l_node_def_byte;

    U8*  rx_buffer;
    U8*  tx_buffer;

    U16  rx_idx;
    U16  tx_idx;
    
    U8   reg_address;
    U8   buf_size;
    U8   link_cnt_;
    U8   link_cnt_ctl_;

    int  link_ret_;
};


// control register: master write to this address to control the node
#define A2B_CTL_REG_LINK    (1 << 7)    //node is linked to master via A2B bus, it is toggled by master. If toggle is lost, link is lost
#define A2B_CTL_REG_CRC_E   (1 << 6)    //CRC error on the bus
#define A2B_CTL_REG_SFM_E   (1 << 5)    //super frame error
#define A2B_CTL_REG_DEF_U   (1 << 4)    //set by master when node def has been changed by master
#define A2B_CTL_REG_EN_PWR  (1 << 3)    //enable device power
#define A2B_CTL_REG_44_1    (1 << 2)    //sample rate is 44.1 Khz

#define A2B_CTL_REG_RST_M   (1 << 0)    //sort reset module, use for updating

// status register: master reads this reg to find out about changes on node
#define A2B_STAT_REG_RDIS   (1 << 7)    //node definition has changed, a new discovery round is needed
#define A2B_STAT_REG_BUF_F  (1 << 6)    //the TX buffer has data to be read

