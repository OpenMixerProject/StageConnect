#include "stcon.h"
#include <Wire.h>
#include "types.h"
#include <stdio.h>
#include "stcon_user.h"
#include <Arduino.h>

void serial_printf(HardwareSerial& serial, const char* fmt, ...) { 
    va_list argv;
    va_start(argv, fmt);

    for (int i = 0; fmt[i] != '\0'; i++) {
        if (fmt[i] == '%') {
            // Look for specification of number of decimal places
            int places = 2;
            if (fmt[i+1] == '.') i++;  // alw1746: Allows %.4f precision like in stdio printf (%4f will still work).
            if (fmt[i+1] >= '0' && fmt[i+1] <= '9') {
                places = fmt[i+1] - '0';
                i++;
            }
            
            switch (fmt[++i]) {
                case 'B':
                    serial.print("0b"); // Fall through intended
                case 'b':
                    serial.print(va_arg(argv, int), BIN);
                    break;
                case 'c': 
                    serial.print((char) va_arg(argv, int));
                    break;
                case 'd': 
                case 'i':
                    serial.print(va_arg(argv, int), DEC);
                    break;
                case 'f': 
                    serial.print(va_arg(argv, double), places);
                    break;
                case 'l': 
                    serial.print(va_arg(argv, long), DEC);
                    break;
                case 'o':
                    serial.print(va_arg(argv, int) == 0 ? "off" : "on");
                    break;
                case 's': 
                    serial.print(va_arg(argv, const char*));
                    break;
                case 'X':
                    serial.print("0x"); // Fall through intended
                case 'x':
                    serial.print(va_arg(argv, int), HEX);
                    break;
                case '%': 
                    serial.print(fmt[i]);
                    break;
                default:
                    serial.print("?");
                    break;
            }
        } else {
            serial.print(fmt[i]);
        }
    }
    va_end(argv);
}


A2B_Ssc_node slv_node_def = {
  1,
  1, //#MIDAS
  {0x05, 0x00},
  16, //#downslots4node
  0, //#upslots4node
  0, //#downslots_local
  16,//#upslots_local
  AD242X_I2SGCFG_EARLY | AD242X_I2SGCFG_TDM16,
  false, //#loopback
  false, //#broadcast
  0, //# power
  0, //# status
  0, //# control
  0, //# key
  255,//# buffers_size
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



 // construct the object
Csc_slave  sc_slave = Csc_slave(&slv_node_def, 1, 0xd0); 

void I2C_RxHandler(int numBytes) {
  while(Wire.available()) {  // Read Any Received Data
    sc_slave.set_reg_add(Wire.read());
  }
  
}

void I2C_TxHandler(void)
{
  Wire.write(sc_slave.read_reg());
}

void setup() {

  //i2c default pins ->  (SCL=GPIO5 ; SDA =GPIO4).
  //Wire.begin(); 
  
  Serial.begin(115200);
  
  while (1) {
  
  Serial.println("Apply Reset");
  A2B_DEBUG_A2B("test message");
  //sc_slave.reset();

  Wire.begin(0x3d); //7a or 3d
  Wire.onReceive(I2C_RxHandler);
  Wire.onRequest(I2C_TxHandler);
  S8 link_status = A2B_NODE_NOT_LINKED_ERROR;
  S8 prev_link_status = A2B_NODE_NOT_LINKED_ERROR;

  //set_st_int((void*) &sc_slave);
  sc_slave.configure_com_buffers(slave_tx_buf, slave_rx_buf, slave_buf_size);
  slv_node_def.status = A2B_STAT_REG_RDIS;

  while (1){

    delay(100); 

    //sc_slave.event_handler();
    link_status = sc_slave.check_link();

    
    switch (link_status) {
      case A2B_NODE_NOT_LINKED_ERROR:
        //#set status led
      break;
      case A2B_NODE_LINKED:
       // #set status led

       // #flag to power up system
        if ( sc_slave.check_ctl_reg_bit(A2B_CTL_REG_EN_PWR) ) {
          
        }
        break;
      
      case A2B_NODE_LINKED_WITH_ERR:
       // #set status led
      break;
      default:

      break;
    }
    if (Serial.available()>0) {
      (void) Serial.read();
      Serial.println("Restart");
      break;
    }
  }
 }
}

void loop() {
  // put your main code here, to run repeatedly:

}
