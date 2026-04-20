/*
  SimpleMaster.ino - Example sketch that runs the StageConnect library as
  an A2B/StageConnect main/controlling device.
  Destination: Raspberry Pi Pico (or ESP) driving an AD242x evaluation board
  (e.g. AD2428MINI).

  The microcontroller acts as I2C master for the AD242x on the local bus.
  The A2B chip is configured as the master of the A2B link and runs the
  discovery round for any subordinated StageConnect devices connected via
  XLR/CAT cable.

  Used library: https://github.com/OpenMixerProject/StageConnect
  Licensed under GPL license
*/

#define USED_MICROCONTROLLER    1 // 0: MKR Vidor 4000 (SAMD21), 1: RPi Pico or ESP
#define PRINT_LINKSTATUS        1
#define SC_MESSAGE_BUFFER       32

#include <stdarg.h>
#include <Ticker.h>
#include <StageConnect.h>
#include <ci2c_com.h>

// StageConnect objects
Ci2c_com i2c_com;
// isMaster=true, a2bNodeId=1, master-I2C-address 0x68 (AD2428 default)
StageConnect stageConnect(true, 1, 0x68, &i2c_com);

// helper state
uint8_t secondCounter = 10;
int8_t lastLink = -127;
uint8_t rxMessage[SC_MESSAGE_BUFFER];

void printLinkStatus(int linkStatus) {
  switch (linkStatus) {
    case A2B_NODE_NOT_LINKED_ERROR:
      Serial.println("StageConnect Link-Status: ERROR - not Linked!");
      break;
    case A2B_NODE_LINKED:
      Serial.println("StageConnect Link-Status: GOOD - link established");
      break;
    case A2B_NODE_LINKED_WITH_ERR:
      Serial.println("StageConnect Link-Status: WARNING - Link with errors!");
      break;
    default:
      Serial.println("StageConnect Link-Status: UNKNOWN");
      break;
  }
}

void ticker100msFcn() {
  // drive the master state machine: discovery, chain configuration and
  // cyclic link/status exchange with every subordinated node
  stageConnect.update();

  int link = stageConnect.getLinkStatus();
  if (link != lastLink) {
    lastLink = link;
    #if PRINT_LINKSTATUS == 1
      printLinkStatus(link);
    #endif
  }

  secondCounter--;
  if (secondCounter == 0) {
    secondCounter = 10;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

    if (link == A2B_NODE_LINKED || link == A2B_NODE_LINKED_WITH_ERR) {
      int nodeCount = stageConnect.master()->get_num_active_nodes();
      Serial.println("Active subordinated nodes: " + String(nodeCount));

      // poll every discovered slave for pending messages
      for (uint8_t node_id = 0; node_id < (uint8_t)nodeCount; node_id++) {
        int n = stageConnect.master()->get_message(node_id, rxMessage, SC_MESSAGE_BUFFER);
        if (n > 0) {
          for (int i = 0; i < n; i++) {
            if ((rxMessage[i] < 0x21) || (rxMessage[i] > 0x7E)) rxMessage[i] = ' ';
          }
          rxMessage[n < SC_MESSAGE_BUFFER ? n : SC_MESSAGE_BUFFER - 1] = 0;
          Serial.println("Message from node " + String(node_id) + ": " + String((char*)rxMessage));
        }
      }

      // broadcast a ping to node 0 once per second (if present)
      if (nodeCount > 0) {
        uint8_t msg[4] = {'P', 'I', 'N', 'G'};
        stageConnect.master()->send_message(0, msg, sizeof(msg));
      }
    }
  }
}

#if USED_MICROCONTROLLER == 0
  Ticker ticker100ms(ticker100msFcn, 100, 0, MILLIS);
#elif USED_MICROCONTROLLER == 1
  Ticker ticker100ms;
#endif

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  delay(1000);
  Serial.println("StageConnect Master Demo");
  delay(500);

  // Wire acts as I2C master on the local bus to talk to the AD242x
  Wire.setSDA(4);
  Wire.setSCL(5);
  Wire.begin();

  Serial.println("Detected A2B-Chip: " + stageConnect.getChipInfo());

  // configure the master - use brand IDs >= 0x80 for DIY projects.
  // inputChannelCount = slots the master consumes from upstream (slaves->master)
  // outputChannelCount = slots the master sends downstream (master->slaves)
  stageConnect.config(0x80, 0x0001, 16, 16,
                      AD242X_I2SGCFG_EARLY | AD242X_I2SGCFG_TDM16,
                      SC_MESSAGE_BUFFER);

  Serial.println("Starting timer...");
  #if USED_MICROCONTROLLER == 0
    ticker100ms.start();
  #elif USED_MICROCONTROLLER == 1
    ticker100ms.attach_ms(100, ticker100msFcn);
  #endif
  Serial.println("Master ready. Waiting for subordinated devices...");
}

void loop() {
  #if USED_MICROCONTROLLER == 0
    ticker100ms.update();
  #endif
}
