/*
  SimpleSlave.ino - Example-Sketch to test the StageConnect-library
  Destination: Raspberry Pi Pico
  A2B-Master: Behringer WING

  Used library: https://github.com/xn--nding-jua/StageConnect

  Written by Chris Noeding
  Licensed under GPL license
*/

#define USED_MICROCONTROLLER    1 // 0: MKR Vidor 4000 (SAMD21), 1: RPi Pico or ESP
#define PRINT_LINKSTATUS        0
#define PRINT_RXTX_MESSAGES     0
#define SC_MESSAGE_BUFFER       32

#if PRINT_RXTX_MESSAGES == 1
  // reduce message buffer as a larger buffer combined with UART_PRINTING will fail
  #define SC_MESSAGE_BUFFER 8
#endif

// includes for Arduino-Framework
#include <stdarg.h>

// includes for libraries
#include <Ticker.h>
#include <StageConnect.h>
#include <ci2c_com.h>

// StageConnect objects
Ci2c_com i2c_com; // I2C-Wrapper-Class for Arduino-Library
StageConnect stageConnect(false, 1, 0xD0, &i2c_com); // isMaster, a2bNodeId, 8-bit-I2C-Address of Master, Pointer to I2C-Object

// buffers for StageConnect
uint8_t scTxBuf[SC_MESSAGE_BUFFER];
uint8_t scRxbuf[SC_MESSAGE_BUFFER];
uint8_t rxMessage[SC_MESSAGE_BUFFER];

// some helping variables
uint8_t secondCounter = 10;
uint8_t newDiscovery = 10;

void ticker100msFcn() {
  stageConnect.update();

  if (stageConnect.checkForNewMessage() > 0) {
    if (stageConnect.slave()->get_message(&rxMessage[0], SC_MESSAGE_BUFFER) > 0) {
      // replace non-ASCII-chars with ' '
      for (uint8_t i=0; i<SC_MESSAGE_BUFFER; i++) {
        if ((rxMessage[i] < 0x21) || (rxMessage[i] > 0x7E)) {
          rxMessage[i] = ' ';
        }
      }
      Serial.println("Received Message from WING: " + String((char*)rxMessage));
    }
  }

  secondCounter--;
  if (secondCounter == 0) {
    secondCounter = 10;

    // toggle LED each second
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

/*
    if (newDiscovery > 0) {
      newDiscovery--;

      if (newDiscovery == 0) {
        Serial.println("=================== TEST ================");
        uint8_t buf[3] = {'*', 'E', '#'}; // request/start a new discovery round
        stageConnect.slave()->send_message(buf, 3);
      }
    }
*/

    switch (stageConnect.getLinkStatus()) {
      case A2B_NODE_NOT_LINKED_ERROR:
        // TODO: RED LED ON
        #if PRINT_LINKSTATUS == 1
          Serial.println("StageConnect Link-Status: ERROR - not Linked!");
        #endif
        break;
      case A2B_NODE_LINKED:
        // TODO: GREEN LED ON
        #if PRINT_LINKSTATUS == 1
          Serial.println("StageConnect Link-Status: GOOD - link established");
        #endif

        // #flag to power up system
        if (stageConnect.slave()->check_ctl_reg_bit(A2B_CTL_REG_EN_PWR)) {
          // enable device-power
        }
        break;
      case A2B_NODE_LINKED_WITH_ERR:
        // TODO: ORANGE LED ON
        #if PRINT_LINKSTATUS == 1
          Serial.println("StageConnect Link-Status: WARNING - Link with errors!");
        #endif
        break;
      default:
        // TODO: RED LED ON
        #if PRINT_LINKSTATUS == 1
          Serial.println("StageConnect Link-Status: UNKNOWN - Linkstatus is unclear!");
        #endif
        break;
    }
  }
}
#if USED_MICROCONTROLLER == 0
  Ticker ticker100ms(ticker100msFcn, 100, 0, MILLIS); // Ticker for SAMD21
#elif USED_MICROCONTROLLER == 1
  Ticker ticker100ms;
#endif

void I2C_RxHandler(int numBytes) {
  uint8_t counter = 0;
  uint8_t rxByte = 0;

  if (Wire.available() == 1) {
    // we received an address for next read
    rxByte = Wire.read();
    stageConnect.slave()->set_reg_add(rxByte);
    #if PRINT_RXTX_MESSAGES >= 1
      Serial.println("WING sent byte " + String(counter) + " 0x" + String(rxByte, HEX));
    #endif
  }else if (Wire.available() > 1) {
    #if PRINT_RXTX_MESSAGES >= 1
      Serial.println("WING sent us " + String(Wire.available()) + " bytes");
    #endif
    while(Wire.available()) {  // Read Any Received Data
      rxByte = Wire.read();

      if (counter == 0) {
        // set this as read-address for write
        stageConnect.slave()->set_reg_add(rxByte);
      }else{
        // counter is above 0 -> write into register with auto-incrementing address-pointer
        stageConnect.slave()->write_reg(rxByte);
      }

      #if PRINT_RXTX_MESSAGES >= 1
        Serial.println("WING sent byte " + String(counter) + " 0x" + String(rxByte, HEX));
      #endif
      counter++;
    }
  }


/*
  while(Wire.available()) {  // Read Any Received Data
    stageConnect.slave()->set_reg_add(Wire.read());
  }
*/
}

void I2C_TxHandler(void)
{
  uint8_t txByte = stageConnect.slave()->read_reg();
  #if PRINT_RXTX_MESSAGES >= 1
    Serial.println("We sent byte 0x" + String(txByte, HEX));
  #endif
  Wire.write(txByte);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  delay(1000);
  Serial.println("StageConnect Demo system");
  delay(1000);

  Serial.println("Try to detect A2B-Chip...");
  Wire.begin(); // init I2C as master temporarily
  Serial.println("Detected A2B-Chip: " + stageConnect.getChipInfo());
  Wire.end();

  Serial.println("Initializing I2C as Slave...");
  stageConnect.config(0x00, 0x0003, 16, 16, AD242X_I2SGCFG_EARLY | AD242X_I2SGCFG_TDM16, SC_MESSAGE_BUFFER); // configure as Behringer P24 with 16 input channels and 16 output channels
  //stageConnect.config(0x00, 0x0080, 0, 32, AD242X_I2SGCFG_EARLY | AD242X_I2SGCFG_TDM16, SC_MESSAGE_BUFFER); // configure as Unknown Device with 0 input channels and 32 output channels
  //stageConnect.config(0x00, 0x0080, 32, 0, AD242X_I2SGCFG_EARLY | AD242X_I2SGCFG_TDM16, SC_MESSAGE_BUFFER); // configure as Unknown Device with 32 input channels and 0 output channels
  stageConnect.slave()->configure_com_buffers(scTxBuf, scRxbuf, SC_MESSAGE_BUFFER);
  stageConnect.slave()->set_status_reg_bit(A2B_STAT_REG_RDIS);
  Wire.begin(0x3D); // I2C-Address to be detected by Behringer WING
  Wire.onReceive(I2C_RxHandler);
  Wire.onRequest(I2C_TxHandler);

  Serial.println("Starting timer...");
  #if USED_MICROCONTROLLER == 0
    ticker100ms.start();
  #elif USED_MICROCONTROLLER == 1
    ticker100ms.attach_ms(100, ticker100msFcn);
  #endif
  Serial.println("System ready.");
}

void loop() {
  #if USED_MICROCONTROLLER == 0
    ticker100ms.update(); 
  #elif USED_MICROCONTROLLER == 1
  #endif
}
