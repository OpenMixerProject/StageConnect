# Stage Connect API

*   Allows the transmission of audio and control data over a twisted pair.
*   Support for 48 kHz and 44,1 kHz
*   Audio channels are 24 bit
*   Up to 32 audio channels in total
*   The 32 audio slots can be configured as required.
*   Audio is handled in standard, left justified, I2S/TMD format. MSB one clock cycle after FS
*   Data communication and control over I2C
*   I2C extension to nodes
*   Mailbox
*   User define RX and TX buffer for up to 256 bytes of customized data.

## Audio distribution (24 bit min.)

*   The number of down stream and up streams cannot be greater than 32!
*   The 32 audio slots can be configured as required, e.g. (down X up)
    *   32 x 0
    *   0 x 32
    *   24 x 8
    *   8 x 24
    *   16 x 16
    *   8 x 8
    *   4 x 4
    *   2 x 2
*   and any other combination as well
*   The total number of downstream/upstream slots is defined on the master
*   The total number of downstream/upstream slots transmitted by a node must be the same as the master defined
*   A node can consume and provide audio slots to/from the master node.
*   The max number of nodes in a chain is 11

Important limitation: the number of nodes is also limited by the number of audio slots transmitted, see _StageConnectCalculator_

## I2S and TDM format

*   In a Master node the BCLK and FS signals are inputs and provided by the host controller
*   In a slave node the BCLK and FS signals are outputs
    *   Local audio elements need to run synchronous to the A2B audio clock or
    *   Use a local sample rate converter when local audio clock is preferred
*   The local I2S/TDM configuration can differ from master. e.g.
    *   master works on TDM8 and provides 16 audio channels on the downstream
    *   master works on TDM8 and expects 16 audio channels on the upstream
    *   case 1: slave works on TDM8, consumes/provides all 16 audio slots and uses a two TX/RX each to handle data
    *   case 2: slave works on TDM16, consumes/provides all 16 audio slots and uses a single TX/RX each to handle data
    *   and other combinations with some restrictions.

## I2C

*   Local host configure master and slave node via I2C interface
*   Local host can remotely communicate with peripheries on the node via I2C
*   Host on slave nodes can communicate with master via Mailbox

## Functionality

*   By default all A2B ICs are configured as slave node and wait for a bus connection on port A.
    *   If a valid link is detected it synchronizes its PLL to the audio clock and provides the TDM signals
    *   Nevertheless the TDM outputs need to be enabled, either locally or remotely by the master node
*   Any node can works as master (AD2425), its need to be configured as one. Once configured the IC expects a valid FS clock (48kHz or 44,1kHz). It synchronizes its PLL to the clock and is ready for node searching.
*   Slave nodes need to be &quot;discovered&quot; by the master, one by one.

## Other characteristics

Line diagnostics
*   CRC error detection in both directions
*   Frame error detection
*   I2S/TDM loop for debugging
*   Pseudo random data sending for line testing

## Daisy chaining

*   More than one node can be connected to a master as daisy chain
*   All audio transfers, upstream and downstream, are between the master node and every individual node
*   Audio transfer between nodes is not supported

## SC Node

### Structure Definition
Structure definition and description of its members you will find [here](SC%20Node.md)

### Master Operation

The [_sc\_main\_cyclic_](Master%20Node.md#csc_mastersc_main_cyclicbool-reset) method needs to be called at least every 100ms. The master node is set on discovery mode and will repeatably search for nodes in the chain until at least one node is found. Once a node is found it will search for more node in the chain.

The master retrieves the node descriptor from every node, it gathers information about the individual power requirement, downstream audio slots, upstream audio slots, etc.

The details on Master Node class definition and description you will find [here](Master%20Node.md#master-node)

### Slave Operation

The descriptor structure A2B\_Ssc\_node provides info about the device. It is used by the master to configure a SC chain
The link information is provided by the [_check\_link_](Slave%20Node.md#csc_slavecheck_link) method. It should be called at least every 100ms.
*   Connected should be displayed as green
*   No connected or errors should be displayed as red
*   Connection with errors should be displayed as yellow

The details on Slave Node class definition and description you will find [here](Slave%20Node.md#slave-node)

### Audio distribution

*   The audio slots are allocated for the downstream first.
*   If audio slots are still available, it will allocated them for the upstream.
*   The upstream allocation starts with the first node and continues until the audio slots are out.
*   If no more upstream audio slots are available the next node upstream configuration is set to zero
*   It is important to note that the order of the upstream audio slots is swapped. I.e. the first audio slots belong to the last node. The SC API provides information about this, e.g. the WING uses it to correct the audio order.

### Power distribution

*   The power requirement is added and accumulated with every node. Max. power is 18W
*   If the required power by a node can be allocated, the power on bit is set on the control register.
*   If no more poser is available the slave node is not allowed to turn fully on.

### Working with Phantom Power

*   The device should operate at minimum power during the enumeration, only the A2B IC and the MCU should be operating.
*   Once the power enable bit is set, the device can turn on the rest of the system. The info is provided by _check\_ctl\_reg\_bit_ method.
*   The device should provide a slow turn on ramp. Any fast or rush current would trip the Efuse in the master and turn off the power for all the chain.