#pragma once
#include "types.h"

//Include file for the register mapping of A2B 2425 traceivers


//I2C Chip Address Register (Slave Only)
//Description:It is used during I2C transactions to address a periph-eral device.
//Used for remote I2C communications. The A2B slave node acts as the I 2 C master
//in I 2 C transactions with peripherals.

#define AD242X_CHIP_REG		(0x00)
#define AD242X_CHIP_MSK		(0x7f)

//Node Address Register (Master Only)
//Description:The  register provides control bits for addressing
//slave nodes through the A2B bus.

#define AD242X_NODEADR_REG		(0x01)
#define AD242X_NODEADR_BRCST	(1 << 7)	//Broadcast enable, disable PERI for use
#define AD242X_NODEADR_PERI		(1 << 5)	//Remote peripheral acces enable, disable BRCST for use
#define AD242X_NODEADR_NONE_MSK	(0x0f)
#define AD242X_NODEADR_NONE_POS	(0)

//Vendor ID Register
//Description:The AD242X_VENDOR register identifies the part as manufactured by
//Analog Devices. Reset value is 0xAD

#define AD242X_VENDOR_REG		(0x02)

//Product ID Register
//Description:The AD242X_PRODUCT register identifies the last two digits of the part number.
//Reset value is 0x25

#define AD242X_PRODUCT_REG		(0x03)

//Version ID Register
//Description:The AD242X_VERSION register identifies the version of the part.

#define AD242X_VERSION_REG		(0x04)

//Capability ID Register
//Description:The AD242X_CAPABILITY register identifies available control interfaces.

#define AD242X_CAPABILITY_REG		(0x05)

//Switch Control Register
//Description:The AD242X_SWCTL register controls switching of phantom power onto the
//downstream B-side of the A2B bus.

#define AD242X_SWCTL_REG		(0x09)
#define AD242X_SWCTL_ENSW		(1 << 0)
#define AD242X_SWCTL_DIAGMODE	(1 << 3)
#define AD242X_SWCTL_MODE_POS	(4)

//Broadcast Downstream Slots Register (Slave Only)
//Description:In a slave node, the AD242X_BCDNSLOTS register defines the number of
//data slots which are captured by the node and also passed downstream (B-side) as
//broadcast data to the next node.

#define AD242X_BCDNSLOTS_REG		(0x0A)
#define AD242X_BCDNSLOTS_MSK	(0x3f)
#define AD242X_BCDNSLOTS_POS	(0)


//Local Downstream Slots Register (Slave Only)
//Description:the AD242X_LDNSLOTS register defines the number of data slots which
//are captured by the local node during the downstream portion of the superframe.

#define AD242X_LDNSLOTS_REG			(0x0B)
#define AD242X_LDNSLOTS_DNMASKEN	(1 << 7)		//Downstream Broadcast Mask Enable.
#define AD242X_LDNSLOTS_MSK			(0x3f)
#define AD242X_LDNSLOTS_POS		(0)

//Local Upstream Slots Register (Slave Only)
//Description:In a slave node, the AD242X_LUPSLOTS register defines the number
//of data slots which are added by the local node during the upstream portion
//of the superframe.

#define AD242X_LUPSLOTS_REG		(0x0C)
#define AD242X_LUPSLOTS_MSK		(0x3f)
#define AD242X_LUPSLOTS_POS		(0)

//Downstream Slots Register
//Description:In a slave node, the AD242X_DNSLOTS register defines the number
//of data slots (broadcast slots not included)which are passed downstream (B-side)
//after the local node begins to capture data slots.

#define AD242X_DNSLOTS_REG		(0x0D)
#define AD242X_DNSLOTS_MSK		(0x3f)
#define AD242X_DNSLOTS_POS		(0)

//Upstream Slots Register
//Description:In a slave node, the AD242X_UPSLOTS register defines the number
//of data slots which are passed upstream (B-side) before the local node begins
//to add data slots. In the master node, the AD242X_UPSLOTS register defines the
//total number of upstream data slots.

#define AD242X_UPSLOTS_REG		(0x0E)
#define AD242X_UPSLOTS_MSK		(0x3f)
#define AD242X_UPSLOTS_POS		(0)

//Response Cycles Register
//Description:The AD242X_RESPCYCS register defines the time between the start
//of the downstream header (first sync preamble bit) and the start of the u
//pstream header (first sync preamble bit).

#define AD242X_RESPCYCS_REG		(0x0F)

//Slot Format Register (Master Only, Auto-Brcst)
//Description:The AD242X_SLOTFMT register defines the size and format of the
//downstream and upstream data slots.

#define AD242X_SLOTFMT_REG			(0x10)
#define AD242X_SLOTFMT_UPFMT		(1 << 7)	//Upstream Floating Point.
#define AD242X_SLOTFMT_UPSIZE_8		(0 << 4)	//Upstream Slot Size.
#define AD242X_SLOTFMT_UPSIZE_12	(1 << 4)	//Upstream Slot Size. bits
#define AD242X_SLOTFMT_UPSIZE_16	(2 << 4)	//Upstream Slot Size. bits
#define AD242X_SLOTFMT_UPSIZE_20	(3 << 4)	//Upstream Slot Size. bits
#define AD242X_SLOTFMT_UPSIZE_24	(4 << 4)	//Upstream Slot Size. bits
#define AD242X_SLOTFMT_UPSIZE_28	(5 << 4)	//Upstream Slot Size. bits
#define AD242X_SLOTFMT_UPSIZE_32	(6 << 4)	//Upstream Slot Size. bits
#define AD242X_SLOTFMT_DNFMT		(1 << 3)	//Upstream Floating Point.
#define AD242X_SLOTFMT_DNSIZE_8		(0 << 0)	//Upstream Slot Size.
#define AD242X_SLOTFMT_DNSIZE_12	(1 << 0)	//Upstream Slot Size. bits
#define AD242X_SLOTFMT_DNSIZE_16	(2 << 0)	//Upstream Slot Size. bits
#define AD242X_SLOTFMT_DNSIZE_20	(3 << 0)	//Upstream Slot Size. bits
#define AD242X_SLOTFMT_DNSIZE_24	(4 << 0)	//Upstream Slot Size. bits
#define AD242X_SLOTFMT_DNSIZE_28	(5 << 0)	//Upstream Slot Size. bits
#define AD242X_SLOTFMT_DNSIZE_32	(6 << 0)	//Upstream Slot Size. bits



//Data Control Register (Master Only, Auto-Brcst)
//Description:The AD242X_DATCTL register is used to enable data slots and
//standby mode on the A2B bus.

#define AD242X_DATCTL_REG		(0x11)
#define AD242X_DATCTL_STANDBY	(1 << 7)	//Enable Standby Mode.
#define AD242X_DATCTL_ENDSNIFF	(1 << 5)	//Enable Data Output on Bus Monitor Node.
#define AD242X_DATCTL_UPS 		(1 << 1)	//Enable Upstream Slots.
#define AD242X_DATCTL_DNS 		(1 << 0)	//Enable Downstream Slots.

//Control Register
//Description:The AD242X_CONTROL register provides bits which control nodes on the bus.

#define AD242X_CONTROL_REG		(0x12)
#define AD242X_CONTROL_MSTR		(1 << 7)	//Master Node.
#define AD242X_CONTROL_SOFTRST	(1 << 2)	//Soft Reset of Protocol Engine.
#define AD242X_CONTROL_ENDDSC	(1 << 1)	//End Discovery Mode.
#define AD242X_CONTROL_NEWSTRCT	(1 << 0)	//Enable New Structure

//Discovery Register (Master Only)
//Description:Programming the AD242X_DISCVRY register with a response cycle value for
//a new node to be added allows the new node to be discovered. It triggers the start
//of full discovery frames being sent to the next node.

#define AD242X_DISCVRY_REG		(0x13)

//Switch Status Register
//Description:The AD242X_SWSTAT register provides line diagnostics status information.

#define AD242X_SWSTAT_REG				(0x14)
#define AD242X_SWSTAT_FAULT_NLOC		(1 << 7)	//Cable Fault Not Localized.
#define AD242X_SWSTAT_FAULT_CODE_MSK	(0x70)
#define AD242X_SWSTAT_FAULT_CODE_POS	(4)
#define AD242X_SWSTAT_FAULT_FAULT		(1 << 1)	//Cable Fault.
#define AD242X_SWSTAT_FAULT_FIN			(1 << 0)	//Switch Activation Complete.


//Interrupt Status Register
//Description:The AD242X_INTSTAT register contains interrupt status information for the node.

#define AD242X_INTSTAT_REG		(0x15)
#define AD242X_INTSTAT_IRQ		((1 << 0)	//Interrupt Currently Asserted.

//Interrupt Source Register (Master Only)
//Description:The AD242X_INTSRC register contains information about the current
//highest priority interrupt.

#define AD242X_INTSRC_REG			(0x16)
#define AD242X_INTSRC_MSTINT		(1 << 7)	//Master Interrupt.
#define AD242X_INTSRC_SLVINT		(1 << 6)	//Slave Interrupt.
#define AD242X_INTSRC_INODE_MSK		(0x0f)		//ID for SLVINT.
#define AD242X_INTSRC_INODE_POS		(0)			//ID for SLVINT.

//Interrupt Type Register (Master Only)
//Description:The AD242X_INTTYPE register contains information about the pending
//interrupt generated by the node indicated in the AD242X_INTSRC register and
//signaled with the IRQ pin.

#define AD242X_INTTYPE_REG		(0x17)
#define AD2425_INTTYPE_HDCNTERR 0
#define AD2425_INTTYPE_DDERR	1
#define AD2425_INTTYPE_CRCERR 	2
#define AD2425_INTTYPE_DPERR 	3
#define AD2425_INTTYPE_BECOVF 	4
#define AD2425_INTTYPE_SRFERR 	5
#define AD2425_INTTYPE_SRFCRCERR 6	// Slave Only
#define AD2425_INTTYPE_PWRERR0 	 9	// Cable Terminal Shorted to Ground
#define AD2425_INTTYPE_PWRERR1 	10	// Cable Terminal Shorted to VBat
#define AD2425_INTTYPE_PWRERR2 	11	// Cable Terminals Shorted Together
#define AD2425_INTTYPE_PWRERR3 	12	// Cable Disconnected or Open Circuit
#define AD2425_INTTYPE_PWRERR4 	13	// Cable is Reverse Connected
#define AD2425_INTTYPE_PWRERR5 	15	// Indeterminate Fault
#define AD2425_INTTYPE_IO0PND 	16	// Slave Only
#define AD2425_INTTYPE_IO1PND 	17	// 
#define AD2425_INTTYPE_IO2PND 	18	// 
#define AD2425_INTTYPE_IO3PND 	19	// 
#define AD2425_INTTYPE_IO4PND 	20	// 
#define AD2425_INTTYPE_IO5PND 	21	// 
#define AD2425_INTTYPE_IO6PND 	22	// 
#define AD2425_INTTYPE_IO7PND 	23	// 
#define AD2425_INTTYPE_DSCDONE 	24	// Master Only
#define AD2425_INTTYPE_I2CERR  	25	// Master Only
#define AD2425_INTTYPE_ICRCERR 	26	// Master Only
#define AD2425_INTTYPE_PWRERR6 	41	// Non-Localized Short to Ground
#define AD2425_INTTYPE_PWRERR7 	42	// on-Localized Short to VBat
#define AD2425_INTTYPE_MB0_FULL 	48	// 
#define AD2425_INTTYPE_MB0_EMPTY 	49	// 
#define AD2425_INTTYPE_MB1_FULL 	50	// 
#define AD2425_INTTYPE_MB1_EMPTY 	51	// 
#define AD2425_INTTYPE_Interrupt_Messaging_error    128             //  0x80
#define AD2425_INTTYPE_Slave_INTTYPE_Read_Error	253	// Master Only
#define AD2425_INTTYPE_Standby_Done 	254	// Master Only
#define AD2425_INTTYPE_MSTR_RUNNING 	255	// Master Only

//Interrupt Pending 0 Register
//Description:The AD242X_INTPND0 register contains interrupt pending bits for the node.

#define AD242X_INTPND0_REG			(0x18)
#define AD242X_INTPND0_SRFCRCERR	(1 << 7)	//SRF CRC Error (Slave Only).
#define AD242X_INTPND0_SRFERR		(1 << 6)	//Missed Synchronization Response Frame (SRF).
#define AD242X_INTPND0_BECOVF		(1 << 5)	//Bit Error Count Error.
#define AD242X_INTPND0_PWRERR		(1 << 4)	//Switch Reporting Error on Downstream Power.
#define AD242X_INTPND0_DPERR		(1 << 3)	//Data Parity Error
#define AD242X_INTPND0_CRCERR		(1 << 2)	//CRC Error
#define AD242X_INTPND0_DDERR		(1 << 1)	//Data Decoding Error
#define AD242X_INTPND0_HDCNTERR		(1 << 0)	//Header Count Error

//Interrupt Pending 1 Register
//Description:The AD242X_INTPND1 register contains interrupt pending bits for the node.

#define AD242X_INTPND1_REG		(0x19)
#define AD242X_INTPND1_IO7PND	(1 << 7)	//IO7 Int Pending.
#define AD242X_INTPND1_IO6PND	(1 << 6)	//IO6 Int Pending.
#define AD242X_INTPND1_IO5PND	(1 << 5)	//IO5 Int Pending.
#define AD242X_INTPND1_IO4PND	(1 << 4)	//IO4 Int Pending.
#define AD242X_INTPND1_IO3PND	(1 << 3)	//IO3 Int Pending.
#define AD242X_INTPND1_IO2PND	(1 << 2)	//IO2 Int Pending.
#define AD242X_INTPND1_IO1PND	(1 << 1)	//IO1 Int Pending.
#define AD242X_INTPND1_IO0PND	(1 << 0)	//IO0 Int Pending.

//Interrupt Pending 2 register  (Master Only)
//Description:The AD242X_INTPND2 register contains interrupt pending bits and is only relevant in slave nodes.

#define AD242X_INTPND2_REG		(0x1A)
#define AD242X_INTPND2_SLVIRQ	(1 << 3)	//Slave Interrupt Received Master Only
#define AD242X_INTPND2_ICRCERR	(1 << 2)	//Int Frame CRC Error Master Only
#define AD242X_INTPND2_I2CERR	(1 << 1)	//I2C Transaction Error Master Only
#define AD242X_INTPND2_DSCDONE	(1 << 0)	//Node Discovered Master Only


//Interrupt Mask 0 Register
//Description:The AD242X_INTMSK0 register acts as a mask to determine which of the
//interrupt pending 0 register bits are used to generate an interrupt.

#define AD242X_INTMSK0_REG		(0x1B)
#define AD242X_INTMSK0_SRFCRCEIEN	(1 << 7)	//SRF CRC Error Interrupt Enable.
#define AD242X_INTMSK0_SRFEIEN		(1 << 6)	//SRF Error Interrupt Enable.
#define AD242X_INTMSK0_BECIEN		(1 << 5)	//Bit Error Count Error Interrupt Enable.
#define AD242X_INTMSK0_PWREIEN		(1 << 4)	//Switch Reporting Error Interrupt Enable.
#define AD242X_INTMSK0_DPEIEN		(1 << 3)	//Data Parity Error Interrupt Enable.
#define AD242X_INTMSK0_CRCEIEN		(1 << 2)	//CRC Error Interrupt Enable.
#define AD242X_INTMSK0_DDEIEN		(1 << 1)	//Data Decoding Error Interrupt Enable.
#define AD242X_INTMSK0_HCEIEN		(1 << 0)	//Header Count Error Interrupt Enable.


//Interrupt Mask 1 Register
//Description:The AD242X_INTMSK1 register acts as a mask to determine which of the
//interrupt pending 1 register bits are used to generate an interrupt. For information
//on interrupt types, see the Interrupt Pending register descriptions.

#define AD242X_INTMSK1_REG		(0x1C)
#define AD242X_INTMSK1_IO7IRQEN	(1 << 7)	//IO7 Interrupt Enable.
#define AD242X_INTMSK1_IO6IRQEN	(1 << 6)	//IO6 Interrupt Enable.
#define AD242X_INTMSK1_IO5IRQEN	(1 << 5)	//IO5 Interrupt Enable.
#define AD242X_INTMSK1_IO4IRQEN	(1 << 4)	//IO4 Interrupt Enable.
#define AD242X_INTMSK1_IO3IRQEN	(1 << 3)	//IO3 Interrupt Enable.
#define AD242X_INTMSK1_IO2IRQEN	(1 << 2)	//IO2 Interrupt Enable.
#define AD242X_INTMSK1_IO1IRQEN	(1 << 1)	//IO1 Interrupt Enable.
#define AD242X_INTMSK1_IO0IRQEN	(1 << 0)	//IO0 Interrupt Enable.

//Interrupt Mask 2 Register  (Master Only)
//Description:The AD242X_INTMSK2 register acts as a mask to determine which of the
//interrupt pending 2 register bits are used to generate an interrupt.

#define AD242X_INTMSK2_REG		(0x1D)
#define AD242X_INTMSK2_SLVIRQEN	(1 << 3)	//Slave Interrupt Enable.
#define AD242X_INTMSK2_ICRCEIEN	(1 << 2)	//ICRC Error Interrupt Enable.
#define AD242X_INTMSK2_I2CEIEN	(1 << 1)	//I2C Error Interrupt Enable.
#define AD242X_INTMSK2_DSCDIEN	(1 << 0)	//Node Discovery Done Interrupt Enable.

//Bit Error Count Control Register
//Description:The AD242X_BECCTL register controls bit error counting, including
//interrupt thresholds

#define AD242X_BECCTL_REG			(0x1E)
#define AD242X_BECCTL_THRESHLD_MSK	(0x07) //Threshold to Generate an Interrupt.
#define AD242X_BECCTL_THRESHLD_POS	(5)
#define AD242X_BECCTL_ENICRC		(1 << 4) //Enable ICRCERR Count.
#define AD242X_BECCTL_ENDP			(1 << 3)	//Enable DPERR Count
#define AD242X_BECCTL_ENCRC			(1 << 2)	//Enable CRCERR Count
#define AD242X_BECCTL_ENDD  		(1 << 1)	//Enable DDERR Count
#define AD242X_BECCTL_ENHDCNT  		(1 << 0)	//Enable HDCNTERR Count


//Bit Error Count Register
//Description:The AD242X_BECNT register accumulates the error count of the error
//types as selected in the AD242X_BECCTL register. Any write to this register
//clears the count.

#define AD242X_BECNT_REG		(0x1f)

//Testmode Register
//Description:The AD242X_TESTMODE register provides control bits to be used in
//testing the A2B link.

#define AD242X_TESTMODE_REG		(0x20)
#define AD242X_TESTMODE_PRBSN2N		(1 << 2)
#define AD242X_TESTMODE_PRBSDN		(1 << 1)
#define AD242X_TESTMODE_PRBSUP		(1 << 0)

//PRBS Error Count Byte 0 - 3 Register
//Description:The AD242X_ERRCNT0-4 register holds    Little Endian
//the 32-bit error count accumulated during the PRBS bit error test.

#define AD242X_ERRCNT0_REG		(0x21)
#define AD242X_ERRCNT1_REG		(0x22)
#define AD242X_ERRCNT2_REG		(0x23)
#define AD242X_ERRCNT3_REG		(0x24)

//Node Register
//Description:The AD242X_NODE register contains information required for
//node-to-node communication.

#define AD242X_NODE_REG			(0x29)
#define AD242X_NODE_LAST		(1 << 7)	//Last Node.
#define AD242X_NODE_NLAST		(1 << 6)	//Next-to-Last Node.
#define AD242X_NODE_DISCVD		(1 << 5)	//Node Discovered.
#define AD242X_NODE_NUMBER_MSK	(0x0f)
#define AD242X_NODE_NUMBER_MPOS	(1 << 7)


//Discovery Status Register (Master Only)
//Description:The AD242X_DISCSTAT register provides status for discovery transactions on the A2B bus.

#define AD242X_DISCSTAT_REG		(0x2B)
#define AD242X_DISCSTAT_DSCACT		(1 << 7)	//Discovery Active.
#define AD242X_DISCSTAT_DNODE_MSK	(0x0f)		//Discovery Node.
#define AD242X_DISCSTAT_DNODE_POS	(0)			//Discovery Node.

//Local Interrupt Type (Slave Only)
//Description:XXX

#define AD242X_LINTTYPE_REG		(0x3E)
#define AD242X_LINTTYPE_MB0_FULL		(48)
#define AD242X_LINTTYPE_MB0_EMPTY		(49)
#define AD242X_LINTTYPE_MB1_FULL		(50)
#define AD242X_LINTTYPE_MB1_EMPTY		(51)

//I2C Configuration Register
//Description:The AD242X_I2CCFG register controls the data rate of the I2C port in A2B slave
//nodes and sets the I 2 C behavior in the A2B master node.

#define AD242X_I2CCFG_REG			(0x3F)
#define AD242X_I2CCFG_FRAMERATE		(1 << 2)	//Audio Frame Rate (A2B  Slave Only) 	0=48KHz  / 44,1KHz
#define AD242X_I2CCFG_EACK			(1 << 1)	//Early Acknowledge (A2B Master Only).
#define AD242X_I2CCFG_DATARATE		(1 << 0)	//I2C Data Rate (A2B Slave Only)		0=100KHz / 400 KHz

//I2S Global Configuration Register
//Description:The AD242X_I2SGCFG register provides bits which control the operation
//of all I2S units.

#define AD242X_I2SGCFG_REG		(0x41)
#define AD242X_I2SGCFG_INV		(1 << 7)	//Invert Sync.
#define AD242X_I2SGCFG_EARLY	(1 << 6)	//Early Sync.
#define AD242X_I2SGCFG_ALT		(1 << 5)	//Alternating Sync.
#define AD242X_I2SGCFG_TDMSS	(1 << 4)	//TDM Slot Size.  // 0=32bit / 161bit
#define AD242X_I2SGCFG_TDM2		(0)			//TDM2 Mode.
#define AD242X_I2SGCFG_TDM4		(1)			//TDM4 Mode.
#define AD242X_I2SGCFG_TDM8		(2)			//TDM8 Mode.
#define AD242X_I2SGCFG_TDM12	(3)			//TDM12 Mode. (No slave node support)
#define AD242X_I2SGCFG_TDM16	(4)			//TDM16 Mode.
#define AD242X_I2SGCFG_TDM20	(5)			//TDM20 Mode. (No slave node support)
#define AD242X_I2SGCFG_TDM24	(6)			//TDM24 Mode. (No slave node support)
#define AD242X_I2SGCFG_TDM32	(7)			//TDM32 Mode.


//I2S Configuration Register
//Description:The AD242X_I2SCFG register provides individual settings for both I2S
//receive data signals and both I2S transmit signals.

#define AD242X_I2SCFG_REG		(0x42)
#define AD242X_I2SCFG_RXBCLKINV	(1 << 7)	//RX BCLK Invert.
#define AD242X_I2SCFG_RX2PINTL	(1 << 6)	//RX 2 Pin Interleave.
#define AD242X_I2SCFG_RX1EN		(1 << 5)	//I2S RX 1 Enable.
#define AD242X_I2SCFG_RX0EN		(1 << 4)	//I2S RX 0 Enable.
#define AD242X_I2SCFG_TXBCLKINV	(1 << 3)	//TX BCLK Invert.
#define AD242X_I2SCFG_TX2PINTL	(1 << 2)	//TX 2 Pin Interleave.
#define AD242X_I2SCFG_TX1EN		(1 << 1)	//I2S TX 1 Enable.
#define AD242X_I2SCFG_TX0EN		(1 << 0)	//I2S TX 0 Enable.

//I2S Rate Register (Slave Only)
//Description:The AD242X_I2SRATE register controls the I2S/TDM interfaces in slave nodes.

#define AD242X_I2SRATE_REG		(0x43)
#define AD242X_I2SRATE_SHARE	(1 << 7)	//Share A2B Bus Slots in Reduced Rate Mode.
#define AD242X_I2SRATE_REDUCE	(1 << 6)	//Reduce and Duplicate.
#define AD242X_I2SRATE_BCLKRATE_DEF		(0 << 3)	//BCLK Frequency Determined by Value of I2SGCFG
#define AD242X_I2SRATE_BCLKRATE_2048FS	(1 << 3)	//
#define AD242X_I2SRATE_BCLKRATE_4096FS	(2 << 3)	//
#define AD242X_I2SRATE_BCLKRATE_64FS	(4 << 3)	//
#define AD242X_I2SRATE_BCLKRATE_128FS	(5 << 3)	//
#define AD242X_I2SRATE_BCLKRATE_256FS	(6 << 3)	//
#define AD242X_I2SRATE_I2SRATE_SSFX1	(0 << 0)	//I2S Rate Setting.
#define AD242X_I2SRATE_I2SRATE_SSFd2	(1 << 0)	//
#define AD242X_I2SRATE_I2SRATE_SSFd4	(2 << 0)	//
#define AD242X_I2SRATE_I2SRATE_SSFdRRDIV (3 << 0)	//
#define AD242X_I2SRATE_I2SRATE_SSFx2	(5 << 0)	//
#define AD242X_I2SRATE_I2SRATE_SSFx4	(6 << 0)	//

//I2S Transmit Data Offset Register (Master Only)
//Description:The AD242X_I2STXOFFSET register controls the number of I2S transmit
//channels which are passed over before the node begins transmitting data.

#define AD242X_I2STXOFFSET_REG		(0x44)
#define AD242X_I2STXOFFSET_TSBEFORE		(1 << 7)	//Three-State Before TX Channels.
#define AD242X_I2STXOFFSET_TSAFTER  	(1 << 6)	//Three-State After TX Channels.
#define AD242X_I2STXOFFSET_TXOFFSET0  	(0)			//Serial TX Offset.
#define AD242X_I2STXOFFSET_TXOFFSET1  	(1)			//TX Offset of 1 TDM Channel
#define AD242X_I2STXOFFSET_TXOFFSET62  	(62)		//TX Offset of 62 TDM Channels
#define AD242X_I2STXOFFSET_TXOFFSET63  	(63)		//TX Offset of 63 TDM Channels


//I2S Receive Data Offset Register (Master Only)
//Description:The AD242X_I2SRXOFFSET register controls the number of I2S receive
//channels which are passed over before the node begins receiving data.

#define AD242X_I2SRXOFFSET_REG		(0x45)
#define AD242X_I2SRXOFFSET_TXOFFSET0  	(0)			//Serial RX Offset.
#define AD242X_I2SRXOFFSET_RXOFFSET1  	(1)			//RX Offset of 1 TDM Channel
#define AD242X_I2SRXOFFSET_RXOFFSET62  	(62)		//RX Offset of 62 TDM Channels
#define AD242X_I2SRXOFFSET_RXOFFSET63  	(63)		//RX Offset of 63 TDM Channels


//SYNC Offset Register (Slave Only)
//Description:The AD242X_SYNCOFFSET register adjusts the A2B bus clock (f SYSBCLK )
//cycle count on which the AD242X_SYNC pin indicates the start of an audio frame.
//The offset is an 8-bit signed value, allowing the sync to
//be moved anywhere between 128 cycles before the start of the superframe to 127 cycles
//after the start of the superframe.

#define AD242X_SYNCOFFSET_REG		(0x46)

//PDM Control Register
//Description:The AD242X_PDMCTL register provides enable bits for the pulse density
//demodulators.

#define AD242X_PDMCTL_REG		(0x47)
#define AD242X_PDMCTL_PDMRATE0	(0 << 5)	//PDM Sample Rate.
#define AD242X_PDMCTL_PDMRATE1	(1 << 5)
#define AD242X_PDMCTL_PDMRATE2	(2 << 5)
#define AD242X_PDMCTL_HPFEN		(1 << 4)	//Highpass Filter Enable.
#define AD242X_PDMCTL_PDM1SLOTS	(1 << 3)	//PDM1 Channels.
#define AD242X_PDMCTL_PDM1EN	(1 << 2)	//PDM1 Enable
#define AD242X_PDMCTL_PDM0SLOTS	(1 << 1)	//PDM0 Channels
#define AD242X_PDMCTL_PDM0EN	(1 << 0) 	//PDM0 Enable

//Error Management Register
//Description:The AD242X_ERRMGMT register provides options for reporting communication
//errors over the I 2 S/TDM interface.

#define AD242X_ERRMGMT_REG		(0x48)
#define AD242X_ERRMGMT_ERRSLOT	(1 << 2)	//Add Error Indicating Channel to I 2 S/TDM Output.
#define AD242X_ERRMGMT_ERRSIG	(1 << 1)	//Show Data Error on Remaining Bits.
#define AD242X_ERRMGMT_ERRLSB	(1 << 0) 	//Show Data Error on LSB.

//GPIO Output Data Register
//Description:The AD242X_GPIODAT register controls output data for general-purpose IO pins.

#define AD242X_GPIODAT_REG		(0x4A)
#define AD242X_GPIODAT_IO7DAT	(1 << 7)	//IO7 Output Data. 0 = output low / high
#define AD242X_GPIODAT_IO6DAT	(1 << 6)	//IO6 Output Data. 0 = output low / high
#define AD242X_GPIODAT_IO5DAT	(1 << 5)	//IO5 Output Data. 0 = output low / high
#define AD242X_GPIODAT_IO4DAT	(1 << 4)	//IO4 Output Data. 0 = output low / high
#define AD242X_GPIODAT_IO3DAT	(1 << 3)	//IO3 Output Data. 0 = output low / high
#define AD242X_GPIODAT_IO2DAT	(1 << 2)	//IO2 Output Data. 0 = output low / high
#define AD242X_GPIODAT_IO1DAT	(1 << 1)	//IO1 Output Data. 0 = output low / high
#define AD242X_GPIODAT_IO0DAT	(1 << 0)	//IO0 Output Data. 0 = output low / high


//GPIO Output Data Set Register
//Description:The AD242X_GPIODATSET register allows setting of individual GPIO output
//register bits (write 1 to set) with- out influencing the 0 or 1 value of other GPIO output
// register bits.

#define AD242X_GPIODATSET_REG		(0x4B)
#define AD242X_GPIODATSET_IO7DSET	(1 << 7)	//Write 1 to Set IO7DAT.
#define AD242X_GPIODATSET_IO6DSET	(1 << 6)	//Write 1 to Set IO6DAT.
#define AD242X_GPIODATSET_IO5DSET	(1 << 5)	//Write 1 to Set IO5DAT.
#define AD242X_GPIODATSET_IO4DSET	(1 << 4)	//Write 1 to Set IO4DAT.
#define AD242X_GPIODATSET_IO3DSET	(1 << 3)	//Write 1 to Set IO3DAT.
#define AD242X_GPIODATSET_IO2DSET	(1 << 2)	//Write 1 to Set IO2DAT.
#define AD242X_GPIODATSET_IO1DSET	(1 << 1)	//Write 1 to Set IO1DAT.
#define AD242X_GPIODATSET_IO0DSET	(1 << 0)	//Write 1 to Set IO0DAT.

//GPIO Output Data Clear Register
//Description:The AD242X_GPIODATCLR register allows clearing of individual GPIO output
//register bits to 0 (write 1 to clear) without influencing the 0 or 1 value of other
//GPIO output register.

#define AD242X_GPIODATCLR_REG		(0x4C)
#define AD242X_GPIODATCLR_IO7DCLR	(1 << 7)	//Write 1 to CLR IO7DAT.
#define AD242X_GPIODATCLR_IO6DCLR	(1 << 6)	//Write 1 to CLR IO6DAT.
#define AD242X_GPIODATCLR_IO5DCLR	(1 << 5)	//Write 1 to CLR IO5DAT.
#define AD242X_GPIODATCLR_IO4DCLR	(1 << 4)	//Write 1 to CLR IO4DAT.
#define AD242X_GPIODATCLR_IO3DCLR	(1 << 3)	//Write 1 to CLR IO3DAT.
#define AD242X_GPIODATCLR_IO2DCLR	(1 << 2)	//Write 1 to CLR IO2DAT.
#define AD242X_GPIODATCLR_IO1DCLR	(1 << 1)	//Write 1 to CLR IO1DAT.
#define AD242X_GPIODATCLR_IO0DCLR	(1 << 0)	//Write 1 to CLR IO0DAT.


//GPIO Output Enable Register
//Description:The AD242X_GPIOOEN register controls the output enables of the 
//general-purpose IO pins.

#define AD242X_GPIOOEN_REG		(0x4D)
#define AD242X_GPIOOEN_IO7OEN	(1 << 7)	//IO7 Output Enable.
#define AD242X_GPIOOEN_IO6OEN	(1 << 6)	//IO6 Output Enable.
#define AD242X_GPIOOEN_IO5OEN	(1 << 5)	//IO5 Output Enable.
#define AD242X_GPIOOEN_IO4OEN	(1 << 4)	//IO4 Output Enable.
#define AD242X_GPIOOEN_IO3OEN	(1 << 3)	//IO3 Output Enable.
#define AD242X_GPIOOEN_IO2OEN	(1 << 2)	//IO2 Output Enable.
#define AD242X_GPIOOEN_IO1OEN	(1 << 1)	//IO1 Output Enable.
#define AD242X_GPIOOEN_IO0OEN	(1 << 0)	//IO0 Output Enable.

//GPIO Input Enable Register
//Description:The AD242X_GPIOIEN register controls the input enables of the 
//general purpose IO pins.

#define AD242X_GPIOIEN_REG		(0x4E)
#define AD242X_GPIOIEN_IO7IEN	(1 << 7)	//IO7 Input Enable.
#define AD242X_GPIOIEN_IO6IEN	(1 << 6)	//IO6 Input Enable.
#define AD242X_GPIOIEN_IO5IEN	(1 << 5)	//IO5 Input Enable.
#define AD242X_GPIOIEN_IO4IEN	(1 << 4)	//IO4 Input Enable.
#define AD242X_GPIOIEN_IO3IEN	(1 << 3)	//IO3 Input Enable.
#define AD242X_GPIOIEN_IO2IEN	(1 << 2)	//IO2 Input Enable.
#define AD242X_GPIOIEN_IO1IEN	(1 << 1)	//IO1 Input Enable.
#define AD242X_GPIOIEN_IO0IEN	(1 << 0)	//IO0 Input Enable.

//GPIO Input Value Register
//Description:The AD242X_GPIOIN register returns the value of enabled 
//general-purpose IO pins.

#define AD242X_GPIOIN_REG		(0x4F)
#define AD242X_GPIOIN_IO7IN		(1 << 7)	//IO7 Input Value.
#define AD242X_GPIOIN_IO6IN		(1 << 6)	//IO6 Input Value.
#define AD242X_GPIOIN_IO5IN		(1 << 5)	//IO5 Input Value.
#define AD242X_GPIOIN_IO4IN		(1 << 4)	//IO4 Input Value.
#define AD242X_GPIOIN_IO3IN		(1 << 3)	//IO3 Input Value.
#define AD242X_GPIOIN_IO2IN		(1 << 2)	//IO2 Input Value.
#define AD242X_GPIOIN_IO1IN		(1 << 1)	//IO1 Input Value.
#define AD242X_GPIOIN_IO0IN		(1 << 0)	//IO0 Input Value.


//Pin Interrupt Enable Register
//Description:The AD242X_PINTEN register is used to enable pin input 
//values used to generate an interrupt.

#define AD242X_PINTEN_REG		(0x50)
#define AD242X_PINTEN_IO7IE		(1 << 7)	//IO7 Interrupt Enable .
#define AD242X_PINTEN_IO6IE		(1 << 6)	//IO6 Interrupt Enable .
#define AD242X_PINTEN_IO5IE		(1 << 5)	//IO5 Interrupt Enable .
#define AD242X_PINTEN_IO4IE		(1 << 4)	//IO4 Interrupt Enable .
#define AD242X_PINTEN_IO3IE		(1 << 3)	//IO3 Interrupt Enable .
#define AD242X_PINTEN_IO2IE		(1 << 2)	//IO2 Interrupt Enable .
#define AD242X_PINTEN_IO1IE		(1 << 1)	//IO1 Interrupt Enable .
#define AD242X_PINTEN_IO0IE		(1 << 0)	//IO0 Interrupt Enable .

//Pin Interrupt Invert Register
//Description:The AD242X_PINTINV register is used to invert pin inputs in the path to interrupt generation.

#define AD242X_PINTINV_REG		(0x51)
#define AD242X_PINTINV_IO7INV		(1 << 7)	//Invert IO7.
#define AD242X_PINTINV_IO6INV		(1 << 6)	//Invert IO6.
#define AD242X_PINTINV_IO5INV		(1 << 5)	//Invert IO5.
#define AD242X_PINTINV_IO4INV		(1 << 4)	//Invert IO4.
#define AD242X_PINTINV_IO3INV		(1 << 3)	//Invert IO3.
#define AD242X_PINTINV_IO2INV		(1 << 2)	//Invert IO2.
#define AD242X_PINTINV_IO1INV		(1 << 1)	//Invert IO1.
#define AD242X_PINTINV_IO0INV		(1 << 0)	//Invert IO0.


//Pin Configuration Register
//Description:The AD242X_PINCFG register configures the digital pins drive strengths.

#define AD242X_PINCFG_REG		(0x52)
#define AD242X_PINCFG_IRQTS		(1 << 5) 	//Tristate IRQ.
#define AD242X_PINCFG_IRQINV	(1 << 4)	//Invert IRQ.
#define AD242X_PINCFG_TXBLP		(1 << 2)	//LVDS XCVRB Low Power TX Mode.
#define AD242X_PINCFG_TXALP		(1 << 1)	//LVDS XCVRA Low Power TX Mode.
#define AD242X_PINCFG_DRVSTR	(1 << 0)	//Digital Pin Drive Strength.

//I2S Test Register
//Description:The AD242X_I2STEST register enables test modes to verify and debug the I 2 S/TDM interface.

#define AD242X_I2STEST_REG		(0x53)
#define AD242X_I2STEST_BUSLOOPBK	(1 << 4)	//Bus Loopback.
#define AD242X_I2STEST_SELRX1		(1 << 3)	//Select RX1 Block to Loopback Buffer.
#define AD242X_I2STEST_RX2LOOPBK	(1 << 2)	//RX Block to Loopback Buffer.
#define AD242X_I2STEST_LOOPBK2TX	(1 << 1)	//Loopback Data to TX Blocks.
#define AD242X_I2STEST_PATTRN2TX	(1 << 0)	//Default Bit Pattern to Serial TX Blocks.

//Raise Interrupt Register
//Description:The AD242X_RAISE register allows the host to generate an interrupt 
//in any node in the system through software. This register must be written over the 
//A2B bus, write to this register from the local I 2 C port has no effect.

#define AD242X_RAISE_REG		(0x54)
#define AD242X_RAISE_HDCNTERR	(0)
#define AD242X_RAISE_DDERR		(1 )
#define AD242X_RAISE_CRCERR		(2 )
#define AD242X_RAISE_DPERR		(3 )
#define AD242X_RAISE_BECOVF		(4 )
#define AD242X_RAISE_SRFERR		(5 )
#define AD242X_RAISE_SRFCRCERR	(6 )
#define AD242X_RAISE_PWRERR0 	(9 )
#define AD242X_RAISE_PWRERR1 	(10)
#define AD242X_RAISE_PWRERR2 	(11)
#define AD242X_RAISE_PWRERR3 	(12)
#define AD242X_RAISE_PWRERR4 	(13)
#define AD242X_RAISE_PWRERR5 	(15)
#define AD242X_RAISE_IO0PND 	(16)
#define AD242X_RAISE_IO1PND		(17)
#define AD242X_RAISE_IO2PND		(18)
#define AD242X_RAISE_IO3PND		(19)

//Generate Bus Error
//Description:The AD242X_GENERR register allows the host to generate bus errors from 
//any node in the system through soft- ware. This register must be written over the 
//A2B bus, write to this register from the local I 2 C port has no effect.

#define AD242X_GENERR_REG		(0x5)
#define AD242X_GENERR_GENICRCERR	(1 << 4)	//Generate Int Frame CRC Error (Slave Only).
#define AD242X_GENERR_GENDPERR		(1 << 3)	//Generate Data Parity Error.
#define AD242X_GENERR_GENCRCERR		(1 << 2)	//Generate CRC Error.
#define AD242X_GENERR_GENDDERR		(1 << 1)	//Generate Data Decoding Error.
#define AD242X_GENERR_GENHCERR		(1 << 0)	//Generate Header Count Error.

//I 2 S Reduced Rate Register (Mstr Only, Auto-Brcst)
//Description:XXX

#define AD242X_I2SRRATE_REG		(0x56)
#define AD242X_I2SRRATE_RBUS	(1 << 7)	//Reduce Data Rate on A 2 B Bus.
#define AD242X_I2SRRATE_RRDIV_MSK	(0x3f)	//
#define AD242X_I2SRRATE_RRDIV_POS	(0)	//

//I 2 S Reduced Rate Control Register
//Description:XXX

#define AD242X_I2SRRCTL_REG		(0x57)
#define AD242X_I2SRRCTL_ENCHAN	(1 << 6)	//Enable I 2 S Reduced Rate Valid Bits in Extra I 2 S Channel.
#define AD242X_I2SRRCTL_STRBDIR	(1 << 5)	//I 2 S Reduced Rate Strobe Direction.
#define AD242X_I2SRRCTL_ENSTRB	(1 << 4)	//Enable I 2 S Reduced Rate Strobe.
#define AD242X_I2SRRCTL_ENXBIT	(1 << 1)	//Enable I 2 S Reduced Rate Valid Bit in Extra Bit.
#define AD242X_I2SRRCTL_ENVLSB	(1 << 0)	//Enable I 2 S Reduced Rate Valid Bit in LSB.

//I 2 S Reduced Rate SYNC Offset (Slave Only)
//Description:XXX

#define AD242X_I2SRRSOFFS_REG		(0x58)
#define AD242X_I2SRRSOFFS_MSK		(0x03)
#define AD242X_I2SRRSOFFS_POS		(0)

//CLKOUT1 Configuration Register
//Description:The AD242X_CLK1CFG register is used to enable and set the frequency of 
//an output clock which is produced on the AD242X_ADR1/AD242X_IO1 pin.

#define AD242X_CLK1CFG_REG		(0x59)
#define AD242X_CLK1CFG_CLK1EN 		(1 << 7)	//CLKOUT1 Enable.
#define AD242X_CLK1CFG_CLK1INV 		(1 << 6)	//CLKOUT1 Invert Enable.
#define AD242X_CLK1CFG_CLK1PDIV 	(1 << 5)	//
#define AD242X_CLK1CFG_CLK1DIV_MKS 	(0x0f)		//Divide Value for CLKOUT1.
#define AD242X_CLK1CFG_CLK1DIV_POS 	(0)			//Divide Value for CLKOUT1.

//CLKOUT2 Configuration Register
//Description:The AD242X_CLK2CFG register is used to enable and set the frequency of 
//an output clock which is produced on the AD242X_ADR2/AD242X_IO2 pin.

#define AD242X_CLK2CFG_REG		(0x5A)
#define AD242X_CLK2CFG_CLK2EN 		(1 << 7)	//CLKOUT2 Enable.
#define AD242X_CLK2CFG_CLK2INV 		(1 << 6)	//CLKOUT2 Invert Enable.
#define AD242X_CLK2CFG_CLK2PDIV 	(1 << 5)	//
#define AD242X_CLK2CFG_CLK2DIV_MKS 	(0x0f)		//Divide Value for CLKOUT2.
#define AD242X_CLK2CFG_CLK2DIV_POS 	(0)			//Divide Value for CLKOUT2.

//Bus Monitor Mode Configuration Register
//Description:The AD242X_BMMCFG register is used to place the part into Bus Monitor 
//Mode and to configure settings for Bus Monitor Mode.

#define AD242X_BMMCFG_REG		(0x5B)
#define AD242X_BMMCFG_BMMNDSC	(1 << 2)	//BMM No Discovery Mode Enable.
#define AD242X_BMMCFG_BMMRXEN	(1 << 1)	//BMM LVDS XCVR RX Enable.
#define AD242X_BMMCFG_BMMEN		(1 << 0)	//Bus Monitor Mode Enable.

//Sustain Configuration Register (Slave Only)
//Description:The AD242X_SUSCFG register is used to configure sustain functionality in a slave node.

#define AD242X_SUSCFG_REG		(0x5C)
#define AD242X_SUSCFG_SUSDIS	(1 << 5)	//Sustain Disable.
#define AD242X_SUSCFG_SUSOE		(1 << 4)	//Sustain GPIO Output Enable.
#define AD242X_SUSCFG_SUSSEL_MSK (0x07)		//Sustain GPIO Output Select.
#define AD242X_SUSCFG_SUSSEL_POS (0)		//Sustain GPIO Output Select.

//Upstream Data RX Mask 0 (Slave Only)
//Description:The AD242X_UPMASK0 register defines bits 7 to 0 of the 32-bit 
//UPMASK field. In a slave node, the UPMASK field defines the upstream data 
//slots which are received from the A 2 B bus by the local node.

#define AD242X_UPMASK0_REG		(0x60)
#define AD242X_UPMASK0_RXUPSLOT07	(1 << 7) //Receive Upstream Data Slot 7.
#define AD242X_UPMASK0_RXUPSLOT06	(1 << 6) //Receive Upstream Data Slot 6.
#define AD242X_UPMASK0_RXUPSLOT05	(1 << 5) //Receive Upstream Data Slot 5.
#define AD242X_UPMASK0_RXUPSLOT04	(1 << 4) //Receive Upstream Data Slot 4.
#define AD242X_UPMASK0_RXUPSLOT03	(1 << 3) //Receive Upstream Data Slot 3.
#define AD242X_UPMASK0_RXUPSLOT02	(1 << 2) //Receive Upstream Data Slot 2.
#define AD242X_UPMASK0_RXUPSLOT01	(1 << 1) //Receive Upstream Data Slot 1.
#define AD242X_UPMASK0_RXUPSLOT00	(1 << 0) //Receive Upstream Data Slot 0.

//Upstream Data RX Mask 1 (Slave Only)
//Description:The AD242X_UPMASK0 register defines bits 15 to 8 of the 32-bit 
//UPMASK field. In a slave node, the UPMASK field defines the upstream data 
//slots which are received from the A 2 B bus by the local node.

#define AD242X_UPMASK1_REG		(0x61)
#define AD242X_UPMASK1_RXUPSLOT15	(1 << 7) //Receive Upstream Data Slot 15.
#define AD242X_UPMASK1_RXUPSLOT14	(1 << 6) //Receive Upstream Data Slot 14.
#define AD242X_UPMASK1_RXUPSLOT13	(1 << 5) //Receive Upstream Data Slot 13.
#define AD242X_UPMASK1_RXUPSLOT12	(1 << 4) //Receive Upstream Data Slot 12.
#define AD242X_UPMASK1_RXUPSLOT11	(1 << 3) //Receive Upstream Data Slot 11.
#define AD242X_UPMASK1_RXUPSLOT10	(1 << 2) //Receive Upstream Data Slot 10.
#define AD242X_UPMASK1_RXUPSLOT09	(1 << 1) //Receive Upstream Data Slot 09.
#define AD242X_UPMASK1_RXUPSLOT08	(1 << 0) //Receive Upstream Data Slot 08.

//Upstream Data RX Mask 2 (Slave Only)
//Description:The AD242X_UPMASK0 register defines bits 23 to 16 of the 32-bit 
//UPMASK field. In a slave node, the UPMASK field defines the upstream data 
//slots which are received from the A 2 B bus by the local node.

#define AD242X_UPMASK2_REG		(0x62)
#define AD242X_UPMASK2_RXUPSLOT23	(1 << 7) //Receive Upstream Data Slot 23.
#define AD242X_UPMASK2_RXUPSLOT22	(1 << 6) //Receive Upstream Data Slot 22.
#define AD242X_UPMASK2_RXUPSLOT21	(1 << 5) //Receive Upstream Data Slot 21.
#define AD242X_UPMASK2_RXUPSLOT20	(1 << 4) //Receive Upstream Data Slot 20.
#define AD242X_UPMASK2_RXUPSLOT19	(1 << 3) //Receive Upstream Data Slot 19.
#define AD242X_UPMASK2_RXUPSLOT18	(1 << 2) //Receive Upstream Data Slot 18.
#define AD242X_UPMASK2_RXUPSLOT17	(1 << 1) //Receive Upstream Data Slot 17.
#define AD242X_UPMASK2_RXUPSLOT16	(1 << 0) //Receive Upstream Data Slot 16.

//Upstream Data RX Mask 3 (Slave Only)
//Description:The AD242X_UPMASK0 register defines bits 31 to 24 of the 32-bit 
//UPMASK field. In a slave node, the UPMASK field defines the upstream data 
//slots which are received from the A 2 B bus by the local node.

#define AD242X_UPMASK3_REG		(0x63)
#define AD242X_UPMASK3_RXUPSLOT31	(1 << 7) //Receive Upstream Data Slot 31.
#define AD242X_UPMASK3_RXUPSLOT30	(1 << 6) //Receive Upstream Data Slot 30.
#define AD242X_UPMASK3_RXUPSLOT29	(1 << 5) //Receive Upstream Data Slot 29.
#define AD242X_UPMASK3_RXUPSLOT28	(1 << 4) //Receive Upstream Data Slot 28.
#define AD242X_UPMASK3_RXUPSLOT27	(1 << 3) //Receive Upstream Data Slot 27.
#define AD242X_UPMASK3_RXUPSLOT26	(1 << 2) //Receive Upstream Data Slot 26.
#define AD242X_UPMASK3_RXUPSLOT25	(1 << 1) //Receive Upstream Data Slot 25.
#define AD242X_UPMASK3_RXUPSLOT24	(1 << 0) //Receive Upstream Data Slot 24.


//Local Upstream Channels Offset Register (Slave Only)
//Description:In a slave node, the AD242X_UPOFFSET register defines number of 
//data channels received via I 2 S/TDM/PDM, which are skipped before data slots 
//are transmitted upstream on the A 2 B bus.

#define AD242X_UPOFFSET_REG		(0x64)
#define AD242X_UPOFFSET_MSK		(0x1f)	//Upstream Channel Offset for Local Node.
#define AD242X_UPOFFSET_POS		(0)		//


//Downsstream Data RX Mask 0 (Slave Only)
//Description:The AD242X_UPMASK0 register defines bits 7 to 0 of the 32-bit 
//UPMASK field. In a slave node, the UPMASK field defines the Downsstream data 
//slots which are received from the A 2 B bus by the local node.

#define AD242X_DNMASK0_REG		(0x65)
#define AD242X_DNMASK0_RXDNSLOT07	(1 << 7) //Receive Downsstream Data Slot 7.
#define AD242X_DNMASK0_RXDNSLOT06	(1 << 6) //Receive Downsstream Data Slot 6.
#define AD242X_DNMASK0_RXDNSLOT05	(1 << 5) //Receive Downsstream Data Slot 5.
#define AD242X_DNMASK0_RXDNSLOT04	(1 << 4) //Receive Downsstream Data Slot 4.
#define AD242X_DNMASK0_RXDNSLOT03	(1 << 3) //Receive Downsstream Data Slot 3.
#define AD242X_DNMASK0_RXDNSLOT02	(1 << 2) //Receive Downsstream Data Slot 2.
#define AD242X_DNMASK0_RXDNSLOT01	(1 << 1) //Receive Downsstream Data Slot 1.
#define AD242X_DNMASK0_RXDNSLOT00	(1 << 0) //Receive Downsstream Data Slot 0.

//Downsstream Data RX Mask 1 (Slave Only)
//Description:The AD242X_DNMASK0 register defines bits 15 to 8 of the 32-bit 
//DNMASK field. In a slave node, the DNMASK field defines the Downsstream data 
//slots which are received from the A 2 B bus by the local node.

#define AD242X_DNMASK1_REG		(0x66)
#define AD242X_DNMASK1_RXDNSLOT15	(1 << 7) //Receive Downsstream Data Slot 15.
#define AD242X_DNMASK1_RXDNSLOT14	(1 << 6) //Receive Downsstream Data Slot 14.
#define AD242X_DNMASK1_RXDNSLOT13	(1 << 5) //Receive Downsstream Data Slot 13.
#define AD242X_DNMASK1_RXDNSLOT12	(1 << 4) //Receive Downsstream Data Slot 12.
#define AD242X_DNMASK1_RXDNSLOT11	(1 << 3) //Receive Downsstream Data Slot 11.
#define AD242X_DNMASK1_RXDNSLOT10	(1 << 2) //Receive Downsstream Data Slot 10.
#define AD242X_DNMASK1_RXDNSLOT09	(1 << 1) //Receive Downsstream Data Slot 09.
#define AD242X_DNMASK1_RXDNSLOT08	(1 << 0) //Receive Downsstream Data Slot 08.

//Downsstream Data RX Mask 2 (Slave Only)
//Description:The AD242X_DNMASK0 register defines bits 23 to 16 of the 32-bit 
//DNMASK field. In a slave node, the DNMASK field defines the Downsstream data 
//slots which are received from the A 2 B bus by the local node.

#define AD242X_DNMASK2_REG		(0x67)
#define AD242X_DNMASK2_RXDNSLOT23	(1 << 7) //Receive Downsstream Data Slot 23.
#define AD242X_DNMASK2_RXDNSLOT22	(1 << 6) //Receive Downsstream Data Slot 22.
#define AD242X_DNMASK2_RXDNSLOT21	(1 << 5) //Receive Downsstream Data Slot 21.
#define AD242X_DNMASK2_RXDNSLOT20	(1 << 4) //Receive Downsstream Data Slot 20.
#define AD242X_DNMASK2_RXDNSLOT19	(1 << 3) //Receive Downsstream Data Slot 19.
#define AD242X_DNMASK2_RXDNSLOT18	(1 << 2) //Receive Downsstream Data Slot 18.
#define AD242X_DNMASK2_RXDNSLOT17	(1 << 1) //Receive Downsstream Data Slot 17.
#define AD242X_DNMASK2_RXDNSLOT16	(1 << 0) //Receive Downsstream Data Slot 16.

//Downsstream Data RX Mask 3 (Slave Only)
//Description:The AD242X_DNMASK0 register defines bits 31 to 24 of the 32-bit 
//DNMASK field. In a slave node, the DNMASK field defines the Downsstream data 
//slots which are received from the A 2 B bus by the local node.

#define AD242X_DNMASK3_REG		(0x68)
#define AD242X_DNMASK3_RXDNSLOT31	(1 << 7) //Receive Downsstream Data Slot 31.
#define AD242X_DNMASK3_RXDNSLOT30	(1 << 6) //Receive Downsstream Data Slot 30.
#define AD242X_DNMASK3_RXDNSLOT29	(1 << 5) //Receive Downsstream Data Slot 29.
#define AD242X_DNMASK3_RXDNSLOT28	(1 << 4) //Receive Downsstream Data Slot 28.
#define AD242X_DNMASK3_RXDNSLOT27	(1 << 3) //Receive Downsstream Data Slot 27.
#define AD242X_DNMASK3_RXDNSLOT26	(1 << 2) //Receive Downsstream Data Slot 26.
#define AD242X_DNMASK3_RXDNSLOT25	(1 << 1) //Receive Downsstream Data Slot 25.
#define AD242X_DNMASK3_RXDNSLOT24	(1 << 0) //Receive Downsstream Data Slot 24.


//Local Downsstream Channels Offset Register (Slave Only)
//Description:In a slave node, the AD242X_DNOFFSET register defines number of 
//data channels received via I 2 S/TDM/PDM, which are skipped before data slots 
//are transmitted Downsstream on the A 2 B bus.

#define AD242X_DNOFFSET_REG		(0x69)
#define AD242X_DNOFFSET_MSK		(0x1f)	//Downsstream Channel Offset for Local Node.
#define AD242X_DNOFFSET_POS		(0)		//



//GPIO Over Distance Enable Register
//Description:The AD242X_GPIODEN register controls the enables of the general-purpose 
//IO pins for use in GPIO Over Distance.

#define AD242X_GPIODEN_REG		(0x80)
#define AD242X_GPIODEN_IOD7EN		(1 << 7)	//GPIO Over Distance IO7 Enable.
#define AD242X_GPIODEN_IOD6EN		(1 << 6)	//GPIO Over Distance IO6 Enable.
#define AD242X_GPIODEN_IOD5EN		(1 << 5)	//GPIO Over Distance IO5 Enable.
#define AD242X_GPIODEN_IOD4EN		(1 << 4)	//GPIO Over Distance IO4 Enable.
#define AD242X_GPIODEN_IOD3EN		(1 << 3)	//GPIO Over Distance IO3 Enable.
#define AD242X_GPIODEN_IOD2EN		(1 << 2)	//GPIO Over Distance IO2 Enable.
#define AD242X_GPIODEN_IOD1EN		(1 << 1)	//GPIO Over Distance IO1 Enable.
#define AD242X_GPIODEN_IOD0EN		(1 << 0)	//GPIO Over Distance IO0 Enable.

//GPIO Over Distance Mask 0-7 Register
//Description:XXX

#define AD242X_GPIOD0MSK_REG		(0x81)	//GPIO Over Distance IO0 Mask.
#define AD242X_GPIOD1MSK_REG		(0x82)	//GPIO Over Distance IO1 Mask.
#define AD242X_GPIOD2MSK_REG		(0x83)	//GPIO Over Distance IO2 Mask.
#define AD242X_GPIOD3MSK_REG		(0x84)	//GPIO Over Distance IO3 Mask.
#define AD242X_GPIOD4MSK_REG		(0x85)	//GPIO Over Distance IO4 Mask.
#define AD242X_GPIOD5MSK_REG		(0x86)	//GPIO Over Distance IO5 Mask.
#define AD242X_GPIOD6MSK_REG		(0x87)	//GPIO Over Distance IO6 Mask.
#define AD242X_GPIOD7MSK_REG		(0x88)	//GPIO Over Distance IO7 Mask.

//GPIO Over Distance Data Register
//Description:XXX

#define AD242X_GPIODDAT_REG		(0x89)
#define AD242X_GPIODDAT_IOD7DAT	(1 << 7)	//Bus GPIO Port Bit 7 Value.
#define AD242X_GPIODDAT_IOD6DAT	(1 << 6)	//Bus GPIO Port Bit 6 Value.
#define AD242X_GPIODDAT_IOD5DAT	(1 << 5)	//Bus GPIO Port Bit 5 Value.
#define AD242X_GPIODDAT_IOD4DAT	(1 << 4)	//Bus GPIO Port Bit 4 Value.
#define AD242X_GPIODDAT_IOD3DAT	(1 << 3)	//Bus GPIO Port Bit 3 Value.
#define AD242X_GPIODDAT_IOD2DAT	(1 << 2)	//Bus GPIO Port Bit 2 Value.
#define AD242X_GPIODDAT_IOD1DAT	(1 << 1)	//Bus GPIO Port Bit 1 Value.
#define AD242X_GPIODDAT_IOD0DAT	(1 << 0)	//Bus GPIO Port Bit 0 Value.


//GPIO Over Distance Invert Register
//Description:XXX

#define AD242X_GPIODINV_REG		(0x8A)
#define AD242X_GPIODINV_IOD7INV	(1 << 7)	//GPIO Over Distance IO7 Invert.
#define AD242X_GPIODINV_IOD6INV	(1 << 6)	//GPIO Over Distance IO6 Invert.
#define AD242X_GPIODINV_IOD5INV	(1 << 5)	//GPIO Over Distance IO5 Invert.
#define AD242X_GPIODINV_IOD4INV	(1 << 4)	//GPIO Over Distance IO4 Invert.
#define AD242X_GPIODINV_IOD3INV	(1 << 3)	//GPIO Over Distance IO3 Invert.
#define AD242X_GPIODINV_IOD2INV	(1 << 2)	//GPIO Over Distance IO2 Invert.
#define AD242X_GPIODINV_IOD1INV	(1 << 1)	//GPIO Over Distance IO1 Invert.
#define AD242X_GPIODINV_IOD0INV	(1 << 0)	//GPIO Over Distance IO0 Invert.

//Mailbox 0 Control Register (Slave Only)
//Description:The AD242X_MBOX0CTL register contains bits that control direction, 
//message length and interrupts.

#define XXAD242X_MBOX0CTL_REG		(0x90)
#define XXAD242X_MBOX0CTL_MB0LEN1B	(0 << 4) //Mailbox 0 Length is 1 Byte
#define XXAD242X_MBOX0CTL_MB0LEN2B	(1 << 4) //Mailbox 0 Length is 2 Bytes
#define XXAD242X_MBOX0CTL_MB0LEN3B	(2 << 4) //Mailbox 0 Length is 3 Bytes
#define XXAD242X_MBOX0CTL_MB0LEN4B	(3 << 4) //Mailbox 0 Length is 4 Bytes
#define XXAD242X_MBOX0CTL_MMB0FIEN	(1 << 3) //Mailbox 0 Full Interrupt Enable.
#define XXAD242X_MBOX0CTL_MB0EIEN	(1 << 2) //Mailbox 0 Empty Interrupt Enable.
#define XXAD242X_MBOX0CTL_MB0DIR	(1 << 1) //Mailbox 0 Direction.
#define XXAD242X_MBOX0CTL_MB0EN		(1 << 0) //Mailbox 0 Enable.

//Mailbox 0 Status Register (Slave Only)
//Description:The AD242X_MBOX0STAT register reports the status of the configured mailbox interrupts.

#define AD242X_MBOX0STAT_REG		(0x91)
#define AD242X_MBOX0STAT_MB0EIRQ		(1<< 5)	//Mailbox 0 Signaling Empty IRQ.
#define AD242X_MBOX0STAT_MB0FIRQ		(1<< 4)	//Mailbox 0 Signaling Full IRQ.
#define AD242X_MBOX0STAT_MB0EMPTY		(1<< 1)	//Mailbox 0 Empty.
#define AD242X_MBOX0STAT_MB0FULL		(1<< 0)	//Mailbox 0 Full


//Mailbox 0 Byte 0-3 Register (Slave Only)
//Description:XXX

#define AD242X_MBOX0B0_REG		(0x92)
#define AD242X_MBOX0B1_REG		(0x93)
#define AD242X_MBOX0B2_REG		(0x94)
#define AD242X_MBOX0B3_REG		(0x95)



//Mailbox 1 Control Register (Slave Only)
//Description:The AD242X_MBOX1CTL register contains bits that control direction, 
//message length and interrupts.

#define XXAD242X_MBOX1CTL_REG		(0x96)
#define XXAD242X_MBOX1CTL_MB0LEN1B	(0 << 4) //Mailbox 1 Length is 1 Byte
#define XXAD242X_MBOX1CTL_MB0LEN2B	(1 << 4) //Mailbox 1 Length is 2 Bytes
#define XXAD242X_MBOX1CTL_MB0LEN3B	(2 << 4) //Mailbox 1 Length is 3 Bytes
#define XXAD242X_MBOX1CTL_MB0LEN4B	(3 << 4) //Mailbox 1 Length is 4 Bytes
#define XXAD242X_MBOX1CTL_MMB0FIEN	(1 << 3) //Mailbox 1 Full Interrupt Enable.
#define XXAD242X_MBOX1CTL_MB0EIEN	(1 << 2) //Mailbox 1 Empty Interrupt Enable.
#define XXAD242X_MBOX1CTL_MB0DIR	(1 << 1) //Mailbox 1 Direction.
#define XXAD242X_MBOX1CTL_MB0EN		(1 << 0) //Mailbox 1 Enable.

//Mailbox 1 Status Register (Slave Only)
//Description:The AD242X_MBOX1STAT register reports the status of the configured mailbox interrupts.

#define AD242X_MBOX1STAT_REG		(0x97)
#define AD242X_MBOX1STAT_MB0EIRQ		(1<< 5)	//Mailbox 1 Signaling Empty IRQ.
#define AD242X_MBOX1STAT_MB0FIRQ		(1<< 4)	//Mailbox 1 Signaling Full IRQ.
#define AD242X_MBOX1STAT_MB0EMPTY		(1<< 1)	//Mailbox 1 Empty.
#define AD242X_MBOX1STAT_MB0FULL		(1<< 0)	//Mailbox 1 Full


//Mailbox 1 Byte 0-3 Register (Slave Only)
//Description:XXX

#define AD242X_MBOX1B0_REG		(0x98)
#define AD242X_MBOX1B1_REG		(0x99)
#define AD242X_MBOX1B2_REG		(0x9A)
#define AD242X_MBOX1B3_REG		(0x9B)


//AMS Control Register 0-2
//Description:XXX

#define AD242X_AMSCTL0_REG		(0xA0)
#define AD242X_AMSCTL1_REG		(0xA1)
#define AD242X_AMSCTL2_REG		(0xA2)
