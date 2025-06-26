#include "StageConnect.h"

// Constructor
StageConnect::StageConnect(bool isMaster, uint8_t local_id, uint8_t I2C_address, Ci2c_com* com_int) {
    if (isMaster) {
		_master = std::unique_ptr<Csc_master>(new Csc_master(&_nodeConfig, local_id, I2C_address, com_int));
    } else {
		_slave = std::unique_ptr<Csc_slave>(new Csc_slave(&_nodeConfig, local_id, I2C_address, com_int));
    }
}

// Destructor
StageConnect::~StageConnect() {}

void StageConnect::config(uint8_t brandId, uint16_t productId, uint8_t inputChannelCount, uint8_t outputChannelCount, uint8_t tdmConfig, uint8_t bufferSize) {
  _nodeConfig.brand_id = brandId;					// don't use values below 0x80 for DIY-projects
  _nodeConfig.product_id[0] = productId;			
  _nodeConfig.product_id[1] = (productId >> 8);
  _nodeConfig.downslots4node = inputChannelCount;   // slave only, audio slots coming from A-side consumed by the node
  _nodeConfig.upslots4node = 0;      				// slave only, audio slots coming from B-side consumed by the node
  _nodeConfig.downslots_local = 0;   				// audio slots going to B-side provided by the node, max downslots on master
  _nodeConfig.upslots_local = outputChannelCount;	// audio slots going to A-side provided by the node, max upslots on master
  _nodeConfig.tdm_config = tdmConfig;
  _nodeConfig.buffers_size = bufferSize;
}

// general information about the connected A2B-chip
String StageConnect::getChipInfo() {
	if (_isMaster) {
		return "VID 0x" + String(_master->get_vendor_id(), HEX) + " and PID 0x" + String(_master->get_product_id(), HEX) + " in Version 0x" + String(_master->get_version(), HEX);
	}else{
		return "VID 0x" + String(_slave->get_vendor_id(), HEX) + " and PID 0x" + String(_slave->get_product_id(), HEX) + " in Version 0x" + String(_slave->get_version(), HEX);
	}
}

uint8_t StageConnect::checkForNewMessage() {
	return _nodeConfig.rx_buf_no_bytes;
}

// update-function to be called at least every 100ms
void StageConnect::update() {
	if (_isMaster) {
		_link_status = _master->check_link();
	}else{
		_link_status = _slave->check_link();
	}
}

int StageConnect::getLinkStatus() {
	return _link_status;
}
