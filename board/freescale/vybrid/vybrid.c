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

#include <common.h>
#include <asm/io.h>
#include <asm/fec.h>
#include <asm/gpio.h>
#include <asm/arch/vybrid-regs.h>
#include <asm/arch/vybrid-pins.h>
#include <asm/arch/iomux.h>
#include <asm/errno.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/crm_regs.h>
#include <asm/arch/scsc_regs.h>
#include <i2c.h>
#include <mmc.h>
#include <fsl_esdhc.h>
#include <usb/ehci-fsl.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_FSL_ESDHC
struct fsl_esdhc_cfg esdhc_cfg[2] = {
	{CONFIG_SYS_ESDHC1_BASE, 1},
	{ESDHC2_BASE_ADDR, 1},
};
#endif

void setup_iomux_ddr(void)
{
#define DDR_IOMUX	0x00000180
#define DDR_IOMUX1	0x00010180
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_A15);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_A14);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_A13);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_A12);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_A11);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_A10);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_A9);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_A8);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_A7);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_A6);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_A5);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_A4);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_A3);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_A2);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_A1);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_BA2);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_BA1);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_BA0);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_CAS);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_CKE);
	__raw_writel(DDR_IOMUX1, IOMUXC_DDR_CLK);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_CS);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_D15);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_D14);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_D13);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_D12);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_D11);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_D10);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_D9);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_D8);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_D7);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_D6);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_D5);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_D4);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_D3);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_D2);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_D1);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_D0);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_DQM1);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_DQM0);
	__raw_writel(DDR_IOMUX1, IOMUXC_DDR_DQS1);
	__raw_writel(DDR_IOMUX1, IOMUXC_DDR_DQS0);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_RAS);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_WE);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_ODT1);
	__raw_writel(DDR_IOMUX, IOMUXC_DDR_ODT0);
}

void ddr_phy_init(void)
{
#define PHY_DQ_TIMING		0x00002613
#define PHY_DQS_TIMING		0x00002615
#define PHY_CTRL		0x01210080
#define PHY_MASTER_CTRL		0x0001012a
#define PHY_SLAVE_CTRL		0x00012020

	/* phy_dq_timing_reg freq set 0 */
	__raw_writel(PHY_DQ_TIMING, DDR_PHY000);
	__raw_writel(PHY_DQ_TIMING, DDR_PHY016);
	__raw_writel(PHY_DQ_TIMING, DDR_PHY032);
	__raw_writel(PHY_DQ_TIMING, DDR_PHY048);

	/* phy_dqs_timing_reg freq set 0 */
	__raw_writel(PHY_DQS_TIMING, DDR_PHY001);
	__raw_writel(PHY_DQS_TIMING, DDR_PHY017);
	__raw_writel(PHY_DQS_TIMING, DDR_PHY033);
	__raw_writel(PHY_DQS_TIMING, DDR_PHY049);

	/* phy_gate_lpbk_ctrl_reg freq set 0 */
	__raw_writel(PHY_CTRL, DDR_PHY002);	/* read delay bit21:19 */
	__raw_writel(PHY_CTRL, DDR_PHY018);	/* phase_detect_sel bit18:16 */
	__raw_writel(PHY_CTRL, DDR_PHY034);	/* bit lpbk_ctrl bit12 */
	__raw_writel(PHY_CTRL, DDR_PHY050);

	/* phy_dll_master_ctrl_reg freq set 0 */
	__raw_writel(PHY_MASTER_CTRL, DDR_PHY003);
	__raw_writel(PHY_MASTER_CTRL, DDR_PHY019);
	__raw_writel(PHY_MASTER_CTRL, DDR_PHY035);
	__raw_writel(PHY_MASTER_CTRL, DDR_PHY051);

	/* phy_dll_slave_ctrl_reg freq set 0 */
	__raw_writel(PHY_SLAVE_CTRL, DDR_PHY004);
	__raw_writel(PHY_SLAVE_CTRL, DDR_PHY020);
	__raw_writel(PHY_SLAVE_CTRL, DDR_PHY036);
	__raw_writel(PHY_SLAVE_CTRL, DDR_PHY052);

	__raw_writel(0x00001105, DDR_PHY050);
}

unsigned long ddr_ctrl_init(void)
{
	int dram_size, rows, cols, banks, port;

	__raw_writel(0x00000600, DDR_CR000);	/* LPDDR2 or DDR3 */
	__raw_writel(0x00000020, DDR_CR002);	/* TINIT */
	__raw_writel(0x0000007c, DDR_CR010);	/* reset during power on */
						/* warm boot - 200ns */
	__raw_writel(0x00013880, DDR_CR011);	/* 500us - 10ns */
	__raw_writel(0x0000050c, DDR_CR012);	/* CASLAT_LIN, WRLAT */
	__raw_writel(0x15040404, DDR_CR013);	/* trc, trrd, tccd
						   tbst_int_interval */
	__raw_writel(0x1406040F, DDR_CR014);	/* tfaw, tfp, twtr, tras_min */
	__raw_writel(0x04040000, DDR_CR016);	/* tmrd, trtp */
	__raw_writel(0x006DB00C, DDR_CR017);	/* tras_max, tmod */
	__raw_writel(0x00000403, DDR_CR018);	/* tckesr, tcke */

	__raw_writel(0x01000000, DDR_CR020);	/* ap, writrp */
	__raw_writel(0x06060101, DDR_CR021);	/* trcd_int, tras_lockout
						   ccAP */
	__raw_writel(0x000B0000, DDR_CR022);	/* tdal */
	__raw_writel(0x03000200, DDR_CR023);	/* bstlen, tmrr, tdll */
	__raw_writel(0x00000006, DDR_CR024);	/* addr_mirror, reg_dimm
						   trp_ab */
	__raw_writel(0x00010000, DDR_CR025);	/* tref_enable, auto_refresh
						   arefresh */
	__raw_writel(0x0C28002C, DDR_CR026);	/* tref, trfc */
	__raw_writel(0x00000005, DDR_CR028);	/* tref_interval fixed at 5 */
	__raw_writel(0x00000003, DDR_CR029);	/* tpdex */

	__raw_writel(0x0000000A, DDR_CR030);	/* txpdll */
	__raw_writel(0x00440200, DDR_CR031);	/* txsnr, txsr */
	__raw_writel(0x00010000, DDR_CR033);	/* cke_dly, en_quick_srefresh
						 * srefresh_exit_no_refresh,
						 * pwr, srefresh_exit
						 */
	__raw_writel(0x00050500, DDR_CR034);	/* cksrx, */
						/* cksre, lowpwr_ref_en */

	/* Frequency change */
	__raw_writel(0x00000100, DDR_CR038);	/* freq change... */
	__raw_writel(0x04001002, DDR_CR039);

	__raw_writel(0x00000001, DDR_CR041);	/* dfi_init_start */
	__raw_writel(0x00000000, DDR_CR045);	/* wrmd */
	__raw_writel(0x00000000, DDR_CR046);	/* rdmd */
	__raw_writel(0x00000000, DDR_CR047);	/* REF_PER_AUTO_TEMPCHK:
						 *   LPDDR2 set to 2, else 0
						 */

	/* DRAM device Mode registers */
	__raw_writel(0x00460420, DDR_CR048);	/* mr0, ddr3 burst of 8 only
						 * mr1, if freq < 125
						 * dll_dis = 1, rtt = 0
						 * if freq > 125, dll_dis = 0
						 * rtt = 3
						 */
	__raw_writel(0x00000000, DDR_CR049);	/* mr2 */
	__raw_writel(0x00000000, DDR_CR051);	/* mr3 & mrsingle_data_0 */

	__raw_writel(0x00000000, DDR_CR057);	/* ctrl_raw */

	/* ECC */
	/*__raw_writel(0x00000000, DDR_CR058);*/

	/* ZQ stuff */
	__raw_writel(0x01000200, DDR_CR066);	/* zqcl, zqinit */
	__raw_writel(0x02000040, DDR_CR067);	/* zqcs */
	__raw_writel(0x00000200, DDR_CR069);	/* zq_on_sref_exit, qz_req */

	__raw_writel(0x00000040, DDR_CR070);	/* ref_per_zq */
	__raw_writel(0x00000000, DDR_CR071);	/* zqreset, ddr3 set to 0 */
	__raw_writel(0x01000000, DDR_CR072);	/* zqcs_rotate, no_zq_init */

	/* DRAM controller misc */
	__raw_writel(0x0a010300, DDR_CR073);	/* arebit, col_diff, row_diff
						   bank_diff */
	__raw_writel(0x0101ffff, DDR_CR074);	/* bank_split, addr_cmp_en
						   cmd/age cnt */
	__raw_writel(0x01010101, DDR_CR075);	/* rw same pg, rw same en
						   pri en, plen */
	__raw_writel(0x03030101, DDR_CR076);	/* #q_entries_act_dis
						 * (#cmdqueues
						 * dis_rw_grp_w_bnk_conflict
						 * w2r_split_en, cs_same_en */
	__raw_writel(0x01000101, DDR_CR077);	/* cs_map, inhibit_dram_cmd
						 * dis_interleave, swen */
	__raw_writel(0x0000000C, DDR_CR078);	/* qfull, lpddr2_s4, reduc
						   burst_on_fly */
	__raw_writel(0x01000000, DDR_CR079);	/* ctrlupd_req_per aref en
						 * ctrlupd_req
						 * ctrller busy
						 * in_ord_accept */
	/* disable interrupts */
	__raw_writel(0x1FFFFFFF, DDR_CR082);

	/* ODT */
	__raw_writel(0x01010000, DDR_CR087);	/* odt: wr_map_cs0
						 * rd_map_cs0
						 * port_data_err_id */
	__raw_writel(0x00040000, DDR_CR088);	/* todtl_2cmd */
	__raw_writel(0x00000002, DDR_CR089);	/* add_odt stuff */

	__raw_writel(0x00020000, DDR_CR091);
	__raw_writel(0x00000000, DDR_CR092);	/* tdqsck _min, max */

	__raw_writel(0x00002819, DDR_CR096);	/* wlmrd, wldqsen */


	/* AXI ports */
	__raw_writel(0x00202000, DDR_CR105);
	__raw_writel(0x20200000, DDR_CR106);
	__raw_writel(0x00002020, DDR_CR110);
	__raw_writel(0x00202000, DDR_CR114);
	__raw_writel(0x20200000, DDR_CR115);

	__raw_writel(0x00000101, DDR_CR117);	/* FIFO type (0-async, 1-2:1
						 *	2-1:2, 3- sync, w_pri
						 * r_pri
						 */
	__raw_writel(0x01010000, DDR_CR118);	/* w_pri, rpri, en */
	__raw_writel(0x00000000, DDR_CR119);	/* fifo_type */

	__raw_writel(0x02020000, DDR_CR120);
	__raw_writel(0x00000202, DDR_CR121);
	__raw_writel(0x01010064, DDR_CR122);
	__raw_writel(0x00000101, DDR_CR123);
	__raw_writel(0x00000064, DDR_CR124);

	/* TDFI */
	__raw_writel(0x00000000, DDR_CR125);
	__raw_writel(0x00000B00, DDR_CR126);	/* PHY rdlat */
	__raw_writel(0x00000000, DDR_CR127);	/* dram ck dis */

	__raw_writel(0x00000000, DDR_CR131);
	__raw_writel(0x00000506, DDR_CR132);	/* wrlat, rdlat */
	__raw_writel(0x02000000, DDR_CR137);
	__raw_writel(0x04070303, DDR_CR139);

	__raw_writel(0x00000000, DDR_CR136);

	__raw_writel(0x68200000, DDR_CR154);
	__raw_writel(0x00000202, DDR_CR155);	/* pad_ibe, _sel */
	__raw_writel(0x00000006, DDR_CR158);	/* twr */
	__raw_writel(0x00000006, DDR_CR159);	/* todth */

	ddr_phy_init();

	__raw_writel(0x00000601, DDR_CR000);	/* LPDDR2 or DDR3, start */

	udelay(200);

	rows = (__raw_readl(DDR_CR001) & 0x1F) -
	       ((__raw_readl(DDR_CR073) >> 8) & 3);
	cols = ((__raw_readl(DDR_CR001) >> 8) & 0xF) -
	       ((__raw_readl(DDR_CR073) >> 16) & 7);
	banks = 1 << (3 - (__raw_readl(DDR_CR073) & 3));
	port = ((__raw_readl(DDR_CR078) >> 8) & 1) ? 1 : 2;

	dram_size = (1 << (rows + cols)) * banks * port;

	return dram_size;
}

int dram_init(void)
{
	setup_iomux_ddr();
#ifdef CONFIG_SYS_UBOOT_IN_GPURAM
	gd->ram_size = 0x80000;
	ddr_ctrl_init();
#else
	gd->ram_size = ddr_ctrl_init();
#endif
	return 0;
}

void setup_iomux_uart(void)
{
	__raw_writel(0x002011a2, IOMUXC_PAD_026);
	__raw_writel(0x002011a1, IOMUXC_PAD_027);
	__raw_writel(0x002011a2, IOMUXC_PAD_028);
	__raw_writel(0x002011a1, IOMUXC_PAD_029);
	__raw_writel(0x001011a2, IOMUXC_PAD_032);
	__raw_writel(0x001011a1, IOMUXC_PAD_033);
}

#if defined(CONFIG_CMD_NET)
int fecpin_setclear(struct eth_device *dev, int setclear)
{
	struct fec_info_s *info = (struct fec_info_s *)dev->priv;

	__raw_writel(0x00203191, IOMUXC_PAD_000);	/* RMII_CLK */

	if (setclear) {
		if (info->iobase == CONFIG_SYS_FEC0_IOBASE) {
			__raw_writel(0x00103192, IOMUXC_PAD_045);	/*MDC*/
			__raw_writel(0x00103193, IOMUXC_PAD_046);	/*MDIO*/
			__raw_writel(0x00103191, IOMUXC_PAD_047);	/*RxDV*/
			__raw_writel(0x00103191, IOMUXC_PAD_048);	/*RxD1*/
			__raw_writel(0x00103191, IOMUXC_PAD_049);	/*RxD0*/
			__raw_writel(0x00103191, IOMUXC_PAD_050);	/*RxER*/
			__raw_writel(0x00103192, IOMUXC_PAD_051);	/*TxD1*/
			__raw_writel(0x00103192, IOMUXC_PAD_052);	/*TxD0*/
			__raw_writel(0x00103192, IOMUXC_PAD_053);	/*TxEn*/
		}
		if (info->iobase == CONFIG_SYS_FEC1_IOBASE) {
			__raw_writel(0x00103192, IOMUXC_PAD_054);	/*MDC*/
			__raw_writel(0x00103193, IOMUXC_PAD_055);	/*MDIO*/
			__raw_writel(0x00103191, IOMUXC_PAD_056);	/*RxDV*/
			__raw_writel(0x00103191, IOMUXC_PAD_057);	/*RxD1*/
			__raw_writel(0x00103191, IOMUXC_PAD_058);	/*RxD0*/
			__raw_writel(0x00103191, IOMUXC_PAD_059);	/*RxER*/
			__raw_writel(0x00103192, IOMUXC_PAD_060);	/*TxD1*/
			__raw_writel(0x00103192, IOMUXC_PAD_061);	/*TxD0*/
			__raw_writel(0x00103192, IOMUXC_PAD_062);	/*TxEn*/
		}
	} else {
		if (info->iobase == CONFIG_SYS_FEC0_IOBASE) {
			__raw_writel(0x00003192, IOMUXC_PAD_045);	/*MDC*/
			__raw_writel(0x00003193, IOMUXC_PAD_046);	/*MDIO*/
			__raw_writel(0x00003191, IOMUXC_PAD_047);	/*RxDV*/
			__raw_writel(0x00003191, IOMUXC_PAD_048);	/*RxD1*/
			__raw_writel(0x00003191, IOMUXC_PAD_049);	/*RxD0*/
			__raw_writel(0x00003191, IOMUXC_PAD_050);	/*RxER*/
			__raw_writel(0x00003192, IOMUXC_PAD_051);	/*TxD1*/
			__raw_writel(0x00003192, IOMUXC_PAD_052);	/*TxD0*/
			__raw_writel(0x00003192, IOMUXC_PAD_053);	/*TxEn*/
		}
		if (info->iobase == CONFIG_SYS_FEC1_IOBASE) {
			__raw_writel(0x00003192, IOMUXC_PAD_054);	/*MDC*/
			__raw_writel(0x00003193, IOMUXC_PAD_055);	/*MDIO*/
			__raw_writel(0x00003191, IOMUXC_PAD_056);	/*RxDV*/
			__raw_writel(0x00003191, IOMUXC_PAD_057);	/*RxD1*/
			__raw_writel(0x00003191, IOMUXC_PAD_058);	/*RxD0*/
			__raw_writel(0x00003191, IOMUXC_PAD_059);	/*RxER*/
			__raw_writel(0x00003192, IOMUXC_PAD_060);	/*TxD1*/
			__raw_writel(0x00003192, IOMUXC_PAD_061);	/*TxD0*/
			__raw_writel(0x00003192, IOMUXC_PAD_062);	/*TxEn*/
		}
	}

	return 0;
}
#endif

#ifdef CONFIG_MXC_SPI
void setup_iomux_spi(void)
{
}
#endif

#ifdef CONFIG_QUAD_SPI
void setup_iomux_quadspi(void)
{
	__raw_writel(0x001030C3, IOMUXC_PAD_079);	/* SCK */
	__raw_writel(0x001030FF, IOMUXC_PAD_080);	/* CS0 */
	__raw_writel(0x001030C3, IOMUXC_PAD_081);	/* D3 */
	__raw_writel(0x001030C3, IOMUXC_PAD_082);	/* D2 */
	__raw_writel(0x001030C3, IOMUXC_PAD_083);	/* D1 */
	__raw_writel(0x001030C3, IOMUXC_PAD_084);	/* D0 */

	__raw_writel(0x001030C3, IOMUXC_PAD_086);	/* SCK */
	__raw_writel(0x001030FF, IOMUXC_PAD_087);	/* CS0 */
	__raw_writel(0x001030C3, IOMUXC_PAD_088);	/* D3 */
	__raw_writel(0x001030C3, IOMUXC_PAD_089);	/* D2 */
	__raw_writel(0x001030C3, IOMUXC_PAD_090);	/* D1 */
	__raw_writel(0x001030C3, IOMUXC_PAD_091);	/* D0 */
}
#endif

#ifdef CONFIG_FSL_ESDHC
int board_mmc_getcd(struct mmc *mmc)
{
	/*struct fsl_esdhc_cfg *cfg = (struct fsl_esdhc_cfg *)mmc->priv;*/
	int ret;
#if 0
	__raw_writel(0x005031e2, IOMUXC_PAD_045);	/* clk */
	__raw_writel(0x005031e2, IOMUXC_PAD_046);	/* cmd */
	__raw_writel(0x005031e3, IOMUXC_PAD_047);	/* dat0 */
	__raw_writel(0x005031e3, IOMUXC_PAD_048);	/* dat1 */
	__raw_writel(0x005031e3, IOMUXC_PAD_049);	/* dat2 */
	__raw_writel(0x005031e3, IOMUXC_PAD_050);	/* dat3 */
#endif
	__raw_writel(0x005031ef, IOMUXC_PAD_014);	/* clk */
	__raw_writel(0x005031ef, IOMUXC_PAD_015);	/* cmd */
	__raw_writel(0x005031ef, IOMUXC_PAD_016);	/* dat0 */
	__raw_writel(0x005031ef, IOMUXC_PAD_017);	/* dat1 */
	__raw_writel(0x005031ef, IOMUXC_PAD_018);	/* dat2 */
	__raw_writel(0x005031ef, IOMUXC_PAD_019);	/* dat3 */

	ret = 1;
	return ret;
}

int board_mmc_init(bd_t *bis)
{
	u32 index;
	s32 status = 0;
#if 0
	printf("%s: ", __func__);
	for (index = 0; index < CONFIG_SYS_FSL_ESDHC_NUM;
			index++) {
		switch (index) {
		case 0:
			printf("sd %d\n", index);
			__raw_writel(0x005031e2, IOMUXC_PAD_045); /* clk */
			__raw_writel(0x005031e2, IOMUXC_PAD_046); /* cmd */
			__raw_writel(0x005031e3, IOMUXC_PAD_047); /* dat0 */
			__raw_writel(0x005031e3, IOMUXC_PAD_048); /* dat1 */
			__raw_writel(0x005031e3, IOMUXC_PAD_049); /* dat2 */
			__raw_writel(0x005031e3, IOMUXC_PAD_050); /* dat3 */
			__raw_writel(0x005031e2, IOMUXC_PAD_051); /* wp */
			break;
		case 1:
			printf("sd %d\n", index);
			__raw_writel(0x005031a2, IOMUXC_PAD_014); /* clk */
			__raw_writel(0x005031a2, IOMUXC_PAD_015); /* cmd */
			__raw_writel(0x005031a3, IOMUXC_PAD_016); /* dat0 */
			__raw_writel(0x005031a3, IOMUXC_PAD_017); /* dat1 */
			__raw_writel(0x005031a3, IOMUXC_PAD_018); /* dat2 */
			__raw_writel(0x005031a3, IOMUXC_PAD_019); /* dat3 */
			__raw_writel(0x005031a2, IOMUXC_PAD_068); /* wp */
			break;
		default:
			printf("Warning: you configured more ESDHC controller"
				"(%d) as supported by the board(2)\n",
				CONFIG_SYS_FSL_ESDHC_NUM);
			return status;
		}
	}
#endif
	status |= fsl_esdhc_initialize(bis, &esdhc_cfg[index]);
	return status;
}
#endif

#ifdef CONFIG_NAND_FSL_NFC
void setup_iomux_nfc(void)
{
	__raw_writel(0x002038df, IOMUXC_PAD_063);
	__raw_writel(0x002038df, IOMUXC_PAD_064);
	__raw_writel(0x002038df, IOMUXC_PAD_065);
	__raw_writel(0x002038df, IOMUXC_PAD_066);
	__raw_writel(0x002038df, IOMUXC_PAD_067);
	__raw_writel(0x002038df, IOMUXC_PAD_068);
	__raw_writel(0x002038df, IOMUXC_PAD_069);
	__raw_writel(0x002038df, IOMUXC_PAD_070);
	__raw_writel(0x002038df, IOMUXC_PAD_071);
	__raw_writel(0x002038df, IOMUXC_PAD_072);
	__raw_writel(0x002038df, IOMUXC_PAD_073);
	__raw_writel(0x002038df, IOMUXC_PAD_074);
	__raw_writel(0x002038df, IOMUXC_PAD_075);
	__raw_writel(0x002038df, IOMUXC_PAD_076);
	__raw_writel(0x002038df, IOMUXC_PAD_077);
	__raw_writel(0x002038df, IOMUXC_PAD_078);

	__raw_writel(0x005038d2, IOMUXC_PAD_094);
	__raw_writel(0x005038d2, IOMUXC_PAD_095);
	__raw_writel(0x006038d2, IOMUXC_PAD_097);
	__raw_writel(0x005038dd, IOMUXC_PAD_099);
	__raw_writel(0x006038d2, IOMUXC_PAD_100);
	__raw_writel(0x006038d2, IOMUXC_PAD_101);
}
#endif
int board_early_init_f(void)
{
	setup_iomux_uart();
#ifdef CONFIG_NAND_FSL_NFC
	setup_iomux_nfc();
#endif
	return 0;
}

int board_init(void)
{
	u32 temp;
	struct vybrid_scsc_reg *scsc = (struct vybrid_scsc_reg *)SCSCM_BASE_ADDR;

	/* address of boot parameters */
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;

	/*
	 * Enable external 32K Oscillator
	 *
	 * The internal clock experiences significant drift
         * so we must use the external oscillator in order
	 * to maintain correct time in the hwclock
	 */
	temp = __raw_readl(&scsc->sosc_ctr);
	temp |= VYBRID_SCSC_SICR_CTR_SOSC_EN;
	__raw_writel(temp, &scsc->sosc_ctr);

	return 0;
}

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
#ifdef CONFIG_MXC_SPI
	setup_iomux_spi();
#endif
	return 0;
}
#endif

int checkboard(void)
{
	puts("Board: Vybrid\n");

	return 0;
}
