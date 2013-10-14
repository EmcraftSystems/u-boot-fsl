/*
 * (C) Copyright 2013
 *
 * Pavel Boldin, Emcraft Systems, paboldin@emcraft.com
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

#ifndef __MVF_DCU_H
#define __MVF_DCU_H

/* DCU registers */
#define DCU_CTRLDESCCURSOR1		0x0000
#define DCU_CTRLDESCCURSOR1_HEIGHT(x)	(x << 16)
#define DCU_CTRLDESCCURSOR1_WIDTH(x)	(x)

#define DCU_CTRLDESCCURSOR2		0x0004
#define DCU_CTRLDESCCURSOR2_POSY(x)	(x << 16)
#define DCU_CTRLDESCCURSOR2_POSX(x)	(x)

#define DCU_CTRLDESCCURSOR3		0x0008
#define DCU_CTRLDESCCURSOR3_CUR_EN(x)	(x << 31)

#define DCU_CTRLDESCCURSOR4		0x000c

#define DCU_DCU_MODE			0x0010
#define DCU_MODE_BLEND_ITER(x)		(x << 20)
#define DCU_MODE_RASTER_EN(x)		(x << 14)
#define DCU_MODE_DCU_MODE(x)		(x)
#define DCU_MODE_DCU_MODE_MASK		0x03

#define DCU_BGND			0x0014
#define DCU_BGND_R(x)			(x << 16)
#define DCU_BGND_G(x)			(x << 8)
#define DCU_BGND_B(x)			(x)

#define DCU_DISP_SIZE			0x0018
#define DCU_DISP_SIZE_DELTA_Y(x)	(x << 16)
#define DCU_DISP_SIZE_DELTA_X(x)	(x)

#define DCU_HSYN_PARA			0x001c
#define DCU_HSYN_PARA_BP(x)		(x << 22)
#define DCU_HSYN_PARA_PW(x)		(x << 11)
#define DCU_HSYN_PARA_FP(x)		(x)

#define DCU_VSYN_PARA			0x0020
#define DCU_VSYN_PARA_BP(x)		(x << 22)
#define DCU_VSYN_PARA_PW(x)		(x << 11)
#define DCU_VSYN_PARA_FP(x)		(x)

#define DCU_SYN_POL			0x0024
#define DCU_SYN_POL_INV_PXCK(x)		(x << 6)
#define DCU_SYN_POL_NEG(x)		(x << 5)
#define DCU_SYN_POL_INV_VS(x)		(x << 1)
#define DCU_SYN_POL_INV_HS(x)		(x)

#define DCU_THRESHOLD			0x0028
#define DCU_THRESHOLD_LS_BF_VS(x)	(x << 16)
#define DCU_THRESHOLD_OUT_BUF_HIGH(x)	(x << 8)
#define DCU_THRESHOLD_OUT_BUF_LOW(x)	(x)

#define DCU_INT_STATUS			0x002C
#define DCU_INT_STATUS_UNDRUN		(1 << 2)
#define DCU_INT_STATUS_LYR_TRANS_FINISH	(1 << 12)

#define DCU_INT_MASK			0x0030
#define DCU_INT_MASK_ALL		0xffffffff

#define DCU_COLBAR_1			0x0034
#define DCU_COLBAR_2			0x0038
#define DCU_COLBAR_3			0x003c
#define DCU_COLBAR_4			0x0040
#define DCU_COLBAR_5			0x0044
#define DCU_COLBAR_6			0x0048
#define DCU_COLBAR_7			0x004c
#define DCU_COLBAR_8			0x0050
#define DCU_DIV_RATIO			0x0054
#define DCU_SIGN_CALC_1			0x0058
#define DCU_SIGN_CALC_2			0x005c
#define DCU_CRC_VAL			0x0060

#define DCU_PARR_ERR_STA_1		0x006c

#define DCU_PARR_ERR_STA_2		0x0070

#define DCU_PARR_ERR_STA_3		0x007c

#define DCU_MSK_PARR_ERR_STA_1		0x0080
#define DCU_MSK_PARR_ERR_ALL		0xffffffff

#define DCU_MSK_PARR_ERR_STA_2		0x0084

#define DCU_MSK_PARR_ERR_STA_3		0x0090

#define DCU_THRSHLD_INP_1		0x0094
#define DCU_THRSHLD_INP_2		0x0098
#define DCU_THRSHLD_INP_3		0x009c
#define DCU_LUMA_COMP			0x00a0
#define DCU_RED_CHRM_COMP		0x00a4
#define DCU_GRN_CHRM_COMP		0x00a8
#define DCU_BLUE_CHRM_COMP		0x00ac
#define DCU_CRC_POS			0x00b0
#define DCU_LYR_INTPOL_EN		0x00b4
#define DCU_LYR_LUMA_COMP		0x00b8
#define DCU_LYR_CHRM_RED		0x00bc
#define DCU_LYR_CHRM_GRN		0x00c0
#define DCU_LYR_CHRM_BLUE		0x00c4
#define DCU_COMP_IMSIZE			0x00c8

#define DCU_UPDATE_MODE			0x00cc
#define DCU_UPDATE_MODE_MODE(x)		(x << 31)
#define DCU_UPDATE_MODE_READREG(x)	(x << 30)

#define DCU_UNDERRUN			0x00d0

#define DCU_CTRLDESCLN_0(x)		(0x200 + (x) * 0x40)
#define DCU_CTRLDESCLN_0_HEIGHT(x)	(x << 16)
#define DCU_CTRLDESCLN_0_WIDTH(x)	(x)

#define DCU_CTRLDESCLN_1(x)		(0x204 + (x) * 0x40)
#define DCU_CTRLDESCLN_1_POSY(x)	(x << 16)
#define DCU_CTRLDESCLN_1_POSX(x)	(x)

#define DCU_CTRLDESCLN_2(x)		(0x208 + (x) * 0x40)

#define DCU_CTRLDESCLN_3(x)		(0x20c + (x) * 0x40)
#define DCU_CTRLDESCLN_3_EN(x)		(x << 31)
#define DCU_CTRLDESCLN_3_TILE_EN(x)	(x << 30)
#define DCU_CTRLDESCLN_3_DATA_SEL(x)	(x << 29)
#define DCU_CTRLDESCLN_3_SAFETY_EN(x)	(x << 28)
#define DCU_CTRLDESCLN_3_TRANS(x)	(x << 20)
#define DCU_CTRLDESCLN_3_BPP(x)		(x << 16)
#define DCU_CTRLDESCLN_3_RLE_EN(x)	(x << 15)
#define DCU_CTRLDESCLN_3_LUOFFS(x)	(x << 4)
#define DCU_CTRLDESCLN_3_BB(x)		(x << 2)
#define DCU_CTRLDESCLN_3_AB(x)		(x)

#define DCU_CTRLDESCLN_4(x)		(0x210 + (x) * 0x40)
#define DCU_CTRLDESCLN_4_CKMAX_R(x)	(x << 16)
#define DCU_CTRLDESCLN_4_CKMAX_G(x)	(x << 8)
#define DCU_CTRLDESCLN_4_CKMAX_B(x)	(x)

#define DCU_CTRLDESCLN_5(x)		(0x214 + (x) * 0x40)
#define DCU_CTRLDESCLN_5_CKMIN_R(x)	(x << 16)
#define DCU_CTRLDESCLN_5_CKMIN_G(x)	(x << 8)
#define DCU_CTRLDESCLN_5_CKMIN_B(x)	(x)

#define DCU_CTRLDESCLN_6(x)		(0x218 + (x) * 0x40)
#define DCU_CTRLDESCLN_6_TILE_VER(x)	(x << 16)
#define DCU_CTRLDESCLN_6_TILE_HOR(x)	(x)

#define DCU_CTRLDESCLN_7(x)		(0x21c + (x) * 0x40)
#define DCU_CTRLDESCLN_7_FG_FCOLOR(x)	(x)

#define DCU_CTRLDESCLN_8(x)		(0x220 + (x) * 0x40)
#define DCU_CTRLDESCLN_8_BG_BCOLOR(x)	(x)


/**
 * Calculate the size of the framebuffer
 */
ulong calc_fbsize(void);

#endif /* __MVF_DCU_H */
