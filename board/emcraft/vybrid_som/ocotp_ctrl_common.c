////////////////////////////////////////////////////////////////////////////////
//
// Filename: ocotp_common.c
//
// Description: This is the C code for utility functions for using the OCOTP
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) SigmaTel, Inc. Unpublished
//
// SigmaTel, Inc.
// Proprietary & Confidential
//
// This source code and the algorithms implemented therein constitute
// confidential information and may compromise trade secrets of SigmaTel, Inc.
// or its associates, and any unauthorized use thereof is prohibited.
//
////////////////////////////////////////////////////////////////////////////////


#include "ocotp_ctrl_common.h"

void wait4Busy(void)
{
//wait for busy is clear
    while (HW_OCOTP_CTRL_RD() & BM_OCOTP_CTRL_BUSY);
//check if error bit is asserted    
    if (HW_OCOTP_CTRL_RD() & BM_OCOTP_CTRL_ERROR)
    {
//     printk("**OCOTP_CTRL ERROR is asserted**\n");      
     // clear the error bit 
     HW_OCOTP_CTRL_CLR(BM_OCOTP_CTRL_ERROR);
    }
}    

void wait4Error(void)
{
	//wait for busy is clear
	while (HW_OCOTP_CTRL_RD() & BM_OCOTP_CTRL_BUSY);
	//check if error bit is asserted    

	if (HW_OCOTP_CTRL_RD() & BM_OCOTP_CTRL_ERROR)
	{
		//     printk("PASS,OCOTP_CTRL ERROR is asserted**\n");  
		// clear the error bit 
		HW_OCOTP_CTRL_CLR(BM_OCOTP_CTRL_ERROR);
	}
	else 
	{ 
	//     printk("FAIL,OCOTP_CTRL ERROR is not asserted** \n");  
	}
}

void program_fuse_word(int address,int data_temp)
{
	//configure fuse word address with unlock key 
	HW_OCOTP_CTRL_WR(OCOTP_WR_UNLOCK_KEY_VALUE | (address & BM_OCOTP_CTRL_ADDR));  

	// now change the data
	HW_OCOTP_DATA_WR(data_temp);	
}

void read_fuse_word(int address)
{
	//configure read fuse word address  
	HW_OCOTP_CTRL_WR(address & BM_OCOTP_CTRL_ADDR);

	//Set read fuse bit
	HW_OCOTP_READ_CTRL_WR(BM_OCOTP_READ_CTRL_READ_FUSE);
}
