














A2B_Ssc_node slv_node_2x2 = {
	1,
	5,
	{0, 0x80},
	2,	//downslots4node
	0,	//upslots4node
	0,  //downslots_local
	2,	//upslots_local
	AD242X_I2SGCFG_EARLY | AD242X_I2SGCFG_TDM8,
	true,
	false,
	0,	// power	
	0,	//status
	0,	//control
	0,	//key
	128,//buffers_size
	0,
	0,
	0,
	0,
	0,
	0,
	{0,0}
};

A2B_Ssc_node slv_node_2x0 = {
	1,
	5,
	{0, 0x80},
	2,	//downslots4node
	0,	//upslots4node
	0,  //downslots_local
	0,	//upslots_local
	AD242X_I2SGCFG_EARLY | AD242X_I2SGCFG_TDM8,
	true,
	false,
	0,	// power
	0,	//status
	0,	//control
	0,	//key
	128,//buffers_size
	0,
	0,
	0,
	0,
	0,
	0,
	{0,0}
};

A2B_Ssc_node slv_node_0x2 = {
	1,
	5,
	{0, 0x80},
	0,	//downslots4node
	0,	//upslots4node
	0,  //downslots_local
	2,	//upslots_local
	AD242X_I2SGCFG_EARLY | AD242X_I2SGCFG_TDM8,
	true,
	false,
	0,	// power
	0,	//status
	0,	//control
	0,	//key
	128,//buffers_size
	0,
	0,
	0,
	0,
	0,
	0,
	{0,0}
};

A2B_Ssc_node slv_node_4x4 = {
	1,
	5,
	{0, 0x80},
	4,	//downslots4node
	0,	//upslots4node
	0,  //downslots_local
	4,	//upslots_local
	AD242X_I2SGCFG_EARLY | AD242X_I2SGCFG_TDM8,
	true,
	false,
	0,	// power	
	0,	//status
	0,	//control
	0,	//key
	128,//buffers_size
	0,
	0,
	0,
	0,
	0,
	0,
	{0,0}
};
