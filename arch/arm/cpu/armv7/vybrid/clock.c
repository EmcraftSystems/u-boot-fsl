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

unsigned long anadig_ctrl[PLL_CLOCKS] = {
	0x40050270,
	0x40050030,
	0x40050010,
	0x40050070,
};

unsigned long anadig_num[PLL_CLOCKS] = {
	0x40050290,
	0x40050050,
	0x00000000,
	0x40050080,
};

unsigned long anadig_denum[PLL_CLOCKS] = {
	0x400502A0,
	0x40050060,
	0x00000000,
	0x40050090,
};

unsigned long anadig_pfd[PLL_CLOCKS] = {
	0x400502B0,
	0x40050100,
	0x400500F0,
	0x00000000,
};

struct clkctl *ccm = (struct clkctl *)CCM_BASE_ADDR;

/* Calculate the frequency of PLLn. */
static uint32_t decode_pll(enum pll_clocks pll, uint32_t pfd)
{
	unsigned long freq;

	switch (pll) {
	case PLL1_CLOCK:
	case PLL2_CLOCK:
	case PLL3_CLOCK:
		if (pll == PLL3_CLOCK)
			freq = CONFIG_SYS_VYBRID_HCLK * 20;
		else
			freq = (__raw_readl(anadig_ctrl[pll]) & 1 ?
				CONFIG_SYS_VYBRID_HCLK * 22 :
					CONFIG_SYS_VYBRID_HCLK * 20) +
				CONFIG_SYS_VYBRID_HCLK *
				__raw_readl(anadig_num[pll]) /
				__raw_readl(anadig_denum[pll]);
		if (!pfd)
			return freq;
		else if (!((__raw_readl(anadig_pfd[pll]) >> ((pfd - 1) * 8)) &
						(1 << 6)))
			return 0;
		else
			return (unsigned long)freq / 1000 * 18 /
				((__raw_readl(anadig_pfd[pll]) >>
						((pfd - 1) * 8)) & 0x3F) * 1000;
		break;
	case PLL4_CLOCK:
		return CONFIG_SYS_VYBRID_HCLK * (__raw_readl(anadig_ctrl[pll]) & 0xFF);
		break;
	default:
		return 0;
	}
	return 0;
}

/* Get mcu main rate */
u32 get_mcu_main_clk(void)
{
#if 0
	u32 reg, freq;

	reg = __raw_readl(&ccm->cacrr) & 7;
	freq = decode_pll(vybridc_plls[PLL1_CLOCK], CONFIG_SYS_VYBRID_HCLK);
	return freq / (reg + 1);
#else
	/* FIXME: calculate the CA5 frequency rather than hard-coding */
	return CONFIG_SYS_CLOCK_FREQUENCY;
#endif
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
	return CONFIG_SYS_CLOCK_FREQUENCY / 6;
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

/* Dump some core clocks. */
int do_vybrid_showclocks(cmd_tbl_t *cmdtp, int flag, int argc,
			 char * const argv[])
{
	u32 freq, ca5_freq;

	printf("\n");

	printf("Cortex-A5  ");
	switch (__raw_readl(0x4006b008) & 7) {
	case 0:
		ca5_freq = 24000000;
		printf("FAST CLK");
		break;
	case 1:
		ca5_freq = 32000;
		printf("SLOW CLK");
		break;
	case 2:
		ca5_freq = decode_pll(PLL2_CLOCK,
				(__raw_readl(0x4006b008) >> 19) & 7);
		printf("PLL2");
		if ((__raw_readl(0x4006b008) >> 19) & 7)
			printf(" PFD%i", (__raw_readl(0x4006b008) >> 19) & 7);
		break;
	case 3:
		ca5_freq = decode_pll(PLL2_CLOCK, 0);
		printf("PLL2");
		break;
	case 4:
		ca5_freq = decode_pll(PLL1_CLOCK,
				(__raw_readl(0x4006b008) >> 16) & 7);
		printf("PLL1");
		if ((__raw_readl(0x4006b008) >> 16) & 7)
			printf(" PFD%i", (__raw_readl(0x4006b008) >> 16) & 7);
		break;
	case 5:
		ca5_freq = decode_pll(PLL3_CLOCK, 0);
		printf("PLL3");
		break;
	default:
		ca5_freq = 0;
		break;
	}

	printf(" (%3d MHz)\n", ca5_freq / 1000000);
	printf("Cortex-M4  %3d MHz\n",
			ca5_freq / (((__raw_readl(0x4006b00c) >> 3) & 7) + 1) /
			1000000);
	printf("PlatBus    %3d MHz\n",
			ca5_freq / (((__raw_readl(0x4006b00c) >> 3) & 7) + 1) /
			1000000);
	printf("IPG        %3d MHz\n",
			ca5_freq / (((__raw_readl(0x4006b00c) >> 3) & 7) + 1) /
			(((__raw_readl(0x4006b00c) >> 11) & 3) + 1) /
			1000000);
	printf("PLL1       %3d MHz (%10d, %10d, %10d, %10d)\n",
			decode_pll(PLL1_CLOCK, 0) / 1000000,
			decode_pll(PLL1_CLOCK, 1),
			decode_pll(PLL1_CLOCK, 2),
			decode_pll(PLL1_CLOCK, 3),
			decode_pll(PLL1_CLOCK, 4));
	printf("PLL2       %3d MHz (%10d, %10d, %10d, %10d)\n",
			decode_pll(PLL2_CLOCK, 0) / 1000000,
			decode_pll(PLL2_CLOCK, 1),
			decode_pll(PLL2_CLOCK, 2),
			decode_pll(PLL2_CLOCK, 3),
			decode_pll(PLL2_CLOCK, 4));
	printf("PLL3       %3d MHz (%10d, %10d, %10d, %10d)\n",
			decode_pll(PLL3_CLOCK, 0) / 1000000,
			decode_pll(PLL3_CLOCK, 1),
			decode_pll(PLL3_CLOCK, 2),
			decode_pll(PLL3_CLOCK, 3),
			decode_pll(PLL3_CLOCK, 4));
	printf("PLL4       %3d MHz\n",
			decode_pll(PLL3_CLOCK, 0) / 1000000);

	printf("DDR        ");
	if ((__raw_readl(0x4006b008) >> 6) & 1) {
		printf("SYS_DIV_OUT_CLK (%d)\n", ca5_freq);
	} else {
		printf("PLL2 PFD2 (%d)\n", decode_pll(PLL2_CLOCK, 2));
	}

	printf("NFC        ");
	if (!(__raw_readl(0x4006b018) & 0x200)) {
		printf("Disabled\n");
	} else {
		switch ((__raw_readl(0x4006b010) >> 12) & 3) {
		case 0:
			printf("Platform Bus Clock");
			freq = gd->bus_clk;
			break;
		case 1:
			printf("PLL1 PFD1");
			freq = decode_pll(PLL1_CLOCK, 1);
			break;
		case 2:
			printf("PLL3 PFD1");
			freq = decode_pll(PLL3_CLOCK, 1);
			break;
		case 3:
			printf("PLL3 PFD3");
			freq = decode_pll(PLL3_CLOCK, 3);
			break;
		default:
			printf("impossible");
			freq = 0;
			break;
		}
		printf(" (%d)\n", freq /
				(((__raw_readl(0x4006B01C) >> 13) & 7) + 1) /
				(((__raw_readl(0x4006B018) >> 4) & 0xF) + 1));
	}

	printf("QSPI0      ");
	if (!(__raw_readl(0x4006B01C) & 0x10)) {
		printf("Disabled\n");
	} else {
		if (__raw_readl(0x4006b03C) & 2) {
			printf("Platform Bus Clock");
			freq = gd->bus_clk;
		} else {
			switch ((__raw_readl(0x4006b010) >> 22) & 3) {
			case 0:
				printf("PLL3");
				freq = decode_pll(PLL3_CLOCK, 0);
				break;
			case 1:
				printf("PLL3 PFD4");
				freq = decode_pll(PLL3_CLOCK, 4);
				break;
			case 2:
				printf("PLL2 PFD4");
				freq = decode_pll(PLL2_CLOCK, 4);
				break;
			case 3:
				printf("PLL1 PFD4");
				freq = decode_pll(PLL1_CLOCK, 4);
				break;
			default:
				printf("impossible");
				freq = 0;
				break;
			}
		}

		printf(" (%d)\n", freq /
				(1 + ((__raw_readl(0x4006B01C) >> 3) & 1)) /
				(1 + ((__raw_readl(0x4006B01C) >> 2) & 1)) /
				(1 + ((__raw_readl(0x4006B01C) >> 0) & 3)));
	}

	printf("QSPI1      ");
	if (!(__raw_readl(0x4006B01C) & 0x1000)) {
		printf("Disabled\n");
	} else {
		if (__raw_readl(0x4006b03C) & 2) {
			printf("Platform Bus Clock");
			freq = gd->bus_clk;
		} else {
			switch ((__raw_readl(0x4006b010) >> 24) & 3) {
			case 0:
				printf("PLL3");
				freq = decode_pll(PLL3_CLOCK, 0);
				break;
			case 1:
				printf("PLL3 PFD4");
				freq = decode_pll(PLL3_CLOCK, 4);
				break;
			case 2:
				printf("PLL2 PFD4");
				freq = decode_pll(PLL2_CLOCK, 4);
				break;
			case 3:
				printf("PLL1 PFD4");
				freq = decode_pll(PLL1_CLOCK, 4);
				break;
			default:
				printf("impossible");
				freq = 0;
				break;
			}
		}

		printf(" (%d)\n", freq /
				(1 + ((__raw_readl(0x4006B01C) >> 11) & 1)) /
				(1 + ((__raw_readl(0x4006B01C) >> 10) & 1)) /
				(1 + ((__raw_readl(0x4006B01C) >> 8) & 3)));
	}

	printf("eSDHC0     ");
	if (!(__raw_readl(0x4006b018) & (1 << 28))) {
		printf("Disabled\n");
	} else {
		switch ((__raw_readl(0x4006b010) >> 16) & 0x3) {
		case 3:
			printf("Platform Bus Clock");
			freq = gd->bus_clk;
			break;
		case 0:
			printf("PLL3");
			freq = decode_pll(PLL3_CLOCK, 0);
			break;
		case 1:
			printf("PLL3 PFD3");
			freq = decode_pll(PLL3_CLOCK, 3);
			break;
		case 2:
			printf("PLL1 PFD3");
			freq = decode_pll(PLL1_CLOCK, 3);
			break;
		default:
			printf("impossible");
			freq = 0;
			break;
		}
		printf(" (%d)\n", freq /
				(((__raw_readl(0x4006B018) >> 16) & 0xF) + 1));
	}

	printf("eSDHC1     ");
	if (!(__raw_readl(0x4006b018) & (1 << 29))) {
		printf("Disabled\n");
	} else {
		switch ((__raw_readl(0x4006b010) >> 18) & 0x3) {
		case 3:
			printf("Platform Bus Clock");
			freq = gd->bus_clk;
			break;
		case 0:
			printf("PLL3");
			freq = decode_pll(PLL3_CLOCK, 0);
			break;
		case 1:
			printf("PLL3 PFD3");
			freq = decode_pll(PLL3_CLOCK, 3);
			break;
		case 2:
			printf("PLL1 PFD3");
			freq = decode_pll(PLL1_CLOCK, 3);
			break;
		default:
			printf("impossible");
			freq = 0;
			break;
		}
		printf(" (%d)\n", freq /
				(((__raw_readl(0x4006B018) >> 20) & 0xF) + 1));
	}

	return 0;
}

/***************************************************/

U_BOOT_CMD(
	clocks,	CONFIG_SYS_MAXARGS, 1, do_vybrid_showclocks,
	"display clocks",
	""
);
