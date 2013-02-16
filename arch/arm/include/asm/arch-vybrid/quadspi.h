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

#ifndef __QUADSPI_H__
#define __QUADSPI_H__

#define QUADSPI_MCR		(0x000)
#define QUADSPI_IPCR		(0x008)
#define QUADSPI_FLSHCR		(0x00C)
#define QUADSPI_BUF0CR		(0x010)
#define QUADSPI_BUF1CR		(0x014)
#define QUADSPI_BUF2CR		(0x018)
#define QUADSPI_BUF3CR		(0x01C)
#define QUADSPI_BFGENCR		(0x020)
#define QUADSPI_SOCCR		(0x024)
#define QUADSPI_BUF0IND		(0x030)
#define QUADSPI_BUF1IND		(0x034)
#define QUADSPI_BUF2IND		(0x038)
#define QUADSPI_SFAR		(0x100)
#define QUADSPI_SMPR		(0x108)
#define QUADSPI_RBSR		(0x10C)
#define QUADSPI_RBCT		(0x110)
#define QUADSPI_TBSR		(0x150)
#define QUADSPI_TBDR		(0x154)
#define QUADSPI_SR		(0x15C)
#define QUADSPI_FR		(0x160)
#define QUADSPI_RSER		(0x164)
#define QUADSPI_SPNDST		(0x168)
#define QUADSPI_SPTRCLR		(0x16C)
#define QUADSPI_SFA1AD		(0x180)
#define QUADSPI_SFA2AD		(0x184)
#define QUADSPI_SFB1AD		(0x188)
#define QUADSPI_SFB2AD		(0x18C)
#define QUADSPI_LUTKEY		(0x300)
#define QUADSPI_LCKCR		(0x304)
#define QUADSPI_LUT0		(0x310)
#define QUADSPI_LUT1		(0x314)
#define QUADSPI_LUT2		(0x318)
#define QUADSPI_LUT3		(0x31C)
#define QUADSPI_LUT4		(0x320)
#define QUADSPI_LUT5		(0x324)
#define QUADSPI_LUT6		(0x328)
#define QUADSPI_LUT7		(0x32C)
#define QUADSPI_LUT8		(0x330)
#define QUADSPI_LUT9		(0x334)
#define QUADSPI_LUT10		(0x338)
#define QUADSPI_LUT11		(0x33C)
#define QUADSPI_LUT12		(0x340)
#define QUADSPI_LUT13		(0x344)
#define QUADSPI_LUT14		(0x348)
#define QUADSPI_LUT15		(0x34C)
#define QUADSPI_LUT16		(0x350)
#define QUADSPI_LUT17		(0x354)
#define QUADSPI_LUT18		(0x358)
#define QUADSPI_LUT19		(0x35C)
#define QUADSPI_LUT20		(0x360)
#define QUADSPI_LUT21		(0x364)
#define QUADSPI_LUT22		(0x368)
#define QUADSPI_LUT23		(0x36C)
#define QUADSPI_LUT24		(0x370)
#define QUADSPI_LUT25		(0x374)
#define QUADSPI_LUT26		(0x378)
#define QUADSPI_LUT27		(0x37C)
#define QUADSPI_LUT28		(0x380)
#define QUADSPI_LUT29		(0x384)
#define QUADSPI_LUT30		(0x388)
#define QUADSPI_LUT31		(0x38C)
#define QUADSPI_LUT32		(0x390)
#define QUADSPI_LUT33		(0x394)
#define QUADSPI_LUT34		(0x398)
#define QUADSPI_LUT35		(0x39C)
#define QUADSPI_LUT36		(0x3A0)
#define QUADSPI_LUT37		(0x3A4)
#define QUADSPI_LUT38		(0x3A8)
#define QUADSPI_LUT39		(0x3AC)
#define QUADSPI_LUT40		(0x3B0)
#define QUADSPI_LUT41		(0x3B4)
#define QUADSPI_LUT42		(0x3B8)
#define QUADSPI_LUT43		(0x3BC)
#define QUADSPI_LUT44		(0x3C0)
#define QUADSPI_LUT45		(0x3C4)
#define QUADSPI_LUT46		(0x3C8)
#define QUADSPI_LUT47		(0x3CC)
#define QUADSPI_LUT48		(0x3D0)
#define QUADSPI_LUT49		(0x3D4)
#define QUADSPI_LUT50		(0x3D8)
#define QUADSPI_LUT51		(0x3DC)
#define QUADSPI_LUT52		(0x3E0)
#define QUADSPI_LUT53		(0x3E4)
#define QUADSPI_LUT54		(0x3E8)
#define QUADSPI_LUT55		(0x3EC)
#define QUADSPI_LUT56		(0x3F0)
#define QUADSPI_LUT57		(0x3F4)
#define QUADSPI_LUT58		(0x3F8)
#define QUADSPI_LUT59		(0x3FC)
#define QUADSPI_LUT60		(0x400)
#define QUADSPI_LUT61		(0x404)
#define QUADSPI_LUT62		(0x408)
#define QUADSPI_LUT63		(0x40C)
#define QUADSPI_RBDR		(0x2000)

#define QUADSPI_MCR_DOZE	(1 << 15)
#define QUADSPI_MCR_MDIS	(1 << 14)
#define QUADSPI_MCR_CLRTXF	(1 << 11)
#define QUADSPI_MCR_CLRRXF	(1 << 10)
#define QUADSPI_MCR_DDREN	(1 << 7)
#define QUADSPI_MCR_DQSEN	(1 << 6)
#define QUADSPI_MCR_SWRSTHD	(1 << 1)
#define QUADSPI_MCR_SWRSTSD	(1 << 0)

#define QUADSPI_IPCR_SEQID(x)		((x & 0xF) << 24)
#define QUADSPI_IPCR_PAREN		(1 << 16)
#define QUADSPI_IPCR_IDATSZ(x)		(x & 0xFFFF)

#define QUADSPI_FLSHCR_TCSH(x)		((x & 0xF) << 8)
#define QUADSPI_FLSHCR_TCSS(x)		(x & 0xF)

#define QUADSPI_BUF0CR_HPEN		(1 << 31)
#define QUADSPI_BUF3CR_ALLMST		(1 << 31)
#define QUADSPI_BUFCR_ADATSZ(x)		((x & 0xFF) << 8)
#define QUADSPI_BUFCR_MSTRID(x)		(x & 0xF)

#define QUADSPI_BFGENCR_PAREN		(1 << 16)
#define QUADSPI_BFGENCR_SEQID(x)	((x & 0xF) << 12)

#define QUADSPI_SOCCR_SOCCFG(x)		(x & 0xFF)

#define QUADSPI_BUFIND_TPINDX(x)	(x << 3)

#define QUADSPI_SMPR_DDRSMP(x)		((x & 7) << 16)
#define QUADSPI_SMPR_FSDLY		(1 << 6)
#define QUADSPI_SMPR_FSPHS		(1 << 5)
#define QUADSPI_SMPR_HSDLY		(1 << 2)
#define QUADSPI_SMPR_HSPHS		(1 << 1)
#define QUADSPI_SMPR_HSENA		(1 << 0)

#define QUADSPI_RBCT_WMRK(x)		(x & 0x1F)
#define QUADSPI_RBCT_RXBRD		(1 << 8)

#define QUADSPI_SR_TXFULL		(1 << 27)
#define QUADSPI_SR_TXNE			(1 << 24)
#define QUADSPI_SR_RXDMA		(1 << 23)
#define QUADSPI_SR_RXFULL		(1 << 19)
#define QUADSPI_SR_RXWE			(1 << 15)
#define QUADSPI_SR_AHB3FUL		(1 << 14)
#define QUADSPI_SR_AHB2FUL		(1 << 13)
#define QUADSPI_SR_AHB1FUL		(1 << 12)
#define QUADSPI_SR_AHB0FUL		(1 << 11)
#define QUADSPI_SR_AHB3NE		(1 << 10)
#define QUADSPI_SR_AHB2NE		(1 << 9)
#define QUADSPI_SR_AHB1NE		(1 << 8)
#define QUADSPI_SR_AHB0NE		(1 << 7)
#define QUADSPI_SR_AHBTRN		(1 << 6)
#define QUADSPI_SR_AHBGNT		(1 << 5)
#define QUADSPI_SR_AHBACC		(1 << 2)
#define QUADSPI_SR_IPACC		(1 << 1)
#define QUADSPI_SR_BUSY			(1 << 0)

#define QUADSPI_FR_DLPFF		(1 << 31)
#define QUADSPI_FR_TBFF			(1 << 27)
#define QUADSPI_FR_TBUF			(1 << 26)
#define QUADSPI_FR_ILLINE		(1 << 23)
#define QUADSPI_FR_RBOF			(1 << 17)
#define QUADSPI_FR_RBDF			(1 << 16)
#define QUADSPI_FR_ABSEF		(1 << 15)
#define QUADSPI_FR_ABOF			(1 << 12)
#define QUADSPI_FR_IUEF			(1 << 11)
#define QUADSPI_FR_IPAEF		(1 << 7)
#define QUADSPI_FR_IPIEF		(1 << 6)
#define QUADSPI_FR_IPGEF		(1 << 4)
#define QUADSPI_FR_TFF			(1 << 0)

/* QUADSPI_RSER use QUADSPI_FR bit field */

#define QUADSPI_SPTRCLR_IPPTRC		(1 << 8)
#define QUADSPI_SPTRCLR_BFPTRC		(1 << 0)

#define QUADSPI_SFAnAD(x)		(x << 10)

#define QUADSPI_LCKCR_UNLOCK		(1 << 1)
#define QUADSPI_LCKCR_LOCK		(1 << 0)

#define QUADSPI_LUTn_INSTR1(x)		((x & 0x3F) << 26)
#define QUADSPI_LUTn_PAD1(x)		((x & 0x3) << 24)
#define QUADSPI_LUTn_OPRND1(x)		((x & 0xFF) << 16)
#define QUADSPI_LUTn_INSTR0(x)		((x & 0x3F) << 10)
#define QUADSPI_LUTn_PAD0(x)		((x & 0x3) << 8)
#define QUADSPI_LUTn_OPRND0(x)		((x & 0xFF) << 0)

#endif				/* __QUADSPI_H__ */
