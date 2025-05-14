# Slave Node

## Csc_slave Class Definition

```cpp
class Csc_slave: public Ca2b_bus_242x
{
  public:
    Csc_slave(A2B_Ssc_node* def, U8 local_id, U8 I2C_address, Ci2c_com* com_int);
    
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
```
## Functions
  - [Csc_slave(A2B_Ssc_node* def, U8 local_id, U8 I2C_address, Ci2c_com* com_int)](#csc_slavea2b_ssc_node-def-u8-local_id-u8-i2c_address-ci2c_com-com_int)
  - [Csc_slave::event_handler()](#csc_slaveevent_handler)
  - [Csc_slave::reset()](#csc_slavereset)
  - [Csc_slave::read_reg()](#csc_slaveread_reg)
  - [Csc_slave::set_reg_add(U8 add)](#csc_slaveset_reg_addu8-add)
  - [Csc_slave::write_reg(U8 data)](#csc_slavewrite_regu8-data)
  - [Csc_slave::configure_com_buffers(U8* tx_buf_p, U8* rx_buf_p, U8 size)](#csc_slaveconfigure_com_buffersu8-tx_buf_p-u8-rx_buf_p-u8-size)
  - [Csc_slave::send_message(U8* data, U8 length)](#csc_slavesend_messageu8-data-u8-length)
  - [Csc_slave::get_message(U8* target, U8 max_len)](#csc_slaveget_messageu8-target-u8-max_len)
  - [Csc_slave::check_link()](#csc_slavecheck_link)
  - [Csc_slave::check_ctl_reg_bit(U8 bit_def)](#csc_slavecheck_ctl_reg_bitu8-bit_def)
  - [Csc_slave::set_status_reg_bit(U8 bit_def)](#csc_slaveset_status_reg_bitu8-bit_def)

## Usage Examples
  - [Initialization](#initialization)
  - [Cyclic](#cyclic)
  - [Node Example](#node-example)
  
## Csc_slave(A2B_Ssc_node* def, U8 local_id, U8 I2C_address, Ci2c_com* com_int)

Initialization of the Slave Node

## Csc_slave::event_handler()

Handle for the connection status

## Csc_slave::reset()

Resets the node

## Csc_slave::read_reg()

Read register on predefined address and auto increment if address is equal to rx buffer read data from buffer

## Csc_slave::set_reg_add(U8 add)

This method set the register address to be  read or written if address is tx or rx buffer  the indexes are reset

## Csc_slave::write_reg(U8 data)

Write to register write to predefined address and auto increment id address is equal to tx buffer write to buffer

## Csc_slave::configure_com_buffers(U8* tx_buf_p, U8* rx_buf_p, U8 size)

Configure auxiliary communication buffers

## Csc_slave::send_message(U8* data, U8 length)

Used to send a message to master

| Parameters | | |
| :----- | :------ | :------ |
| U8* | data | pointer to data |
| U8 | length | number of bytes to be sent |

| Returns | | |
| :----- | :------ | :------ |
| A2B_NO_TX_BUF_P | -8 | com buffer does nor exist |
| A2B_TX_BUF_NOT_EMPTY | -7 | the buffer is not empty |
| A2B_NO_ERROR | 0 | operation successful |

## Csc_slave::get_message(U8* target, U8 max_len)

Used to get a message from master

| Parameters | | |
| :----- | :------ | :------ |
| U8* | target | pointer to store data |
| U8 | max_len | number of bytes to receive |

| Returns | | |
| :----- | :------ | :------ |
| A2B_NO_RX_BUF_P | -9 | com rx buffer does not exist |
|| >=0 | number of byte received  |

## Csc_slave::check_link()

Checks if SC link is established, if the flag is not set by the master regularly the link will be declared as broken after 4s of inactivity by the master

| returns |  | |
| :------ | :------ | :------ |
| A2B_NODE_LINKED  | 1 | SC link active |
| A2B_NODE_LINKED_WITH_ERR | 2 | SC link active but an error occurred that was not critical |
| A2B_NODE_NOT_LINKED_ERROR | -3 | error on the SC link |

## Csc_slave::check_ctl_reg_bit(U8 bit_def)

Checks the control register on the node

## Csc_slave::set_status_reg_bit(U8 bit_def)

Sets the status register on the node

## Usage Examples

### Initialization

```cpp
void sc_slave_init(){
  sc_slave.reset();
  i2c.i2c_init();
  i2c.i2c_int_en(true);
  link_status = A2B_NODE_NOT_LINKED_ERROR;
  set_st_int((void*) &sc_slave);
  sc_slave.configure_com_buffers(slave_tx_buf, slave_rx_buf, slave_buf_size);
  slv_node_def.status = A2B_STAT_REG_RDIS;
}
```

Start with the initialization on the i2c interface with the interrupt disabled

Variable *link_status* initialized as *A2B_NODE_NOT_LINKED_ERROR*, so the first status is no connection

The method *configure_com_buffers* is used to set the buffers for the messages between master node and slave nodes

Lastly the status on the slave node definition is set with *A2B_STAT_REG_RDIS*, this will instruct the master node that a discovery round on the nodes is needed

### Cyclic

```cpp
void sc_slave_cyclic(){
  link_status = sc_slave.check_link();

  switch (link_status) {
    case A2B_NODE_NOT_LINKED_ERROR:

    break;
    case A2B_NODE_LINKED:

    break;
    case A2B_NODE_LINKED_WITH_ERR:

    break;
    default:
    break
  }
}
```

The link status is check regularly to assess the state of the SC connection
it is to be used to perform actions depending on the link status, like error detection and recovery.

### Node Example

```cpp
// initialize the node

A2B_Ssc_node slv_node_def = {
  1,
  1, #MIDAS
  {0x05, 0x00},
  16, #downslots4node
  0, #upslots4node
  0, #downslots_local
  16,#upslots_local
  AD242X_I2SGCFG_EARLY | AD242X_I2SGCFG_TDM16,
  false, #loopback
  false, #broadcast
  0, # power
  0, # status
  0, # control
  0, # key
  255,# buffers_size
  0,
  0,
  0,
  0,
  0,
  0,
  {0,0}
};

U8 slave_buf_size = 255;
U8 slave_tx_buf[255];
U8 slave_rx_buf[255];

// initialize i2c
Ci2c_com i2c_int = Ci2c_com(DN_I2C_INTERFACE,
                            DN_I2C_SPEED,
                            DN_I2C_GPIO,
                            DN_I2C_SCL_PIN,
                            DN_I2C_SDA_PIN,
                            DN_I2C_PRIO);

// construct the object
Csc_slave  sc_slave = Csc_slave(&slv_node_def, 1, 0xd0, &i2c);

void main(){

  sc_slave.reset();
  i2c.i2c_init();
  i2c.i2c_int_en(true);
  link_status = A2B_NODE_NOT_LINKED_ERROR;
  prev_link_status = A2B_NODE_NOT_LINKED_ERROR;
  set_st_int((void*) &sc_slave);
  sc_slave.configure_com_buffers(slave_tx_buf, slave_rx_buf, slave_buf_size);
  slv_node_def.status = A2B_STAT_REG_RDIS;

  while (1){
    sleep(100); # wait 100 milliseconds

    link_status = sc_slave.check_link();

    switch (link_status) {
      case A2B_NODE_NOT_LINKED_ERROR:
        #set status led
      break;
      case A2B_NODE_LINKED:
        #set status led

        #flag to power up system
        if ( sc_slave.check_ctl_reg_bit(A2B_CTL_REG_EN_PWR) ) {
          flags_ |= F_SC_POWER_UP;
        }
      break;
      case A2B_NODE_LINKED_WITH_ERR:
        #set status led
      break;
      default:

      break
    }
  }
}
```

In this example, we start by initializing the slave node with the options that the project needs, we then need to initialize the message buffers and the i2c port.

In the cyclic part we check every 100ms the state of the link and we do the steps according to the connection state. In the case of a slave Node we need to take care of the powering up of the system (as in phantom power mode the only peripherals turned on should be the microcontroller and the a2b chip). After the master node allows the system to be activated. here we can also check and take care of any connection issue that may need reconfiguration.
