/*
 * (C) Copyright 2007
 * Sascha Hauer, Pengutronix
 *
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

#include <common.h>
#include <asm/io.h>
#include <asm/errno.h>
#include <asm/arch/vybrid-regs.h>
#include <asm/arch/clock.h>
#include <div64.h>

DECLARE_GLOBAL_DATA_PTR;

enum pll_clocks {
	PLL1_CLOCK = 0,
	PLL2_CLOCK,
	PLL3_CLOCK,
	PLL4_CLOCK,
	PLL_CLOCKS,
};

struct mxc_pll_reg *vybridc_plls[PLL_CLOCKS] = {
/*
	[PLL1_CLOCK] = (struct mxc_pll_reg *)PLL1_BASE_ADDR,
	[PLL2_CLOCK] = (struct mxc_pll_reg *)PLL2_BASE_ADDR,
	[PLL3_CLOCK] = (struct mxc_pll_reg *)PLL3_BASE_ADDR,
*/
};

struct clkctl *ccm = (struct clkctl *)CCM_BASE_ADDR;

/* Calculate the frequency of PLLn. */
static uint32_t decode_pll(struct mxc_pll_reg *pll, uint32_t infreq)
{
#if 0
	uint32_t ctrl, op, mfd, mfn, mfi, pdf, ret;
	uint64_t refclk, temp;
	int32_t mfn_abs;

	ctrl = readl(&pll->ctrl);

	if (ctrl & MXC_DPLLC_CTL_HFSM) {
		mfn = __raw_readl(&pll->hfs_mfn);
		mfd = __raw_readl(&pll->hfs_mfd);
		op = __raw_readl(&pll->hfs_op);
	} else {
		mfn = __raw_readl(&pll->mfn);
		mfd = __raw_readl(&pll->mfd);
		op = __raw_readl(&pll->op);
	}

	mfd &= MXC_DPLLC_MFD_MFD_MASK;
	mfn &= MXC_DPLLC_MFN_MFN_MASK;
	pdf = op & MXC_DPLLC_OP_PDF_MASK;
	mfi = (op & MXC_DPLLC_OP_MFI_MASK) >> MXC_DPLLC_OP_MFI_OFFSET;

	/* 21.2.3 */
	if (mfi < 5)
		mfi = 5;

	/* Sign extend */
	if (mfn >= 0x04000000) {
		mfn |= 0xfc000000;
		mfn_abs = -mfn;
	} else
		mfn_abs = mfn;

	refclk = infreq * 2;
	if (ctrl & MXC_DPLLC_CTL_DPDCK0_2_EN)
		refclk *= 2;

	do_div(refclk, pdf + 1);
	temp = refclk * mfn_abs;
	do_div(temp, mfd + 1);
	ret = refclk * mfi;

	if ((int)mfn < 0)
		ret -= temp;
	else
		ret += temp;

	return ret;
#else
	return 0;
#endif
}

/* Get mcu main rate */
u32 get_mcu_main_clk(void)
{
	u32 reg, freq;

	reg = __raw_readl(&ccm->cacrr) & 7;
	freq = decode_pll(vybridc_plls[PLL1_CLOCK], CONFIG_SYS_VYBRID_HCLK);
	return freq / (reg + 1);
}

/* Get the rate of peripheral's root clock. */
static u32 get_periph_clk(void)
{
#if 0
	u32 reg;

	reg = __raw_readl(&mxc_ccm->cbcdr);
	if (!(reg & MXC_CCM_CBCDR_PERIPH_CLK_SEL))
		return decode_pll(vybridc_plls[PLL2_CLOCK],
				  CONFIG_SYS_VYBRID_HCLK);
	reg = __raw_readl(&mxc_ccm->cbcmr);
	switch ((reg & MXC_CCM_CBCMR_PERIPH_CLK_SEL_MASK) >>
		MXC_CCM_CBCMR_PERIPH_CLK_SEL_OFFSET) {
	case 0:
		return decode_pll(vybridc_plls[PLL1_CLOCK],
				  CONFIG_SYS_VYBRID_HCLK);
	case 1:
		return decode_pll(vybridc_plls[PLL3_CLOCK],
				  CONFIG_SYS_VYBRID_HCLK);
	default:
		return 0;
	}
	/* NOTREACHED */
#else
	return 0;
#endif
}

/* Get the rate of ahb clock. */
static u32 get_ahb_clk(void)
{
#if 0
	uint32_t freq, div, reg;

	freq = get_periph_clk();

	reg = __raw_readl(&mxc_ccm->cbcdr);
	div = ((reg & MXC_CCM_CBCDR_AHB_PODF_MASK) >>
			MXC_CCM_CBCDR_AHB_PODF_OFFSET) + 1;

	return freq / div;
#else
	return 0;
#endif
}

/* Get the rate of ipg clock. */
static u32 get_ipg_clk(void)
{
#if 0
	uint32_t freq, reg, div;

	freq = get_ahb_clk();

	reg = __raw_readl(&mxc_ccm->cbcdr);
	div = ((reg & MXC_CCM_CBCDR_IPG_PODF_MASK) >>
			MXC_CCM_CBCDR_IPG_PODF_OFFSET) + 1;

	return freq / div;
#else
	return 66000000;
#endif
}

/* Get the rate of ipg_per clock. */
static u32 get_ipg_per_clk(void)
{
#if 0
	u32 pred1, pred2, podf;

	if (__raw_readl(&mxc_ccm->cbcmr) & MXC_CCM_CBCMR_PERCLK_IPG_CLK_SEL)
		return get_ipg_clk();
	/* Fixme: not handle what about lpm*/
	podf = __raw_readl(&mxc_ccm->cbcdr);
	pred1 = (podf & MXC_CCM_CBCDR_PERCLK_PRED1_MASK) >>
		MXC_CCM_CBCDR_PERCLK_PRED1_OFFSET;
	pred2 = (podf & MXC_CCM_CBCDR_PERCLK_PRED2_MASK) >>
		MXC_CCM_CBCDR_PERCLK_PRED2_OFFSET;
	podf = (podf & MXC_CCM_CBCDR_PERCLK_PODF_MASK) >>
		MXC_CCM_CBCDR_PERCLK_PODF_OFFSET;

	return get_periph_clk() / ((pred1 + 1) * (pred2 + 1) * (podf + 1));
#else
	return 0;
#endif
}

/* get cspi clock rate. */
u32 vybrid_get_dspiclk(void)
{
#if 0
	u32 ret_val = 0, pdf, pre_pdf, clk_sel;
	u32 cscmr1 = __raw_readl(&mxc_ccm->cscmr1);
	u32 cscdr2 = __raw_readl(&mxc_ccm->cscdr2);

	pre_pdf = (cscdr2 & MXC_CCM_CSCDR2_CSPI_CLK_PRED_MASK) \
			>> MXC_CCM_CSCDR2_CSPI_CLK_PRED_OFFSET;
	pdf = (cscdr2 & MXC_CCM_CSCDR2_CSPI_CLK_PODF_MASK) \
			>> MXC_CCM_CSCDR2_CSPI_CLK_PODF_OFFSET;
	clk_sel = (cscmr1 & MXC_CCM_CSCMR1_CSPI_CLK_SEL_MASK) \
			>> MXC_CCM_CSCMR1_CSPI_CLK_SEL_OFFSET;

	switch (clk_sel) {
	case 0:
		ret_val = decode_pll(vybridc_plls[PLL1_CLOCK],
					CONFIG_SYS_VYBRID_HCLK) /
					((pre_pdf + 1) * (pdf + 1));
		break;
	case 1:
		ret_val = decode_pll(vybridc_plls[PLL2_CLOCK],
					CONFIG_SYS_VYBRID_HCLK) /
					((pre_pdf + 1) * (pdf + 1));
		break;
	case 2:
		ret_val = decode_pll(vybridc_plls[PLL3_CLOCK],
					CONFIG_SYS_VYBRID_HCLK) /
					((pre_pdf + 1) * (pdf + 1));
		break;
	default:
		ret_val = get_lp_apm() / ((pre_pdf + 1) * (pdf + 1));
		break;
	}

	return ret_val;
#else
	return 0;
#endif
}

/* The API of get vybrid clocks. */
unsigned int vybrid_get_clock(enum vybrid_clock clk)
{
	switch (clk) {
	case VYBRID_ARM_CLK:
		return get_mcu_main_clk();
	case VYBRID_AHB_CLK:
		return get_ahb_clk();
	case VYBRID_IPG_CLK:
		return get_ipg_clk();
	case VYBRID_IPG_PERCLK:
		return get_ipg_per_clk();
	case VYBRID_UART_CLK:
		return vybrid_get_uartclk();
	case VYBRID_CSPI_CLK:
		return vybrid_get_dspiclk();
	case VYBRID_FEC_CLK:
#if 0
		return decode_pll(vybridc_plls[PLL1_CLOCK],
				    CONFIG_SYS_VYBRID_HCLK);
#else
		return 0;
#endif

	default:
		break;
	}
	return -1;
}

/* Get the rate of uart clk. */
u32 vybrid_get_uartclk(void)
{
	return gd->ipg_clk;
}

u32 vybrid_get_fecclk(void)
{
#if 0
	return vybrid_get_clock(VYBRID_IPG_CLK);
#else
	return 0;
#endif
}

/* Dump some core clockes. */
int do_vybrid_showclocks(cmd_tbl_t *cmdtp, int flag, int argc,
			 char * const argv[])
{
#if 0
	u32 freq;
	freq = decode_pll(vybridc_plls[PLL1_CLOCK], CONFIG_SYS_VYBRID_HCLK);
	printf("PLL1       %8d MHz\n", freq / 1000000);
	freq = decode_pll(vybridc_plls[PLL2_CLOCK], CONFIG_SYS_VYBRID_HCLK);
	printf("PLL2       %8d MHz\n", freq / 1000000);
	freq = decode_pll(vybridc_plls[PLL3_CLOCK], CONFIG_SYS_VYBRID_HCLK);
	printf("PLL3       %8d MHz\n", freq / 1000000);
#endif
	printf("\n");
	printf("AHB        %8d kHz\n",
		vybrid_get_clock(VYBRID_AHB_CLK) / 1000);
	printf("IPG        %8d kHz\n",
		vybrid_get_clock(VYBRID_IPG_CLK) / 1000);
	printf("IPG PERCLK %8d kHz\n",
		vybrid_get_clock(VYBRID_IPG_PERCLK) / 1000);

	return 0;
}

/***************************************************/

U_BOOT_CMD(
	clocks,	CONFIG_SYS_MAXARGS, 1, do_vybrid_showclocks,
	"display clocks",
	""
);
