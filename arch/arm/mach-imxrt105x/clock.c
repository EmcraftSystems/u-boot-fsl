/*
 * (C) Copyright 2017 Emcraft Systems
 * Yuri Tikhonov <yur@emcraft.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>

void mxs_set_lcdclk(uint32_t __maybe_unused lcd_base, uint32_t freq)
{
	static struct clk_regs {
		u32	ofs;
		u32	msk;
		u32	val;
	}
	ana_regs[] = {
		{ 0x0a0, 0x001DF07F,	/* ANALOG_PLL_VIDEO: /2,ENA,d31	*/
		  (1 << 19) | (1 << 13) | (31 << 0) },
		{ 0x170, 3 << 30, 3 << 30 }, /* ANALOG_MISC2[VIDEO]: /4	*/
	},
	ccm_regs[] = {
		{ 0x018, 7 << 23, 1 << 23 }, /* CBCMR[LCDIF_PODF]: /2	*/
		{ 0x038,		     /* CSCDR2[LCDIF]: PLL5,/5	*/
		  (7 << 15) | (7 << 12) | (7 << 9),
		  (2 << 15) | (4 << 12) },
		{ 0x070, 3 << 28, 3 << 28 }, /* CCGR2[CG14]: lcd clk	*/
		{ 0x074, 3 << 10, 3 << 10 }, /* CCGR3[CG05]: pix clk	*/
		{ 0x07c, 3 << 16, 3 << 16 }, /* CCGR5[CG08]: sim clk	*/
	};

	static struct {
		char		*prop;
		struct clk_regs	*reg;
		u32		base;
		int		reg_num;
	} clk[] = {
		{ "imx-ana-init", ana_regs, 0x400d8000, ARRAY_SIZE(ana_regs) },
		{ "imx-ccm-init", ccm_regs, 0x400fc000, ARRAY_SIZE(ccm_regs) },
	};

	u32 i, k, val, base;

	/* Check if initialization of some i.MX CCM clocks is required */
	for (i = 0; i < ARRAY_SIZE(clk); i++) {
		base = clk[i].base;
		for (k = 0; k < clk[i].reg_num; k++) {
			val = readl(base + clk[i].reg[k].ofs);
			val &= ~clk[i].reg[k].msk;
			val |= clk[i].reg[k].val;
			writel(val, base + clk[i].reg[k].ofs);
		}
	}
}
