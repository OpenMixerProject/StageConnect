/*
  Example-Sketch to test the StageConnect-library
  Destination: Arduino MKR Vidor 4000 (SAMD21)

  Used library: https://github.com/xn--nding-jua/StageConnect
*/

// includes for Arduino-Framework
#include "Ticker.h"
#include <stdarg.h>

// includes for libraries
#include <StageConnect.h>
#include <ci2c_com.h>

// StageConnect objects
Ci2c_com i2c_com; // I2C-Wrapper-Class for Arduino-Library
StageConnect stageConnect(false, 1, 0xD0, &i2c_com); // isMaster, a2bNodeId, 8-bit-I2C-Address of Master, Pointer to I2C-Object

// buffers for StageConnect
uint8_t scBufSize = 64;
uint8_t scTxBuf[64];
uint8_t scRxbuf[64];

// some helping variables
uint8_t secondCounter = 10;
uint8_t newDiscovery = 10;

void ticker100msFcn() {
  stageConnect.update();

  secondCounter--;
  if (secondCounter == 0) {
    secondCounter = 10;

    if (newDiscovery > 0) {
      newDiscovery--;

      if (newDiscovery == 0) {
        Serial.println("=================== TEST ================");
        uint8_t buf[3] = {'*', 'E', '#'}; // request/start a new discovery round
        stageConnect.slave()->send_message(buf, 3);
      }
    }
 
    switch (stageConnect.getLinkStatus()) {
      case A2B_NODE_NOT_LINKED_ERROR:
        // TODO: RED LED ON
        Serial.println("StageConnect Link-Status: ERROR - not Linked!");
        break;
      case A2B_NODE_LINKED:
        // TODO: GREEN LED ON
        Serial.println("StageConnect Link-Status: GOOD - link established");

        // #flag to power up system
        if (stageConnect.slave()->check_ctl_reg_bit(A2B_CTL_REG_EN_PWR)) {
          // enable device-power
        }
        break;
      case A2B_NODE_LINKED_WITH_ERR:
        // TODO: ORANGE LED ON
        Serial.println("StageConnect Link-Status: WARNING - Link with errors!");
        break;
      default:
        // TODO: RED LED ON
        Serial.println("StageConnect Link-Status: UNKNOWN - Linkstatus is unclear!");
        break;
    }
  }
}
Ticker ticker100ms(ticker100msFcn, 100, 0, MILLIS);

void I2C_RxHandler(int numBytes) {
  uint8_t counter = 0;
  uint8_t rxByte = 0;

  if (Wire.available() == 1) {
    // we received an address for next read
    rxByte = Wire.read();
    stageConnect.slave()->set_reg_add(rxByte);
    Serial.println("WING sent byte " + String(counter) + " 0x" + String(rxByte, HEX));
  }else{
    while(Wire.available()) {  // Read Any Received Data
      rxByte = Wire.read();

      if (counter == 0) {
        // set this as read-address for write
        stageConnect.slave()->set_reg_add(rxByte);
      }else{
        // counter is above 0 -> write into register with auto-incrementing address-pointer
        stageConnect.slave()->write_reg(rxByte);
      }

      Serial.println("WING sent byte " + String(counter) + " 0x" + String(rxByte, HEX));
      counter++;
    }
  }
}

void I2C_TxHandler(void)
{
  uint8_t txByte = stageConnect.slave()->read_reg();
  Serial.println("We sent byte 0x" + String(txByte, HEX));
  Wire.write(txByte);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Try to detect A2B-Chip...");
  Wire.begin(); // init I2C as master temporarily
  Serial.println("Detected A2B-Chip: " + stageConnect.getChipInfo());
  Wire.end();

  Serial.println("Initializing I2C as Slave...");
  stageConnect.config(0x00, 0x0002, 16, 16, AD242X_I2SGCFG_EARLY | AD242X_I2SGCFG_TDM16, scBufSize); // configure as Behringer P24 with 16 channels in both directions
  stageConnect.slave()->configure_com_buffers(scTxBuf, scRxbuf, scBufSize);
  stageConnect.slave()->set_status_reg_bit(A2B_STAT_REG_RDIS);
  Wire.begin(0x3D); // I2C-Address to be detected by Behringer WING
  Wire.onReceive(I2C_RxHandler);
  Wire.onRequest(I2C_TxHandler);

  Serial.println("Starting timer...");
  ticker100ms.start();
  Serial.println("System ready.");
}

void loop() {
  ticker100ms.update();
}
