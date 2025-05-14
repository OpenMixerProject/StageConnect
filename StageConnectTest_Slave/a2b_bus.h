#pragma once

#include "stcon_user.h"
#include "types.h"

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
    Ca2b_bus_242x(U8 a2b_id, U8 I2C_address);

    void set_as_master(void);
    int  is_master_running(void);
    int  event_handler(void);

    U8   get_version(void);
    U8   get_product_id(void);
    U8   get_vendor_id(void);

    int  discover_1st_node(bool start);
    int  discover_more_nodes(U8 node_id, bool start);
    void reset_bus(void);
    void reset_comint(void);

    //a udio slots configuration
    int  configure_master_slots(void);
    int  configure_node_downstream(U8 node_id); // U8 bypass_slots,  U8 local_slots, U8 slots4node);
    int  configure_node_upstream(U8 node_id);   // U8 bypass_slots,  U8 local_slots, U8 slots4node);

    U8   irq_event(U8* event_id);
    int  configure_master_i2s(U8 config);
    int  configure_node_i2s(U8 node_id, U8 config, bool loopback);
    void set_newstruct_master(void);
    int  calc_respnd_cycles(void);
    
    int  node_linked(U8 node_id, bool clear);
    int  check_node_id(U8 node_id);

    // I2C
    int  node_i2c_write(U8 node_id, U8 rmt_i2c_add, U8 reg, U8* data, U8 length);
    int  node_i2c_read(U8 node_id, U8 rmt_i2c_add, U8 reg, U8* data, U8 length);

    void set_def_respcycles(U8 value){respnd_cycles_=value;}
    
    struct Snode {
      U8 vendor;    
      U8 product;   
      U8 version;
      bool i2c_cap;
      bool linked;
      U8 __attribute__((aligned(4)))downslots_msk[4]; //audio slots coming from B-side consumed by the node
      U8 upslots_msk[4];  //audio slots coming from A-side consumed by the node 
      U8 downslotsbypass; //audio slots coming from B-side bypassed by the node 
      U8 upslotsbypass; //audio slots coming from A-side bypassed by the node 
      U8 downslots_local; //audio slots going to B-side provided by the node
      U8 upslots_local; //audio slots going to A-side provided by the node 
    };

    // configuration
    U8 downstream_slots_;
    U8 upstream_slots_;
    U8 no_nodes_;
    U8 last_node_;
    U8 master_a2b_;
    Snode nodes[A2B_MAX_NO_NODES];

  private:
     void ArduinoI2CWrite(U8 a2b_id, U8 I2C_address, U8 data); 
     void ArduinoI2CWrite_array( U8 a2b_id, U8 I2C_address, U8 * data, U8 len);
     int ArduinoI2CRead (U8 a2b_id, U8 I2C_address, U8 * returndata);
    int  ArduinoI2CRead_array (U8 a2b_id, U8 I2C_address, U8 * returndata, U8 len);
    U8 respnd_cycles_;
    U8 chain_id_;
    U8 I2C_master;
    U8 I2C_node;
    U8 control_reg;
    

    bool new_structure;

    // auxiliary variables
    U8 temp_byte;
    U32 time_out;
};



