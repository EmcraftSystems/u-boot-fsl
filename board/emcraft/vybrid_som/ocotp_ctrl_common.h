////////////////////////////////////////////////////////////////////////////////
//
// Filename: ocotp_common.h
//
// Description: This is include file for utility functions for using the OCOTP
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

#ifndef OCOTP_CTRL_COMMON_H
#define OCOTP_CTRL_COMMON_H

//#include "regs.h"
//#include "registers.h"
#include "regsocotp.h"

#define OCOTP_WR_UNLOCK_KEY_VALUE 		0x3e770000

#define BM_OCOTP_LOCK_TESTER_PROG       	0x00000001
#define BM_OCOTP_LOCK_TESTER_WRITE      	0x00000002

#define BM_OCOTP_LOCK_BOOT_CFG_PROG      	0x00000004
#define BM_OCOTP_LOCK_BOOT_CFG_WRITE      	0x00000008

#define BM_OCOTP_LOCK_MEM_TRIM_PROG      	0x00000010
#define BM_OCOTP_LOCK_MEM_TRIM_WRITE      	0x00000020

#define BM_OCOTP_LOCK_MAC_ADDR_PROG      	0x00000100
#define BM_OCOTP_LOCK_MAC_ADDR_WRITE      	0x00000200

#define BM_OCOTP_LOCK_GP1_PROG			0x00000400
#define BM_OCOTP_LOCK_GP1_WRITE			0x00000800

#define BM_OCOTP_LOCK_GP2_PROG			0x00001000
#define BM_OCOTP_LOCK_GP2_WRITE			0x00002000

#define BM_OCOTP_LOCK_ANALOG_PROG      		0x00040000
#define BM_OCOTP_LOCK_ANALOG_WRITE     		0x00080000

#define BM_OCOTP_LOCK_CRC_GP_HI_LOCK_GP   0x10000000
#define BM_OCOTP_LOCK_CRC_GP_HI_LOCK_CRC      0x20000000

#define BM_OCOTP_LOCK_CRC_GP_LO_LOCK_GP   0x04000000
#define BM_OCOTP_LOCK_CRC_GP_LO_LOCK_CRC   	  0x08000000


void wait4Busy(void);
void program_fuse_word(int address,int data_temp);
void read_fuse_word(int address);


#endif // ifndef OCOTP_CTRL_COMMON_H

