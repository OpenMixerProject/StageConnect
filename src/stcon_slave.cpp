#include <string.h>
#include "a2b_bus.h"
#include "AD2425_inc.h"
#include "stcon.h"


Csc_slave::Csc_slave(A2B_Ssc_node* def, uint8_t local_id, uint8_t I2C_address, Ci2c_com* com_int): Ca2b_bus_242x(local_id, I2C_address, com_int)
{
	l_node_def = def;
	l_node_def_byte = (uint8_t*)l_node_def;
	reg_address = 0;

	tx_buffer = 0;
	rx_buffer = 0;
	buf_size = 0;

	rx_idx = 0;
	tx_idx = 0;
	link_cnt_ = 0;
	link_cnt_ctl_ = 0;
	link_ret_ = A2B_NODE_NOT_LINKED_ERROR;

}


/**
 * \brief configure auxiliary communication buffers
 *  
 *
 */
void Csc_slave::configure_com_buffers(uint8_t* tx_buf_p, uint8_t* rx_buf_p, uint8_t size)
{
	tx_buffer = tx_buf_p;
	rx_buffer = rx_buf_p;
	buf_size = size;

	rx_idx = 0;
	tx_idx = 0;

}//end method


/**
 * \brief set com register address
 *  this function set the registter address to be  read or written
 *  if addres is tx or rx buffer  the indexes are reseted
 *
 */
void Csc_slave::set_reg_add(uint8_t add)
{
	//read and write to this address over structure size is protected!
	reg_address = add;

	if(reg_address >= sizeof(A2B_Ssc_node))
		return;

	if (&l_node_def_byte[reg_address] == &l_node_def->rx_buf_p)
		rx_idx = 0;


	if ( &l_node_def_byte[reg_address] == &l_node_def->tx_buf_p)
		tx_idx = 0;


}//end method

/**
 * \brief read register
 *  read register on predefined address and auto increment
 * if address is equal to rx buffer read data from buffer 
 *
 */
uint8_t Csc_slave::read_reg(void)
{

	if(reg_address >= sizeof(A2B_Ssc_node))
		return 0;

	if(!reg_address)
		l_node_def->status = 0;

	//if if address is tx buffer
	if ( &l_node_def_byte[reg_address] == &l_node_def->tx_buf_p)
		return read_from_buf();
	else 
		return l_node_def_byte[reg_address++];


}//end method



/**
 * \brief write to register
 *  write to predefined address and auto increment
 *  id address is equal to tx buffer write to buffer
 *
 */
void Csc_slave::write_reg(uint8_t data)
{

	if(reg_address >= sizeof(A2B_Ssc_node))
		return;

	//if if address is rx buffer
	if ( &l_node_def_byte[reg_address] == &l_node_def->rx_buf_p) 
		write_to_buf(data);
	else
		l_node_def_byte[reg_address++] = data;

}//end method


/**
 * \brief send message
 *  use this function to send a message to master
 *
 *	\param data  pointer to data
 *	\param length number of bytes to be set
 *	\return 0 when no error  or error code
 *
 */
int Csc_slave::send_message(uint8_t* data, uint8_t length)
{
	if(!tx_buffer)
		return A2B_NO_TX_BUF_P;

	if(l_node_def->tx_buf_no_bytes)
		return A2B_TX_BUF_NOT_EMPTY;

	if(length >= buf_size)
		length = buf_size;

	memcpy(tx_buffer, data, length);
	l_node_def->tx_buf_no_bytes = length;

	//set the bit on the status register
	l_node_def->status |= A2B_STAT_REG_BUF_F; 

	return A2B_NO_ERROR;
	
}//end method

/**
 * \brief get message
 *  use this function to get a message from master
 *
 *	\param target  pointer to data
 *	\return >=0 number of byte received. or error code
 *
 */
int Csc_slave::get_message(uint8_t* target, uint8_t max_len)
{
	uint8_t no_bytes=0;

	if(!rx_buffer)
		return A2B_NO_RX_BUF_P;

	if(!l_node_def->rx_buf_no_bytes )
		return 0;

	no_bytes = l_node_def->rx_buf_no_bytes;

	if(no_bytes >= max_len)
		no_bytes = max_len;
	
	memcpy(target, rx_buffer, no_bytes);
	l_node_def->rx_buf_no_bytes = 0;

	return no_bytes;

}//end method


void Csc_slave::write_to_buf(uint8_t data)
{
	if(!rx_buffer)
		return;

	if (rx_idx < buf_size)
		rx_buffer[rx_idx++] = data;

}//end method


uint8_t Csc_slave::read_from_buf( void)
{
 	uint8_t data = 0;

	if (tx_idx < buf_size)
		data = tx_buffer[tx_idx++];
	else{
		l_node_def->tx_buf_no_bytes = 0;
		l_node_def->status &= ~A2B_STAT_REG_BUF_F;
	}


	if(tx_idx >= l_node_def->tx_buf_no_bytes){
		l_node_def->tx_buf_no_bytes = 0;
		l_node_def->status &= ~A2B_STAT_REG_BUF_F;
	}

	return data;
}//end method


int Csc_slave::check_link(void) 
{//NOTE: flag need to be set regularly by master
	uint8_t control = l_node_def->control;

	if(control & A2B_CTL_REG_SFM_E)
		link_ret_ = A2B_NODE_NOT_LINKED_ERROR;
	
	else if(control & A2B_CTL_REG_LINK){
		link_cnt_ = A2B_SLV_LINK_CNT;
		link_cnt_ctl_++; 

		if(control & A2B_CTL_REG_CRC_E)
			link_ret_ =  A2B_NODE_LINKED_WITH_ERR;
		else
			link_ret_ =  A2B_NODE_LINKED;
	}else
		link_cnt_ctl_ = 0;
	
	//link flag must toggle, if stays as one for more than 4 sec link is broken
	if(link_cnt_ctl_ == (A2B_SLV_LINK_CNT*2)){
		l_node_def->control = 0;
		link_cnt_ctl_ = 0;
		link_ret_ = A2B_NODE_NOT_LINKED_ERROR;
	}

	if(link_cnt_)
		link_cnt_--;
	else
		link_ret_ = A2B_NODE_NOT_LINKED_ERROR;


	return link_ret_;

}//end method

bool Csc_slave::check_ctl_reg_bit(uint8_t bit_def)
{
	return (l_node_def->control & bit_def) ? true : false;

}//end method


void Csc_slave::set_status_reg_bit(uint8_t bit_def)
{
	l_node_def->status |= bit_def;

}//end method



void Csc_slave::event_handler()
{
	uint8_t source;
	uint8_t int_type;


	source = irq_event(&int_type);

	if (!source)
		return;

	switch (int_type) {
	case AD2425_INTTYPE_HDCNTERR:           		// 0
		break;
	case AD2425_INTTYPE_DDERR:           			//	1	//data decoded error parity error in stlot
		break;
	case AD2425_INTTYPE_CRCERR:           			// 	2	// crc error in th SFM
		break;
	case AD2425_INTTYPE_DPERR:           			// 	3
		break;
	case AD2425_INTTYPE_BECOVF:           			// 	4
		break;
	case AD2425_INTTYPE_SRFERR:           			// 	5	//super frame error. connection may be lost
		break;
	case AD2425_INTTYPE_SRFCRCERR:           		//  6	// Slave Only
		break;
	case AD2425_INTTYPE_PWRERR0:           			// 	9	// Cable Terminal Shorted to Ground
		break;
	case AD2425_INTTYPE_PWRERR1:           			// 	10	// Cable Terminal Shorted to VBat
		break;
	case AD2425_INTTYPE_PWRERR2:           			// 	11	// Cable Terminals Shorted Together
		break;
	case AD2425_INTTYPE_PWRERR3:           			// 	12	// Cable Disconnected or Open Circuit
		break;
	case AD2425_INTTYPE_PWRERR4:           			// 	13	// Cable is Reverse Connected
		break;
	case AD2425_INTTYPE_PWRERR5:           			// 	15	// Indeterminate Fault
		break;
	case AD2425_INTTYPE_IO0PND:           			// 	16	// Slave Only
		break;
	case AD2425_INTTYPE_IO1PND:           			// 	17	//
		break;
	case AD2425_INTTYPE_IO2PND:           			// 	18	//
		break;
	case AD2425_INTTYPE_IO3PND:           			// 	19	//
		break;
	case AD2425_INTTYPE_IO4PND:           			// 	20	//
		break;
	case AD2425_INTTYPE_IO5PND:           			// 	21	//
		break;
	case AD2425_INTTYPE_IO6PND:           			// 	22	//
		break;
	case AD2425_INTTYPE_IO7PND:           			// 	23	//
		break;
	case AD2425_INTTYPE_DSCDONE:           			// 	24	// Master Only
		break;
	case AD2425_INTTYPE_I2CERR:           			//  25	// Master Only
		break;
	case AD2425_INTTYPE_ICRCERR:           			// 	26	// Master Only
		break;
	case AD2425_INTTYPE_PWRERR6:           			// 	41	// Non-Localized Short to Ground
		break;
	case AD2425_INTTYPE_PWRERR7:           			// 	42	// on-Localized Short to VBat
		break;
	case AD2425_INTTYPE_MB0_FULL:           		// 	48	//
		break;
	case AD2425_INTTYPE_MB0_EMPTY:           		// 	49	//
		break;
	case AD2425_INTTYPE_MB1_FULL:           		// 	50	//
		break;
	case AD2425_INTTYPE_MB1_EMPTY:           		// 	51	//
		break;
	case AD2425_INTTYPE_Slave_INTTYPE_Read_Error:           			//	253	// Master Only
		break;
	case AD2425_INTTYPE_Standby_Done:           			// 	254	// Master Only
		break;
	case AD2425_INTTYPE_MSTR_RUNNING:           			// 	255	// Master Only
		break;

	default:
		break;
	}

}//end function


///////////////////////////////////////////////////////////////////////////////////
//  private
