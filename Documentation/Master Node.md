# Master Node

## Csc_master Class Definition

```cpp
class Csc_master: public Ca2b_bus_242x
{
  public:
    Csc_master(A2B_Ssc_node* def, U8 local_id, U8 I2C_address, Ci2c_com* com_int);
    
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
```
## Functions

  - [Csc_master(A2B_Ssc_node* def, U8 local_id, U8 I2C_address, Ci2c_com* com_int)](#csc_mastera2b_ssc_node-def-u8-local_id-u8-i2c_address-ci2c_com-com_int)
  - [Csc_master::reset()](#csc_masterreset)
  - [Csc_master::modify_node(A2B_Ssc_node* def)](#csc_mastermodify_nodea2b_ssc_node-def)
  - [Csc_master::event_handler()](#csc_masterevent_handler)
  - [Csc_master::start_discovery_rnd()](#csc_masterstart_discovery_rnd)
  - [Csc_master::configure_chain()](#csc_masterconfigure_chain)
  - [Csc_master::configure_master()](#csc_masterconfigure_master)
  - [Csc_master::send_message(U8 node_id, U8* data, U8 length)](#csc_mastersend_messageu8-node_id-u8-data-u8-length)
  - [Csc_master:get_message(U8 node_id, U8* target, U8 max_len)](#csc_masterget_messageu8-node_id-u8-target-u8-max_len)
  - [Csc_master::conf_fw_update(U8* fw_p, U32 fw_len)](#csc_masterconf_fw_updateu8-fw_p-u32-fw_len)
  - [Csc_master::update_node(U8 node_id)](#csc_masterupdate_nodeu8-node_id)
  - [Csc_master::update_allnodes()](#csc_masterupdate_allnodes)
  - [Csc_master::send_link_stat()](#csc_mastersend_link_stat)
  - [Csc_master::check_nodes_stat()](#csc_mastercheck_nodes_stat)
  - [Csc_master::change_node_audio_def(U8 node_id, U8 downslots4node, U8 upslots_local)](#csc_masterchange_node_audio_defu8-node_id-u8-downslots4node-u8-upslots_local)
  - [Csc_master::get_num_active_nodes()](#csc_masterget_num_active_nodes)
  - [Csc_master::get_node_def(A2B_Ssc_node* def, U8 node_id)](#csc_masterget_node_defa2b_ssc_node-def-u8-node_id)
  - [Csc_master::sc_main_cyclic(bool reset)](#csc_mastersc_main_cyclicbool-reset)
  - [Csc_master::check_link()](#csc_mastercheck_link)
  - [Csc_master::clear_streams()](#csc_masterclear_streams)
  - [Csc_master::check_chain_power()](#csc_mastercheck_chain_power)
  - [Csc_master::samp_rate_is_44_1(bool en)](#csc_mastersamp_rate_is_44_1bool-en)
  - [Csc_master::get_ch_alloc_flag()](#csc_masterget_ch_alloc_flag)
  - [Csc_master::get_ch_alloc(U32 *lut)](#csc_masterget_ch_allocu32-lut)
  - [Csc_master::int status_g](#csc_masterstatus_g)
  
## Usage examples
  - [Initialization](#initialization)
  - [Cyclic](#cyclic)
  - [Master Node Usage](#master-node-usage)

## Csc_master

## Csc_master::Csc_master(A2B_Ssc_node* def, U8 local_id, U8 I2C_address, Ci2c_com\* com_int)

Constructor for the Master Node

| Arguments            |              |  |
| :------------------- |:------------ | :------ |
| A2B_Ssc_node*        | def          | pointer to Stage Connect Node definition structure |
| U8                   | local_id     | Local Id |
| U8                   | I2C_address  | i2c address |
| Ci2c_com* com_int    | com_int      | pointer to i2c interface |

## Csc_master::reset()

Resets the Stage Connect interface

## Csc_master::modify_node(A2B_Ssc_node* def)

Modifies any parameter of the Stage Connect Node definition

| Arguments            |              |  |
| :------------------- |:------------ | :------ |
| A2B_Ssc_node*        | def          | pointer to Stage Connect Node |

## Csc_master::event_handler()

Handler for the Interrupt type register of the a2b chip

## Csc_master::start_discovery_rnd()

This function search for nodes connected to the master

## Csc_master::configure_chain()

This function checks the status of the save nodes connected in the chain and configures the data slots starting on the downstream configuration and then the upstream configuration one node at a time, the configuration will begin on the closest node to the master.

| returns |  |
| :------ | :------ |
| A2B_NO_ERROR 0 | No error in configuration |
| A2B_MAX_NODE_ERROR -2 | Too many nodes on the chain |
| A2B_NODE_NOT_LINKED_ERROR -3 | Node not linked |
| A2B_NODE_ID_INVALID -5 | invalid node Id |

## Csc_master::configure_master()

Configures the master node tdm configuration and data slots.

| returns |  |
| :------ | :------ |
| A2B_NO_ERROR 0 | No error in configuration |
| A2B_MAX_NODE_ERROR -2 | Too many nodes on the chain |
| A2B_NODE_NOT_LINKED_ERROR -3 | Node not linked |

## Csc_master::send_message(U8 node_id, U8* data, U8 length)

Sends a message to a specific node on the chain.

| Arguments            |              |  |
| :------------------- |:------------ | :------ |
| U8         | node_id          | Stage Connect Node id |
| U8*        | data          | pointer to data |
| U8         | length          | number of bytes to be set |

| returns |  |
| :------ | :------ |
| 0 | buffer is busy |
| length | positive number with data length |
| A2B_NODE_ID_INVALID  -5 | invalid node Id |

## Csc_master::get_message(U8 node_id, U8* target, U8 max_len)

Function to get a message from node

| Arguments            |              |  |
| :------------------- |:------------ | :------ |
| U8         | node_id          | pointer to Stage Connect Node |
| U8*        | target          | pointer to store data |
| U8         | max_len          | number of bytes to read |

| returns |  |
| :------ | :------ |
| 0 | buffer is empty |
| no_bytes | positive number with data length |
| A2B_NODE_ID_INVALID  -5 | invalid node Id |

## Csc_master::conf_fw_update(U8* fw_p, U32 fw_len)

Configures the firmware update function with a pointer to the firmware and the length of the firmware

## Csc_master::update_node(U8 node_id)

Sends the firmware to the SC slave node

## Csc_master::update_allnodes()

Runs the update function for each node of the chain

## Csc_master::send_link_stat()

Called by sc_main_cyclic, sends the link status to the nodes

## Csc_master::check_nodes_stat()

Called by sc_main_cyclic, reads the slave nodes state

## Csc_master::change_node_audio_def(U8 node_id, U8 downslots4node, U8 upslots_local)

Changes the audio slots on the selected node

## Csc_master::get_num_active_nodes()

Returns the number of nodes in the chain

## Csc_master::get_node_def(A2B_Ssc_node* def, U8 node_id)

Reads the selected node

## Csc_master::sc_main_cyclic(bool reset)

Function that must be called periodically, checks the event handler, initially configures the nodes, and checks if the chain is running, can be reset for reconfiguration of the chain

## Csc_master::check_link()

Checks if the stage connect link status

| returns |  |
| :------ | :------ |
| A2B_NODE_LINKED 1 | SC link active |
| A2B_NODE_LINKED_WITH_ERR 2 | SC link active but an error occurred that was not critical |
| A2B_NODE_NOT_LINKED_ERROR -3 | error on the SC link |

## Csc_master::clear_streams()

Clears all the slot allocation in all the nodes

## Csc_master::check_chain_power()

If the slave nodes are in phantom power mode, it keeps adding the power requirements of the nodes until it reaches the maximum allowable power than will provoke the process to end or when it reaches a node that is self powered what will reset the count at zero and to add after that

## Csc_master::samp_rate_is_44_1(bool en)

sets the sample rate of the audio

## Csc_master::get_ch_alloc_flag()

checks if the channel allocation has changed

## Csc_master::get_ch_alloc(U32 *lut)

gets to the pointer the channel allocation.

## Csc_master::status_g

This is an accessible variable that is helpful to catch connection errors or freezes on the connection when SC is being used as plug and play. The difference between the variable and the status is that this one is specifically set by the discovery round. This variable is to be checked by the master node, when this variable has the value of -4, *A2B_MST_INIT_ERROR*, it means that the a2b ic could not be initialized as master and requires a reset and re-initialization

## Usage Examples

### initialization

```cpp
void init_master{
  i2c.i2c_int_en(false);
  i2c.i2c_init();
  sc_master.reset();
  link_status = A2B_NODE_NOT_LINKED_ERROR;
  prev_link_status = A2B_NODE_NOT_LINKED_ERROR;
  VU32 *p = (VU32 *)(16 * 1024 + FLASH_BASE);
  sc_master.conf_fw_update((U8*)p , (U32)p[8]);
  sc_linked = sc_master.sc_main_cyclic(true);
}
```

Start with the initialization on the i2c interface with the interrupt disabled

The method *reset()* is called to make sure the nodes are all in a blank state

### Cyclic

```cpp
void sc_cyclic_master() {
  sc_linked = sc_master.sc_main_cyclic(false);
  link_status = sc_master.check_link();

  switch (link_status) {
    case A2B_NODE_NOT_LINKED_ERROR:
      //for instance the status yellow led
    break;

    case A2B_NODE_LINKED:
      //for instance the status green led
    break;

    case A2B_NODE_LINKED_WITH_ERR:
      //for instance the status red led
    break;

    default:

    break;
  }
}
```

Fist it is needed to run *sc_main_cyclic(false)* to at the beginning to configure the nodes and to keep the link active.

The method *check_link()*, wil get the status of the SC link and with it we can decide what to do according to the response, for instance the status led's would be updated here

### Master Node Usage

```cpp
//initialize the node

A2B_Ssc_node mst_node_def = {
  1,
  1,
  {0x01, 0x00},
  0,
  0,
  16,
  16,
  AD242X_I2SGCFG_EARLY | AD242X_I2SGCFG_TDM8,
  false,
  true,
  0,
  0,
  0
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  {0,0}
};

//initialize i2c
Ci2c_com i2c_int = Ci2c_com(DN_I2C_INTERFACE,
                            DN_I2C_SPEED,
                            DN_I2C_GPIO,
                            DN_I2C_SCL_PIN,
                            DN_I2C_SDA_PIN,
                            DN_I2C_PRIO);

//construct the object
Csc_master sc_master(&mst_node_def,1, 0xd0 , &i2c_int);

void main() {

  i2c_int.i2c_int_en(false);
  i2c_int.i2c_init();
  sc_master.reset();
  link_status = A2B_NODE_NOT_LINKED_ERROR;
  prev_link_status = A2B_NODE_NOT_LINKED_ERROR;
  VU32 *p = (VU32 *)(16 * 1024 + FLASH_BASE);
  sc_master.conf_fw_update((U8*)p , (U32)p[8]);
  sc_linked = sc_master.sc_main_cyclic(true);

  while (1){
    sleep(100); # wait 100 milliseconds

    sc_linked = sc_master.sc_main_cyclic(false);
    link_status = sc_master.check_link();

    switch (link_status) {
      case A2B_NODE_NOT_LINKED_ERROR:

      break;

      case A2B_NODE_LINKED:

      break;

      case A2B_NODE_LINKED_WITH_ERR:

      break;

      default:

      break;
    }
  }
}
```

The first thing needed is to initialize the master node

It is also needed to initialize i2c for the API to communicate with the a2b chip

On the *main* part of the example fist the i2c and SC is initialized, in this phase the SC will find and configure the nodes as here the method *sc_main_cyclic* is run with a *true* argument.

On the cyclic part, *sc_main_cyclic* is run with a *false* argument, this will, after the configuration round done, keep the SC link alive. the *check_link* method will get the SC link status that will be used to do various things like changing the color of status led's, (typically red for no link, green for a fully working link and yellow for a link established with problems) and in case of a connection failing completely it can be used to activate flags, functions or methods to handle the problem to reconnect or reinitialize the SC link.
