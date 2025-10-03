# StageConnect
This repository contains an Arduino-Library to create a StageConnect device (subordinated node-device as well as controlling main-device) using AnalogDevices AD242x-ICs like the AD2428.

StageConnect is based on the automotive A²B and is used to connect the Behringer WING to the Midas DP48, for instance. Analog Devices [A²B audio](https://www.analog.com/en/solutions/a2b-audio-bus.html) bus is used as the technology basis and uses simple XLR connectors between two devices to transmit 32 channels of uncompressed 48kHz 32bit audio.

A StageConnect/A2B main-device uses a virtual I2C-connection through the A2B-link that is routed via the AD242x-IC to the Arduino microcontroller.

## Usage

Include the main-library as well as the I2C-Wrapper-Class, instantiate both classes and setup the Wire-library:

    #include <StageConnect.h>
    #include <ci2c_com.h>

    Ci2c_com i2c_com;
    StageConnect stageConnect(false, 1, 0xD0, &i2c_com);

    void I2C_RxHandler(int numBytes) {...}
	void I2C_TxHandler(void) {...}
	
	void setup() {
	    Wire.begin(0x3D);
		Wire.onReceive(I2C_RxHandler);
		Wire.onRequest(I2C_TxHandler);
	}

stageConnect.update() should be then called every 100ms.

Have a look into the example-sketch to learn how to use the callbacks and the mailbox-system to receive channel-names from the host-device.

For electrical connection have a look at the following picture showing the connection with the AD2428MINI evaluation board:
![alt text](Documentation/connectiong.jpg)

## Technical information

Analog Devices suggests cable-length of up to 15 meters, while Behringer allows longer cables. The filter-network of the AD242x-chips is quite tricky and needs caution on adjusting the hardware-parameters. Use the schematics of the AD2428MINI as a reference.

Read the files in the folder "Documentation" for more information about the configuration-options and technical details regarding the brand- and product-ID as well as the specific commands for the mailbox-system.

Please use brand-IDs above 0x80 to prevent interference with other products.

## Credits

Many thanks to MusicTribe/Behringer for sharing detailed information about the used protocol and for the permission to share the code publicly.