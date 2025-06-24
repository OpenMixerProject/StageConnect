#pragma once

#include "stcon_user.h"
#include "a2b_bus.h"
#include "AD2425_inc.h"


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
  uint8_t api_ver;   //current version= 1
  uint8_t brand_id;
  uint8_t product_id[2]; //product_id[1] bit 7 is 1 for SC module
  uint8_t downslots4node;  //slave only, audio slots coming from A-side consumed by the node, 
  uint8_t upslots4node;  //slave only,audio slots coming from B-side consumed by the node, 
  uint8_t downslots_local; //audio slots going to B-side provided by the node, max downslots on master
  uint8_t upslots_local; //audio slots going to A-side provided by the node, max upslots on master
  uint8_t tdm_config;    //see note 1
  uint8_t tdm_loop_back; //slave only, for debugging
  uint8_t bcst_mode;   //slave only, the nodes receives audio from ch1 to ch_n
  uint8_t node_pwr;    //slave only. gives the node power in W x 10. e.g. 125 = 12.5W
  uint8_t status;      // 0x0C read for info on slave node.  see note below 
  uint8_t control;     // 0x0D write to control slave node.  see note below 
  uint8_t rst_key;     //must be '~' in order to apply reset   
  uint8_t buffers_size;  //slave only, tx and rx buffer size in  bytes  max. 2^16!!
  uint8_t rx_buf_no_bytes; //slave only, how many bytes were received
  uint8_t dummy0;
  uint8_t rx_buf_p;    //slave only, pointer to rx buffer
  uint8_t tx_buf_no_bytes; //slave only, how many bytes are to be transmitted
  uint8_t dummy1;
  uint8_t tx_buf_p;    //slave only, pointer to tx buffer
  uint8_t stuffing[2];    //SG NOTE: keep the length align 4
};


/*----------------   Clases   -----------------*/
class Csc_master: public Ca2b_bus_242x
{
  public:
    Csc_master(A2B_Ssc_node* def, uint8_t local_id, uint8_t I2C_address, Ci2c_com* com_int);
    
    void reset(void){master_a2b_= 1; reset_bus();}
    void modify_node(A2B_Ssc_node* def); // reset required
    void event_handler(void);
    int  start_discovery_rnd(void);
    int  configure_chain(void);
    int  configure_master(void);
    int  send_message(uint8_t node_id, uint8_t* data, uint8_t length);
    int  get_message(uint8_t node_id, uint8_t* target, uint8_t max_len);

    // remote update
    void conf_fw_update(uint8_t* fw_p, uint32_t fw_len);
    int  update_node(uint8_t node_id);
    void update_allnodes(void);

    // bus status and control
    int  send_link_stat(void);
    int  check_nodes_stat(void);
    int  change_node_audio_def(uint8_t node_id, uint8_t downslots4node, uint8_t upslots_local);
    int  get_num_active_nodes();
    bool get_node_def(A2B_Ssc_node* def, uint8_t node_id);

    bool sc_main_cyclic(bool reset);
    int  check_link(void);
    int check_link_node(uint8_t node);

    void clear_streams(void);
    int  check_chain_power(void);

    void samp_rate_is_44_1(bool en){is_44_1_ = en;}

    // channel upstream allocation
    bool get_ch_alloc_flag(void);
    uint8_t   get_ch_alloc(uint32_t *lut);

    int status_g;
    
  private:
    int  get_node_def_int(A2B_Ssc_node* def, uint8_t node_id);
    void balance_upstream(void);
    void balance_downstream(void);
    void balance_streams(void);
    // void balance_downstream_bdcs(void);
    void myxtoa_byte(uint8_t *str, uint8_t var);

    bool chain_runnig_;
    bool is_44_1_;
    bool ch_alloc_changed_;

    // remote update
    uint8_t*  fw_buf_;
    uint32_t  fw_size_;

    uint8_t   get_reg_add(uint8_t* ofset);
    uint32_t  calc_mask(uint8_t no_slots);


    A2B_Ssc_node* l_node_def;
    A2B_Ssc_node sc_nodes_def[A2B_MAX_NO_NODES];

    uint8_t   chain_pwr_;
    uint8_t   local_id_;
    uint8_t   dis_status_;
    uint8_t   dis_node_idx_;

    uint16_t  data_rdy_on_slv_; // zero is no data, one bit per note
};


class Csc_slave: public Ca2b_bus_242x
{
  public:
    Csc_slave(A2B_Ssc_node* def, uint8_t local_id, uint8_t I2C_address, Ci2c_com* com_int);
    
    void event_handler(void);
    void reset(void){master_a2b_= 0; reset_bus();}

    //I2C com interface
    uint8_t   read_reg(void);
    void set_reg_add(uint8_t add);
    void write_reg(uint8_t data);

    // buffered communication
    //void configure_com_buffers(uint8_t* tx_buf_p, uint8_t* rx_buf_p, uint8_t size); override;
	void configure_com_buffers(uint8_t* tx_buf_p, uint8_t* rx_buf_p, uint8_t size);
    int  send_message(uint8_t* data, uint8_t length);
    int  get_message(uint8_t* target, uint8_t max_len);

    //bus status and control
    int  check_link(void);
    bool check_ctl_reg_bit(uint8_t bit_def);
    void set_status_reg_bit(uint8_t bit_def);

  private:
    // buffered communication
    void write_to_buf(uint8_t data);
    uint8_t   read_from_buf( void);


    A2B_Ssc_node* l_node_def;
    uint8_t*  l_node_def_byte;

    uint8_t*  rx_buffer;
    uint8_t*  tx_buffer;

    uint16_t  rx_idx;
    uint16_t  tx_idx;
    
    uint8_t   reg_address;
    uint8_t   buf_size;
    uint8_t   link_cnt_;
    uint8_t   link_cnt_ctl_;

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

