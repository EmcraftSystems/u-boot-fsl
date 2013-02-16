/*
 * Copyright 2012 Freescale Semiconductor, Inc.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef _QSPI_H_
#define _QSPI_H_

typedef struct {
	ulong size;		/* total size in bytes */
	ulong sector_cnt;	/* number of erase units */
	ulong flash_id;		/* flash_id */
	/* virtual sector start addr */
	ulong start[CONFIG_SYS_MAX_QSPI_FLASH_SECT];
	/* sector protection status */
	ulong protect[CONFIG_SYS_MAX_QSPI_FLASH_SECT];
	uchar iowidth;		/* witdh of the IO pad */
	ulong erase_blk_tout;	/* maximum block erase timeout */
	ulong write_tout;	/* maximum write timeout */
	ushort vendor;		/* Primary vendor ID */
	ushort manufacturer_id;	/* manufacturer ID */
} qspi_flash_info_t;

/* Command registers */
#define QSPI_CMD_WRR		0x01	/* Write */
#define QSPI_CMD_PP		0x02	/* Page Program */
#define QSPI_CMD_READ		0x03	/* Read */
#define QSPI_CMD_WRDI		0x04	/* Write Disable */
#define QSPI_CMD_RDSR1		0x05	/* Read Status */
#define QSPI_CMD_WREN		0x06	/* Write Enable */
#define QSPI_CMD_RDSR2		0x07	/* Read Status 2 */
#define QSPI_CMD_FAST_RD	0x0B	/* Read Fast */
#define QSPI_CMD_4FAST_RD	0x0C	/* Read Fast */
#define QSPI_CMD_DDRFR		0x0D	/* Read DDR Fast */
#define QSPI_CMD_4DDRFR		0x0E	/* Read DDR Fast */
#define QSPI_CMD_4PP		0x12	/* Page Program */
#define QSPI_CMD_4READ		0x13	/* Read */
#define QSPI_CMD_ABRD		0x14	/* AutoBoot Read */
#define QSPI_CMD_ABWR		0x15	/* AutoBoot Write */
#define QSPI_CMD_BRRD		0x16	/* Bank Register Read */
#define QSPI_CMD_BRWR		0x17	/* Bank Register Write */
#define QSPI_CMD_P4E		0x20	/* 4KB Sector Erase */
#define QSPI_CMD_4P4E		0x21	/* 4KB Sector Erase */
#define QSPI_CMD_ASPRD		0x2B	/* ASP Read */
#define QSPI_CMD_ASPP		0x2F	/* ASP Program */
#define QSPI_CMD_CLSR		0x30	/* Clear Status */
#define QSPI_CMD_QPP		0x32	/* Quad Page Program */
#define QSPI_CMD_4QPP		0x34	/* Quad Page Program */
#define QSPI_CMD_RDCR		0x35	/* Read Configuration */
#define QSPI_CMD_QPPALT		0x38	/* Quad Page Program Alt */
#define QSPI_CMD_DOR		0x3B	/* Read Dual Out */
#define QSPI_CMD_4DOR		0x3C	/* Read Dual Out */
#define QSPI_CMD_DLPRD		0x41	/* Data Learning Pattern Read*/
#define QSPI_CMD_PNVDLR		0x43	/* Program NVDLR */
#define QSPI_CMD_WVDLR		0x4A	/* Write VDLR */
#define QSPI_CMD_BE		0x60	/* Bulk Erase */
#define QSPI_CMD_QOR		0x6B	/* Read Quad Out */
#define QSPI_CMD_4QOR		0x6C	/* Read Quad Out */
#define QSPI_CMD_ERSP		0x75	/* Erase Suspend */
#define QSPI_CMD_ERRS		0x7A	/* Erase Resume */
#define QSPI_CMD_RGSP		0x85	/* Program Suspend */
#define QSPI_CMD_PGRS		0x8A	/* Program Resume */
#define QSPI_CMD_REMS		0x90	/* Read Manufacturer signature */
#define QSPI_CMD_RDID		0x9F	/* Read ID*/
#define QSPI_CMD_PLBWR		0xA6	/* PPB Lock Write*/
#define QSPI_CMD_PLBRD		0xA7	/* PPB Lock Read */
#define QSPI_CMD_RES		0xAB	/* Read Elec. Signature */
#define QSPI_CMD_BRAC		0xB9	/* Bank Register Access */
#define QSPI_CMD_DIOR		0xBB	/* Dual IO Read */
#define QSPI_CMD_4DIOR		0xBC	/* Dual IO Read */
#define QSPI_CMD_DDRDIOR	0xBD	/* DDR Dual IO Read */
#define QSPI_CMD_4DDRDIOR	0xBE	/* DDR Dual IO Read */
#define QSPI_CMD_BEALT		0xC7	/* Bulk Erase alt */
#define QSPI_CMD_SE		0xD8	/* Erase 64/256KB */
#define QSPI_CMD_4SE		0xDC	/* Erase 64/256KB */
#define QSPI_CMD_DYBRD		0xE0	/* DBY Read */
#define QSPI_CMD_DYBP		0xE1	/* DBY Program */
#define QSPI_CMD_PPBRD		0xE2	/* PPB Read*/
#define QSPI_CMD_PASSRD		0xE7	/* Password Read */
#define QSPI_CMD_PASSSP		0xE8	/* Password Program */
#define QSPI_CMD_PASSSU		0xE9	/* Password Unlock */
#define QSPI_CMD_QIOR		0xEB	/* Quad IO Read */
#define QSPI_CMD_4QIOR		0xEC	/* Quad IO Read */
#define QSPI_CMD_DDRQIOR	0xED	/* DDR Quad IO Read */
#define QSPI_CMD_4DDRQIOR	0xEE	/* DDR Quad IO Read */
#define QSPI_CMD_RESET		0xF0	/* SW Reset */
#define QSPI_CMD_MBR		0xFF	/* Mode bit Reset */

/* Status Register 1 */
#define QSPI_SR1_SRWD		0x80	/* Status register wr disable */
#define QSPI_SR1_P_ERR		0x40	/* Programming Error Occurred */
#define QSPI_SR1_E_ERR		0x20	/* Erase Error Occurred */
#define QSPI_SR1_BP2		0x10	/* Block Protection */
#define QSPI_SR1_BP1		0x08
#define QSPI_SR1_BP0		0x04
#define QSPI_SR1_WEL		0x02	/* Write Enable Latch */
#define QSPI_SR1_WIP		0x01	/* Write in progress */

/* Status Register 2 */
#define QSPI_SR2_ES		0x02	/* Erase suspend */
#define QSPI_SR2_PS		0x01	/* Program Suspend */

/* Configuration register */
#define QSPI_CR1_LC1		0x80	/* Latency Code */
#define QSPI_CR1_LC0		0x40
#define QSPI_CR1_TBPROT		0x20	/* Config. Start of Block Protection */
#define QSPI_CR1_RFU		0x10	/* RFU */
#define QSPI_CR1_BPNV		0x08	/* Config. BP2-0 in Status register */
#define QSPI_CR1_TBPARM		0x04	/* Config. Parameter Sectors Location */
#define QSPI_CR1_QUAD		0x02	/* Quad I/O Operation */
#define QSPI_CR1_FREEZE		0x01	/* Lock current state of BP2-0 */

/* Bank Address Register */
#define QSPI_BAR_EXTADD		0x80	/* Extended Address En */
#define QSPI_BAR_BA24		0x01	/* Bank Address */

/* ASP Register */
#define QSPI_ASPR_PWDMLB	0x04	/* Password protection lock bit */
#define QSPI_ASPR_PSTMLB	0x02	/* persistent Protection lock bit */




#endif	/* _QSPI_H_ */
