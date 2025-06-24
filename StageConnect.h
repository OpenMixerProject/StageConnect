#ifndef STAGECONNECT_H
#define STAGECONNECT_H

#include <memory> // for std::unique_ptr
#include "Arduino.h"
#include "stcon.h"

class StageConnect {
	public:
		StageConnect(bool isMaster, uint8_t local_id, uint8_t I2C_address, Ci2c_com* com_int);
		
		virtual ~StageConnect();
		
		// getter to access A2B-bus directly
		Ca2b_bus_242x* bus() const {
			return _bus.get();
		}
		Csc_master* master() const {
			return _master.get();
		}
		Csc_slave* slave() const {
			return _slave.get();
		}

		void config(uint8_t brandId, uint16_t productId, uint8_t inputChannelCount, uint8_t outputChannelCount, uint8_t tdmConfig, uint8_t bufferSize);

		String getChipInfo();
		
		void update();
		
		int getLinkStatus();

	private:
		bool _isMaster;
		int8_t _link_status = A2B_NODE_NOT_LINKED_ERROR;

		std::unique_ptr<Ca2b_bus_242x> _bus;
		std::unique_ptr<Csc_master> _master;
		std::unique_ptr<Csc_slave> _slave;
		A2B_Ssc_node _nodeConfig = {
		  1, // API-Version
		  0, // Brand: 0x00 = Behringer
		  {0x02, 0x00}, // Product-ID: 0x0002 = product_id[1] bit 7 is 1 for SC module (0x0002 = P24 HUB)
		  16, // slave only, audio slots coming from A-side consumed by the node
		  0, // slave only,audio slots coming from B-side consumed by the node
		  0, // audio slots going to B-side provided by the node, max downslots on master
		  16,// audio slots going to A-side provided by the node, max upslots on master
		  AD242X_I2SGCFG_EARLY | AD242X_I2SGCFG_TDM16,
		  false, // slave only, loopback for debugging
		  false, // slave only, broadcast-mode. the nodes receives audio from ch1 to ch_n
		  0, // slave only. gives the node power in W x 10. e.g. 125 = 12.5W
		  0, // Status: 0x0C read for info on slave node.  see note below 
		  0, // Control: 0x0D write to control slave node.  see note below 
		  0, // ResetKey: must be '~' in order to apply reset
		  255,// BufferSize. slave only, tx and rx buffer size in  bytes  max. 2^16!!
		  0, // slave only, how many bytes were received
		  0, // Dummy0
		  0, // slave only, pointer to rx buffer
		  0, // slave only, how many bytes are to be transmitted
		  0, // Dummy1
		  0, // slave only, pointer to tx buffer
		  {0,0} // SG NOTE: keep the length align 4
		};
};

#endif