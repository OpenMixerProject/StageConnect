# SC Node

```cpp
    struct A2B_Ssc_node {
        U8 api_ver;
        U8 brand_id;
        U8 product_id[2];
        U8 downslots4node;  
        U8 upslots4node;  
        U8 downslots_local; 
        U8 upslots_local; 
        U8 tdm_config;    
        U8 tdm_loop_back; 
        U8 bcst_mode;   
        U8 node_pwr;    
        U8 status;      
        U8 control;     
        U8 rst_key;     
        U8 buffers_size;  
        U8 rx_buf_no_bytes; 
        U8 dummy0;
        U8 rx_buf_p;    
        U8 tx_buf_no_bytes; 
        U8 dummy1;
        U8 tx_buf_p;    
        U8 stuffing[2];    
    };
```

## _U8 api\_ver_

Current version = 1

## _U8 brand\_id_

| ID | Brand |
| :----- | :------ |
| 0x00 | Behringer |
| 0x01 | Midas |
| 0x02 | Turbosound |
| 0x03 | KT |
| 0x04 | Bugera |
| 0x05 | TC Electronics |
| 0x06 | TC Helicon |
| 0x07 | LabGruppen |
| 0x08 | Tannoy |

## _U8 product\_id\[2\]_

*   Behringer Product IDs

    | SC ID | Model Code | Model Name |
    | :----- | :------ | :------ |
    | 0x0001 | P0BV2 | NGC(Wing) |
    | 0x0002 | P0E0V | P24 HUB |
    | 0x0003 | P0E0W | P24 | 

*   Midas Product IDs 

    | SC ID | Model Code | Model Name |
    | :----- | :------ | :------ |
    | 0x0001 | P0DM1 | HUB4 |
    | 0x0002 | P0DVZ | DN4816-O |
    | 0x0003 | P0E0W | P24 | 
    | 0x0004 | P0DW0 | DN4816-I |
    | 0x0005 | P0E0U | DN4816U |

## _U8 downslots4node_

Slave only, audio slots coming from A-side consumed by the node

## _U8 upslots4node_

Slave only, audio slots coming from B-side consumed by the node

## _U8 downslots\_local_

Audio slots going to B-side provided by the node, max downslots on master

## _U8 upslots\_local_

Audio slots going to A-side provided by the node, max upslots on master

## _U8 tdm\_config_

TDM configuration for the inputs/outputs from the a2b chip, the 2 input lines and 2 output lines will have this TDM configuration.

| Configuration |  | Description |
| :----- | :------ | :------ |
| AD242X_I2SGCFG_INV | (1 << 7) | Invert Sync. |
| AD242X_I2SGCFG_EARLY | (1 << 6) | Early Sync. |
| AD242X_I2SGCFG_ALT | (1 << 5) | Alternating Sync. | 
| AD242X_I2SGCFG_TDMSS | (1 << 4) | TDM Slot Size. 0=32bit 1=61bit |
| AD242X_I2SGCFG_TDM2 | (0) | TDM2 Mode. |
| AD242X_I2SGCFG_TDM4 | (1) | TDM4 Mode. |
| AD242X_I2SGCFG_TDM8 | (2) | TDM8 Mode. |
| AD242X_I2SGCFG_TDM12 | (3) | TDM12 Mode. (No Slave Node support) |
| AD242X_I2SGCFG_TDM16| (4) | TDM16 Mode. |
| AD242X_I2SGCFG_TDM20 | (5) | TDM20 Mode. (No Slave Node support) |
| AD242X_I2SGCFG_TDM24 | (6) | TDM24 Mode. (No Slave Node support) |
| AD242X_I2SGCFG_TDM32 | (7) | TDM32 Mode. |

## _U8 tdm\_loop\_back_

Slave only, for debugging

## _U8 bcst\_mode_

Slave only, the nodes receives audio from ch1 to ch\_n

## _U8 node\_pwr_

Slave only. gives the node power in W x 10. e.g. 125 = 12.5W

## _U8 status_

Status register: master reads this reg to find out about changes on node

| Status Register |  | Description |
| :----- | :------ | :------ |
| A2B_STAT_REG_RDIS | (1 << 7) | Node definition has changed, a new discovery round is needed |
| A2B_STAT_REG_BUF_F | (1 << 6) | The TX buffer has data to be read |

## _U8 control_

Control register: master write to this address to control the node

| Control Register |  | Description |
| :----- | :------ | :------ |
| A2B_CTL_REG_LINK | (1 << 7) | Node is lined to master via A2B bus, it is toggled by the master. If toggle is lost, link is lost |
| A2B_CTL_REG_CRC_E | (1 << 6) | CRC error on the bus |
| A2B_CTL_REG_SFM_E | (1 << 5) | Super frame error |
| A2B_CTL_REG_DEF_U | (1 << 4) | Set by the master when node def has been changes by the master |
| A2B_CTL_REG_EN_PWR | (1 << 3) | Enable device power |
| A2B_CTL_REG_44_1 | (1 << 2) | Sample rate is 44.1 KHz |
| A2B_CTL_REG_RST_M | (1 << 0) | Sort reset module, use for updating |

## _U8 rst\_key_

Most be &#39;~&#39; in order to apply reset

## _U8 buffers\_size_

Slave only, tx and rx buffer size in bytes max. 2^16!!

## _U8 rx\_buf\_no\_bytes_

Slave only, how many bytes were received

## _U8 dummy0_

## _U8 rx\_buf\_p_

Slave only, pointer to rx buffer

## _U8 tx\_buf\_no\_bytes_

Slave only, how many bytes are to be transmitted

## _U8 dummy1_

## _U8 tx\_buf\_p_

Slave only, pointer to tx buffer

## _U8 stuffing\[2\]_

Keep the length align 4