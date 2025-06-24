#pragma once

#include "stcon_user.h"


#define A2B_MAX_NO_NODES          10
#define A2B_RESPND_CYCLES_INIT    130 //  0x7d
#define A2B_DEF_BIT_RES           24
#define A2B_SW_MODE_DEF           2   // see pag. 69 of hw ref  3 for debug and engineering


//Error codes
#define A2B_TIME_OUT_ERROR          -1
#define A2B_MAX_NODE_ERROR          -2
#define A2B_NODE_NOT_LINKED_ERROR   -3
#define A2B_MST_INIT_ERROR          -4
#define A2B_NODE_ID_INVALID         -5
#define A2B_RESPCLY_ERROR           -6
#define A2B_TX_BUF_NOT_EMPTY        -7
#define A2B_NO_TX_BUF_P             -8
#define A2B_NO_RX_BUF_P             -9
#define A2B_NO_UPDATE_BUF_P         -10
#define A2B_I2C_ERROR               -11

//Status code
#define A2B_NO_ERROR                0
#define A2B_NODE_LINKED             1
#define A2B_NODE_LINKED_WITH_ERR    2
#define A2B_RESTART_DISCVY          3
#define A2B_BUS_BUSY                4



class Ca2b_bus_242x
{
  public:
    Ca2b_bus_242x(uint8_t a2b_id, uint8_t I2C_address, Ci2c_com* com_int);

    void set_as_master(void);
    int  is_master_running(void);
    int  event_handler(void);

    uint8_t   get_version(void);
    uint8_t   get_product_id(void);
    uint8_t   get_vendor_id(void);

    int  discover_1st_node(bool start);
    int  discover_more_nodes(uint8_t node_id, bool start);
    void reset_bus(void);
    void reset_comint(void);

    // virtual function-prototypes for polymorphic implementation
	//virtual ~Ca2b_bus_242x() = default;
	//virtual void configure_com_buffers(uint8_t* tx_buf_p, uint8_t* rx_buf_p, uint8_t size);
    
	//audio slots configuration
    int  configure_master_slots(void);
    int  configure_node_downstream(uint8_t node_id); // uint8_t bypass_slots,  uint8_t local_slots, uint8_t slots4node);
    int  configure_node_upstream(uint8_t node_id);   // uint8_t bypass_slots,  uint8_t local_slots, uint8_t slots4node);

    uint8_t   irq_event(uint8_t* event_id);
    int  configure_master_i2s(uint8_t config);
    int  configure_node_i2s(uint8_t node_id, uint8_t config, bool loopback);
    void set_newstruct_master(void);
    int  calc_respnd_cycles(void);
    
    int  node_linked(uint8_t node_id, bool clear);
    int  check_node_id(uint8_t node_id);

    // I2C
    int  node_i2c_write(uint8_t node_id, uint8_t rmt_i2c_add, uint8_t reg, uint8_t* data, uint8_t length);
    int  node_i2c_read(uint8_t node_id, uint8_t rmt_i2c_add, uint8_t reg, uint8_t* data, uint8_t length);

    void set_def_respcycles(uint8_t value){respnd_cycles_=value;}
    
    struct Snode {
      uint8_t vendor;    
      uint8_t product;   
      uint8_t version;
      bool i2c_cap;
      bool linked;
      uint8_t __attribute__((aligned(4)))downslots_msk[4]; //audio slots coming from B-side consumed by the node
      uint8_t upslots_msk[4];  //audio slots coming from A-side consumed by the node 
      uint8_t downslotsbypass; //audio slots coming from B-side bypassed by the node 
      uint8_t upslotsbypass; //audio slots coming from A-side bypassed by the node 
      uint8_t downslots_local; //audio slots going to B-side provided by the node
      uint8_t upslots_local; //audio slots going to A-side provided by the node 
    };

    // configuration
    uint8_t downstream_slots_;
    uint8_t upstream_slots_;
    uint8_t no_nodes_;
    uint8_t last_node_;
    uint8_t master_a2b_;
    Snode nodes[A2B_MAX_NO_NODES];

  private:
    uint8_t respnd_cycles_;
    uint8_t chain_id_;
    uint8_t I2C_master;
    uint8_t I2C_node;
    uint8_t control_reg;
    Ci2c_com* com_int_;

    bool new_structure;

    // auxiliary variables
    uint8_t temp_byte;
    uint32_t time_out;
};



