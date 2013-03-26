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
#include <i2c.h>
#include <mmc.h>
#include <fsl_esdhc.h>
#include <usb/ehci-fsl.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_FSL_ESDHC
struct fsl_esdhc_cfg esdhc_cfg[2] = {
	{ESDHC1_BASE_ADDR, 1},
	{ESDHC2_BASE_ADDR, 1},
};
#endif

#define reg32_write(a, b) __raw_writel(b, a)

void setup_iomux_ddr(void)
{
#define DDR_INPUT	(0 << 16)
#define DDR_TRIM	(0 << 14)
#define DDR_DSE		(5 << 6)
#define DDR_IOMUX	(DDR_INPUT | DDR_TRIM | DDR_DSE)

#define DDR_INPUT1	(1 << 16)
#define DDR_TRIM1	(0 << 14)
#define DDR_DSE1	(5 << 6)
#define DDR_IOMUX1	(DDR_INPUT1 | DDR_TRIM1 | DDR_DSE1)
	reg32_write(0x4004821c, DDR_IOMUX);
	reg32_write(0x40048220, DDR_IOMUX);
	reg32_write(0x40048224, DDR_IOMUX);
	reg32_write(0x40048228, DDR_IOMUX);
	reg32_write(0x4004822c, DDR_IOMUX);
	reg32_write(0x40048230, DDR_IOMUX);
	reg32_write(0x40048234, DDR_IOMUX);
	reg32_write(0x40048238, DDR_IOMUX);
	reg32_write(0x4004823c, DDR_IOMUX);
	reg32_write(0x40048240, DDR_IOMUX);
	reg32_write(0x40048244, DDR_IOMUX);
	reg32_write(0x40048248, DDR_IOMUX);
	reg32_write(0x4004824c, DDR_IOMUX);
	reg32_write(0x40048250, DDR_IOMUX);
	reg32_write(0x40048254, DDR_IOMUX);
	reg32_write(0x40048258, DDR_IOMUX);
	reg32_write(0x4004825c, DDR_IOMUX);
	reg32_write(0x40048260, DDR_IOMUX);	// BA 0
	reg32_write(0x40048264, DDR_IOMUX);	// BA 1
	reg32_write(0x40048268, DDR_IOMUX);	// BA 0
	reg32_write(0x4004826c, DDR_IOMUX);	// CAS b
	reg32_write(0x40048270, DDR_IOMUX);	// CKE 0
	reg32_write(0x40048274, DDR_IOMUX1);	// CLK 0
	reg32_write(0x40048278, DDR_IOMUX);
	reg32_write(0x4004827c, DDR_IOMUX);
	reg32_write(0x40048280, DDR_IOMUX);
	reg32_write(0x40048284, DDR_IOMUX);
	reg32_write(0x40048288, DDR_IOMUX);
	reg32_write(0x4004828c, DDR_IOMUX);
	reg32_write(0x40048290, DDR_IOMUX);
	reg32_write(0x40048294, DDR_IOMUX);
	reg32_write(0x40048298, DDR_IOMUX);
	reg32_write(0x4004829c, DDR_IOMUX);
	reg32_write(0x400482a0, DDR_IOMUX);
	reg32_write(0x400482a4, DDR_IOMUX);
	reg32_write(0x400482a8, DDR_IOMUX);
	reg32_write(0x400482ac, DDR_IOMUX);
	reg32_write(0x400482b0, DDR_IOMUX);
	reg32_write(0x400482b4, DDR_IOMUX);
	reg32_write(0x400482b8, DDR_IOMUX);
	reg32_write(0x400482bc, DDR_IOMUX);	// DQM 1
	reg32_write(0x400482c0, DDR_IOMUX);	// DQM 0
	reg32_write(0x400482c4, DDR_IOMUX1);	// DQS 1
	reg32_write(0x400482c8, DDR_IOMUX1);	// DQS 0
	reg32_write(0x400482cc, DDR_IOMUX);	// RAS
	reg32_write(0x400482d0, DDR_IOMUX);	// WE
	reg32_write(0x400482d4, DDR_IOMUX);	// ODT 0
	reg32_write(0x400482d8, DDR_IOMUX);	// ODT 1
	reg32_write(0x400482dc, DDR_IOMUX);
	reg32_write(0x400482e0, DDR_IOMUX);

       /* enable 1V5 for DDR */
	__raw_writel(0x000000CA, IOMUXC_PAD_098);
	__raw_writel(0x00000004, 0x400ff0c4);
	__raw_writel(0x00000004, 0x400ff0c0);
        udelay(100); // wait 100 us for DDR +1V5 to stabilize;
}

void ddr_phy_init(void)
{
#define PHY_DQ_TIMING		0x00002613
#define PHY_DQS_TIMING		0x00002615
#define PHY_CTRL		0x01210080
#if 0
#define PHY_MASTER_CTRL		0x0000002a
#define PHY_SLAVE_CTRL		0x00002000
#else
#define PHY_MASTER_CTRL		0x0001012a
#define PHY_SLAVE_CTRL		0x00012020
#endif
  
  	// phy_dq_timing_reg freq set 0
	reg32_write(DDR_PHY000, PHY_DQ_TIMING);
	reg32_write(DDR_PHY016, PHY_DQ_TIMING);
	reg32_write(DDR_PHY032, PHY_DQ_TIMING);
	reg32_write(DDR_PHY048, PHY_DQ_TIMING);

  	// phy_dqs_timing_reg freq set 0
	reg32_write(DDR_PHY001, PHY_DQS_TIMING);
	reg32_write(DDR_PHY017, PHY_DQS_TIMING);
	reg32_write(DDR_PHY033, PHY_DQS_TIMING);
	reg32_write(DDR_PHY049, PHY_DQS_TIMING);

	// phy_gate_lpbk_ctrl_reg freq set 0
	reg32_write(DDR_PHY002, PHY_CTRL);	// read delay bit21:19
	reg32_write(DDR_PHY018, PHY_CTRL);	// phase_detect_sel bit18:16
	reg32_write(DDR_PHY034, PHY_CTRL);	// bit lpbk_ctrl bit12
	reg32_write(DDR_PHY050, PHY_CTRL);

	// phy_dll_master_ctrl_reg freq set 0
	reg32_write(DDR_PHY003, PHY_MASTER_CTRL);
	reg32_write(DDR_PHY019, PHY_MASTER_CTRL);
	reg32_write(DDR_PHY035, PHY_MASTER_CTRL);
	reg32_write(DDR_PHY051, PHY_MASTER_CTRL);

	// phy_dll_slave_ctrl_reg freq set 0
	reg32_write(DDR_PHY004, PHY_SLAVE_CTRL);
	reg32_write(DDR_PHY020, PHY_SLAVE_CTRL);
	reg32_write(DDR_PHY036, PHY_SLAVE_CTRL);
	reg32_write(DDR_PHY052, PHY_SLAVE_CTRL);

	reg32_write(DDR_PHY050, 0x00001105);
}

unsigned long ddr_ctrl_init(void)
{
	int dram_size, rows, cols, banks, port;

	int i;

	//Turn on clock gating
	//CCM->CCGR6 |= 0x30000000;        

	//
	// Dram Device Parameters
	//
	reg32_write(DDR_CR000, 0x00000600);	// LPDDR2 or DDR3
	reg32_write(DDR_CR002, 0x00000020);	// TINIT F0
	// cold boot - 1ms??? 0x61a80
	reg32_write(DDR_CR010, 0x0000007c);	// reset during power on
						// warm boot - 200ns
	reg32_write(DDR_CR011, 0x00013880);	// 500us - 10ns
	reg32_write(DDR_CR012, 0x0000050c);	// CASLAT_LIN, WRLAT
	reg32_write(DDR_CR013, 0x15040404);	// trc, trrd, tccd, tbst_int_interval
	reg32_write(DDR_CR014, 0x1406040F);	// tfaw, trp, twtr, tras_min
	reg32_write(DDR_CR016, 0x04040000);	// tmrd, trtp
	reg32_write(DDR_CR017, 0x006db00c);	// tras_max, tmod
	reg32_write(DDR_CR018, 0x00000403);	// tckesr, tcke

	reg32_write(DDR_CR020, 0x01000403);	// ap, writeinterp, tckesr_f1, tcke_f1

//	reg32_write(DDR_CR021, 0x06060101);	// twr_int, trcd_int, tras_lockout, ccAP
	reg32_write(DDR_CR021, 0x00060000);

	reg32_write(DDR_CR022, 0x000b0000);	// tdal
	reg32_write(DDR_CR023, 0x03000200);	// bstlen, tmrr - lpddr2, tdll
	reg32_write(DDR_CR024, 0x00000006);	// addr_mirror, reg_dimm, trp_ab
	reg32_write(DDR_CR025, 0x00010000);	// tref_enable, auto_refresh, arefresh
	reg32_write(DDR_CR026, 0x0c28002c);	// tref, trfc
	reg32_write(DDR_CR028, 0x00000005);	// tref_interval fixed at 5
	reg32_write(DDR_CR029, 0x00000003);	// tpdex_f0

	reg32_write(DDR_CR030, 0x0000000a);	// txpdll
	reg32_write(DDR_CR031, 0x00440200);	// txsnr, txsr
	reg32_write(DDR_CR033, 0x00010000);	// cke_dly, en_quick_srefresh,
						// srefresh_exit_no_refresh,
						// pwr, srefresh_exit
	reg32_write(DDR_CR034, 0x00050500);	// cksrx_f0,
						// cksre_f0, lowpwr_ref_en

	//
	// Frequency change
	//
	reg32_write(DDR_CR038, 0x00000100);	// freq change...
	reg32_write(DDR_CR039, 0x04001002);	// PHY_INI: com, sta, freq_ch_dll_off

	reg32_write(DDR_CR041, 0x00000001);	// 15.02 - allow dfi_init_start
	reg32_write(DDR_CR045, 0x00000000);	// wrmd
	reg32_write(DDR_CR046, 0x00000000);	// rmd
	reg32_write(DDR_CR047, 0x00000000);	// REF_PER_AUTO_TEMPCHK: LPDDR2 set to 2, else 0

	//
	// DRAM device Mode registers
	//
#define MR_BL_FIXED8	0x0000	// Fixed 8
#define MR_BL_4_8_A12	0x0001	// 4 or 8 on A12
#define MR_BL_FIXED4	0x0002	// Fixed 4
#define MR_CAS_5	0x0010	// CAS 5
#define MR_CAS_6	0x0020	// CAS 6
#define MR_CAS_7	0x0030	// CAS 7
#define MR_CAS_8	0x0040	// CAS 8
#define MR_CAS_9	0x0050	// CAS 9
#define MR_CAS_10	0x0060	// CAS 10
#define MR_CAS_11	0x0070	// CAS 11
#define MR_CAS_12	0x0004	// CAS 12
#define MR_CAS_13	0x0014	// CAS 13
#define MR_WR_5		0x0200	// Write Recovery 5
#define MR_WR_6		0x0400	// Write Recovery 6
#define MR_WR_7		0x0600	// Write Recovery 7
#define MR_WR_8		0x0800	// Write Recovery 8
#define MR_WR_10	0x0A00	// Write Recovery 10
#define MR_WR_12	0x0C00	// Write Recovery 12
#define MR_WR_14	0x0E00	// Write Recovery 14
#define MR	(MR_BL_FIXED8 | MR_CAS_6 | MR_WR_6)

#define MR1_ODS_RZQ_6	0x0000	// drvstrength 40 ohm
#define MR1_ODS_RZQ_7	0x0002	// drvstrength 34 ohm
#define MR1_AL_DIS	0x0000
#define MR1_AL_1	0x0008	// CL - 1
#define MR1_AL_2	0x0010	// CL - 2
#define MR1_ODT_DIS	0x0000
#define MR1_ODT_RZQ_4	0x0004	// RZQ/4 60 ohm
#define MR1_ODT_RZQ_2	0x0040	// RZQ/2 120 ohm
#define MR1_ODT_RZQ_6	0x0044	// RZQ/6 40 ohm
#define MR1_ODT_RZQ_12	0x0200	// RZQ/12 20 ohm
#define MR1_ODT_RZQ_8	0x0204	// RZQ/8 30 ohm
#define MR1_WL		0x0080	// Write Leveling
#define MR1_QOFF	0x0400	// Q off
//#define MR1	(MR1_ODS_RZQ_7 | MR1_AL_DIS | MR1_ODT_RZQ_6)

#define MR1	(MR1_ODS_RZQ_6 | MR1_AL_DIS | MR1_ODT_RZQ_4)


#define MR2_CWL_5CK	0x0000	// 5ck (tCK >= 2.5 ns)
#define MR2_CWL_6CK	0x0008	// 6ck (2.5 ns > tCK >= 1.875 ns)
#define MR2_CWL_7CK	0x0010	// 7ck (1.875 ns > tCK >= 1.5 ns)
#define MR2_CWL_8CK	0x0018	// 8ck (1.5 ns > tCK >= 1.25 ns)
#define MR2_CWL_9CK	0x0020	// 9ck (1.25 ns > tCK >= 1.07 ns)
#define MR2_DODT_DIS	0x0000
#define MR2_DODT_RZQ_4	0x0200	// RZQ / 4
#define MR2_DODT_RZQ_2	0x0400	// RZQ / 2
#define MR2	(MR2_CWL_5CK | MR2_DODT_DIS)

	reg32_write(DDR_CR048, ((MR1 << 16) | MR));	// mr0, ddr3 burst of 8 only
						// mr1, if freq < 125, dll_dis = 1, rtt = 0
						// if freq > 125, dll_dis = 0, rtt = 3
	reg32_write(DDR_CR049, MR2);	// mr0_f1_0 & mr2_f0_0

	reg32_write(DDR_CR051, 0x00000000);	// mr3 & mrsingle_data
	reg32_write(DDR_CR052, 0x00000000);	// mr17, mr16

	//
	// ECC
	//
	reg32_write(DDR_CR057, 0x00000000);	// ctrl_raw
						// ctrl_raw, if DDR3, set 3, else 0
	reg32_write(DDR_CR058, 0x00000000);

	//
	// ZQ stuff
	//
	reg32_write(DDR_CR066, 0x01000200);	// zqcl, zqinit
	reg32_write(DDR_CR067, 0x02000040);	// zqcs
	reg32_write(DDR_CR069, 0x00000200);	// zq_on_sref_exit, qz_req

	reg32_write(DDR_CR070, 0x00000040);	// ref_per_zq
	reg32_write(DDR_CR071, 0x00000000);	// zqreset, ddr3 set to 0
	reg32_write(DDR_CR072, 0x01000000);	// zqcs_rotate, no_zq_init, zqreset_f1

	//
	// DRAM controller misc
	//
//	reg32_write(DDR_CR073, 0x0a020301);	// arebit, col_diff, row_diff, bank_diff

	reg32_write(DDR_CR073, 0x0a010300);	// arebit, col_diff, row_diff, bank_diff

	reg32_write(DDR_CR074, 0x0101ffff);	// bank_split, addr_cmp_en, cmd/age cnt
	reg32_write(DDR_CR075, 0x01010101);	// rw same pg, rw same en, pri en, plen
	reg32_write(DDR_CR076, 0x03030101);	// #q_entries_act_dis, (#cmdqueues
						// dis_rw_grp_w_bnk_conflict
						// w2r_split_en, cs_same_en
       	reg32_write(DDR_CR077, 0x01000101);	// cs_map, inhibit_dram_cmd, dis_interleave, swen //by devendra
	reg32_write(DDR_CR078, 0x0000000c);	// qfull, lpddr2_s4, reduc, burst_on_fly
	reg32_write(DDR_CR079, 0x01000000);	// ctrlupd_req_per aref en, ctrlupd_req
						// ctrller busy, in_ord_accept

	//
	// ODT
	//
	reg32_write(DDR_CR087, 0x01010000);	// odt: wr_map_cs, rd_map_cs, port_data_err_id
	reg32_write(DDR_CR088, 0x00040000);	// todtl_2cmd
						// todtl_2cmd = odtl_off = CWL + AL - 2ck
	reg32_write(DDR_CR089, 0x00000002);	// add_odt stuff
	reg32_write(DDR_CR091, 0x00020000);	
	reg32_write(DDR_CR092, 0x00000000);	// tdqsck_min, _max, w2w_smcsdl

	reg32_write(DDR_CR096, 0x00002819);	// wlmrd, wldqsen

#if 0
	reg32_write(DDR_CR105, 0x00020200);
	reg32_write(DDR_CR106, 0x02020000);
	reg32_write(DDR_CR110, 0x00000202);
	reg32_write(DDR_CR114, 0x00202000);
	reg32_write(DDR_CR115, 0x02020000);
#else
	reg32_write(DDR_CR105, 0x00202000);
	reg32_write(DDR_CR106, 0x20200000);
	reg32_write(DDR_CR110, 0x00002020);
	reg32_write(DDR_CR114, 0x00202000);
	reg32_write(DDR_CR115, 0x20200000);
#endif
	
	//
	// AXI ports
	//
//	reg32_write(DDR_CR117, 0x00000101);	// FIFO type (0-async, 1-2:1, 2-1:2, 3- sync, w_pri, r_prii
	reg32_write(DDR_CR117, 0x00020101);	// FIFO type (0-async, 1-2:1, 2-1:2, 3- sync, w_pri, r_prii

	reg32_write(DDR_CR118, 0x01010000);	// w_pri, rpri, en

//	reg32_write(DDR_CR119, 0x00000000);	// fifo_type
	reg32_write(DDR_CR119, 0x00000002);	// fifo_type/

	reg32_write(DDR_CR120, 0x02020000);
	reg32_write(DDR_CR121, 0x00000202);	// round robin port ordering
	reg32_write(DDR_CR122, 0x01010064);
	reg32_write(DDR_CR123, 0x00010101);
	reg32_write(DDR_CR124, 0x00000064);

	//
	// TDFI
	//
	reg32_write(DDR_CR125, 0x00000000);	// dll_rst_adj_dly, dll_rst_delay
	reg32_write(DDR_CR126, 0x00000b00);	// phy_rdlat
	reg32_write(DDR_CR127, 0x00000000);	// dram_ck_dis
//	reg32_write(DDR_CR131, 0x00003cc8);	// tdfi_ctrlupd_interval_f0
	reg32_write(DDR_CR131, 0);	// tdfi_ctrlupd_interval_f0
#define WRADJ	0x00000500
#define RDADJ	0x00000006
#define ADJLAT	(WRADJ | RDADJ)
	reg32_write(DDR_CR132, ADJLAT);	// wrlat, rdlat 15.02
	reg32_write(DDR_CR137, 0x00020000);	// Phyctl_dl
	reg32_write(DDR_CR139, 0x04070303);

//	reg32_write(DDR_CR136, 0x000012e3);	// tdfi_ctrlupd_interval_f1
	reg32_write(DDR_CR136, 0);	// tdfi_ctrlupd_interval_f1

	reg32_write(DDR_CR154, 0x68200000);	// pad_zq: _early_cmp_en_timer, _mode, _hw_for, _cmp_out_smp
//	reg32_write(DDR_CR155, 0x00000202);	// pad: _ibe1, ibe0, pad_ibe: _sel1, _sel0,
	reg32_write(DDR_CR155, 0x00000212);	// pad: _ibe1, ibe0, pad_ibe: _sel1, _sel0,
						// axi_awcache, axi_cobuf, pad_odt: bate0, byte1
	reg32_write(DDR_CR158, 0x00000006);	// twr
	reg32_write(DDR_CR159, 0x00000006);	// todth

        ddr_phy_init();
        reg32_write(DDR_CR082, 0x1FFFFFFF);
	reg32_write(DDR_CR000, 0x00000601);	// LPDDR2 or DDR3, start


	i = 5000;
	while (i--);

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

#define WAIT_DDR_INIT 0x3FFFFFF

 int int_stat,wait_cntr;

	//udelay (1000000);
	setup_iomux_ddr();
#ifdef CONFIG_SYS_UBOOT_IN_GPURAM
	gd->ram_size = 0x80000;
	ddr_ctrl_init();
#else
	gd->ram_size = ddr_ctrl_init();
#endif
	int_stat = 0;
	wait_cntr = 0;
	while((int_stat == 0) & (wait_cntr < WAIT_DDR_INIT)){ 	
	int_stat =(__raw_readl(DDR_CR080) >> 8) & 1; // check if DDR controller is initialized;
	wait_cntr = wait_cntr + 1;
	}
	if (int_stat) {
        puts ("DDR controller is initialized\n");	
	}
	else {
	puts ("DDR controller is not initialized\n");
	}
	return 0;
}

void setup_iomux_uart(void)
{
	/* UART0 */
	__raw_writel(0x001021a3, IOMUXC_PAD_032);
	__raw_writel(0x001021a1, IOMUXC_PAD_033);
}

#if defined(CONFIG_CMD_NET)
int fecpin_setclear(struct eth_device *dev, int setclear)
{
	struct fec_info_s *info = (struct fec_info_s *)dev->priv;

#define ENET_SRE	(1 << 11)
#define ENET_ODE	(0 << 10)
#define ENET_DRV	(2 << 6)
#define ENETMUX		(ENET_SRE | ENET_ODE | ENET_DRV)
	__raw_writel(0x00200001 | ENETMUX, IOMUXC_PAD_000);

	if (setclear) {
		if (info->iobase == MACNET0_BASE_ADDR) {
			/* MDC,MDIO,RxDV,RxD1,RxD0 */
			__raw_writel(0x00103002 | ENETMUX, IOMUXC_PAD_045);
			__raw_writel(0x00103003 | ENETMUX, IOMUXC_PAD_046);
			__raw_writel(0x00103001 | ENETMUX, IOMUXC_PAD_047);
			__raw_writel(0x00103001 | ENETMUX, IOMUXC_PAD_048);
			__raw_writel(0x00103001 | ENETMUX, IOMUXC_PAD_049);
			/* RxER,TxD1,TxD0,TxEn */
			__raw_writel(0x00103001 | ENETMUX, IOMUXC_PAD_050);
			__raw_writel(0x00103002 | ENETMUX, IOMUXC_PAD_051);
			__raw_writel(0x00103002 | ENETMUX, IOMUXC_PAD_052);
			__raw_writel(0x00103002 | ENETMUX, IOMUXC_PAD_053);
		}
		if (info->iobase == MACNET1_BASE_ADDR) {
			__raw_writel(0x00103182, IOMUXC_PAD_054); /* MDC */
			__raw_writel(0x00103183, IOMUXC_PAD_055); /* MDIO */
			__raw_writel(0x00103181, IOMUXC_PAD_056); /* RxDV */
			__raw_writel(0x00103181, IOMUXC_PAD_057); /* RxD1 */
			__raw_writel(0x00103181, IOMUXC_PAD_058); /* RxD0 */
			__raw_writel(0x00103181, IOMUXC_PAD_059); /* RxER */
			__raw_writel(0x00103182, IOMUXC_PAD_060); /* TxD1 */
			__raw_writel(0x00103182, IOMUXC_PAD_061); /* TxD0 */
			__raw_writel(0x00103182, IOMUXC_PAD_062); /* TxEn */
		}
	} else {
		if (info->iobase == MACNET0_BASE_ADDR) {
			__raw_writel(0x00003192, IOMUXC_PAD_045); /* MDC */
			__raw_writel(0x00003193, IOMUXC_PAD_046); /* MDIO */
			__raw_writel(0x00003191, IOMUXC_PAD_047); /* RxDV */
			__raw_writel(0x00003191, IOMUXC_PAD_048); /* RxD1 */
			__raw_writel(0x00003191, IOMUXC_PAD_049); /* RxD0 */
			__raw_writel(0x00003191, IOMUXC_PAD_050); /* RxER */
			__raw_writel(0x00003192, IOMUXC_PAD_051); /* TxD1 */
			__raw_writel(0x00003192, IOMUXC_PAD_052); /* TxD0 */
			__raw_writel(0x00003192, IOMUXC_PAD_053); /* TxEn */
		}
		if (info->iobase == MACNET1_BASE_ADDR) {
			__raw_writel(0x00003192, IOMUXC_PAD_054); /* MDC */
			__raw_writel(0x00003193, IOMUXC_PAD_055); /* MDIO */
			__raw_writel(0x00003191, IOMUXC_PAD_056); /* RxDV */
			__raw_writel(0x00003191, IOMUXC_PAD_057); /* RxD1 */
			__raw_writel(0x00003191, IOMUXC_PAD_058); /* RxD0 */
			__raw_writel(0x00003191, IOMUXC_PAD_059); /* RxER */
			__raw_writel(0x00003192, IOMUXC_PAD_060); /* TxD1 */
			__raw_writel(0x00003192, IOMUXC_PAD_061); /* TxD0 */
			__raw_writel(0x00003192, IOMUXC_PAD_062); /* TxEn */
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

#ifdef CONFIG_FSL_ESDHC
int board_mmc_getcd(struct mmc *mmc)
{
	/*struct fsl_esdhc_cfg *cfg = (struct fsl_esdhc_cfg *)mmc->priv;*/
	int ret;

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
	u32 index = 0;
	s32 status = 0;

	status |= fsl_esdhc_initialize(bis, &esdhc_cfg[index]);
	return status;
}
#endif

int board_early_init_f(void)
{
	setup_iomux_uart();

	return 0;
}

int board_init(void)
{
	/* address of boot parameters */
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;

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

#ifndef CONFIG_SYS_DCACHE_OFF
void enable_caches(void)
{
        /* Enable D-cache. I-cache is already enabled in start.S */
//        dcache_enable();
}
#endif
