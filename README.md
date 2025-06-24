# StageConnect
This repository contains an Arduino-Library to create a StageConnect device (subordinated node-device as well as controlling main-device) using AnalogDevices AD242x-ICs like the AD2428.

StageConnect is based on the automotive A²B and is used to connect the Behringer WING to the Midas DP48, for instance. Analog Devices [A²B audio](https://www.analog.com/en/solutions/a2b-audio-bus.html) bus is used as the technology basis and uses simple XLR connectors between two devices to transmit 32 channels of uncompressed 48kHz 32bit audio.

A StageConnect/A2B main-device uses a virtual I2C-connection through the A2B-link that is routed via the AD242x-IC to the Arduino microcontroller.

## Usage

TBD

## Technical information

TBD

## Credits

Many thanks to MusicTribe/Behringer for sharing detailed information about the used protocol and for the permission to share the code publicly.