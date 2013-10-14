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
#include <asm/armv7.h>
#include <asm/pl310.h>
#include <asm/arch/scsc_regs.h>
#include <i2c.h>
#include <mmc.h>
#include <fsl_esdhc.h>
#include <usb/ehci-fsl.h>
#include "ocotp_ctrl_common.h"

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_FSL_ESDHC
struct fsl_esdhc_cfg esdhc_cfg[2] = {
	{ESDHC1_BASE_ADDR, 1},
	{ESDHC2_BASE_ADDR, 1},
};
#endif

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
	__raw_writel(DDR_IOMUX, 0x4004821c);
	__raw_writel(DDR_IOMUX, 0x40048220);
	__raw_writel(DDR_IOMUX, 0x40048224);
	__raw_writel(DDR_IOMUX, 0x40048228);
	__raw_writel(DDR_IOMUX, 0x4004822c);
	__raw_writel(DDR_IOMUX, 0x40048230);
	__raw_writel(DDR_IOMUX, 0x40048234);
	__raw_writel(DDR_IOMUX, 0x40048238);
	__raw_writel(DDR_IOMUX, 0x4004823c);
	__raw_writel(DDR_IOMUX, 0x40048240);
	__raw_writel(DDR_IOMUX, 0x40048244);
	__raw_writel(DDR_IOMUX, 0x40048248);
	__raw_writel(DDR_IOMUX, 0x4004824c);
	__raw_writel(DDR_IOMUX, 0x40048250);
	__raw_writel(DDR_IOMUX, 0x40048254);
	__raw_writel(DDR_IOMUX, 0x40048258);
	__raw_writel(DDR_IOMUX, 0x4004825c);
	__raw_writel(DDR_IOMUX, 0x40048260);	// BA 0
	__raw_writel(DDR_IOMUX, 0x40048264);	// BA 1
	__raw_writel(DDR_IOMUX, 0x40048268);	// BA 0
	__raw_writel(DDR_IOMUX, 0x4004826c);	// CAS b
	__raw_writel(DDR_IOMUX, 0x40048270);	// CKE 0
	__raw_writel(DDR_IOMUX1, 0x40048274);	// CLK 0
	__raw_writel(DDR_IOMUX, 0x40048278);
	__raw_writel(DDR_IOMUX, 0x4004827c);
	__raw_writel(DDR_IOMUX, 0x40048280);
	__raw_writel(DDR_IOMUX, 0x40048284);
	__raw_writel(DDR_IOMUX, 0x40048288);
	__raw_writel(DDR_IOMUX, 0x4004828c);
	__raw_writel(DDR_IOMUX, 0x40048290);
	__raw_writel(DDR_IOMUX, 0x40048294);
	__raw_writel(DDR_IOMUX, 0x40048298);
	__raw_writel(DDR_IOMUX, 0x4004829c);
	__raw_writel(DDR_IOMUX, 0x400482a0);
	__raw_writel(DDR_IOMUX, 0x400482a4);
	__raw_writel(DDR_IOMUX, 0x400482a8);
	__raw_writel(DDR_IOMUX, 0x400482ac);
	__raw_writel(DDR_IOMUX, 0x400482b0);
	__raw_writel(DDR_IOMUX, 0x400482b4);
	__raw_writel(DDR_IOMUX, 0x400482b8);
	__raw_writel(DDR_IOMUX, 0x400482bc);	// DQM 1
	__raw_writel(DDR_IOMUX, 0x400482c0);	// DQM 0
	__raw_writel(DDR_IOMUX1, 0x400482c4);	// DQS 1
	__raw_writel(DDR_IOMUX1, 0x400482c8);	// DQS 0
	__raw_writel(DDR_IOMUX, 0x400482cc);	// RAS
	__raw_writel(DDR_IOMUX, 0x400482d0);	// WE
	__raw_writel(DDR_IOMUX, 0x400482d4);	// ODT 0
	__raw_writel(DDR_IOMUX, 0x400482d8);	// ODT 1
	__raw_writel(DDR_IOMUX, 0x400482dc);
	__raw_writel(DDR_IOMUX, 0x400482e0);

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
	__raw_writel(PHY_DQ_TIMING, DDR_PHY000);
	__raw_writel(PHY_DQ_TIMING, DDR_PHY016);
	__raw_writel(PHY_DQ_TIMING, DDR_PHY032);
	__raw_writel(PHY_DQ_TIMING, DDR_PHY048);

  	// phy_dqs_timing_reg freq set 0
	__raw_writel(PHY_DQS_TIMING, DDR_PHY001);
	__raw_writel(PHY_DQS_TIMING, DDR_PHY017);
	__raw_writel(PHY_DQS_TIMING, DDR_PHY033);
	__raw_writel(PHY_DQS_TIMING, DDR_PHY049);

	// phy_gate_lpbk_ctrl_reg freq set 0
	__raw_writel(PHY_CTRL, DDR_PHY002);	// read delay bit21:19
	__raw_writel(PHY_CTRL, DDR_PHY018);	// phase_detect_sel bit18:16
	__raw_writel(PHY_CTRL, DDR_PHY034);	// bit lpbk_ctrl bit12
	__raw_writel(PHY_CTRL, DDR_PHY050);

	// phy_dll_master_ctrl_reg freq set 0
	__raw_writel(PHY_MASTER_CTRL, DDR_PHY003);
	__raw_writel(PHY_MASTER_CTRL, DDR_PHY019);
	__raw_writel(PHY_MASTER_CTRL, DDR_PHY035);
	__raw_writel(PHY_MASTER_CTRL, DDR_PHY051);

	// phy_dll_slave_ctrl_reg freq set 0
	__raw_writel(PHY_SLAVE_CTRL, DDR_PHY004);
	__raw_writel(PHY_SLAVE_CTRL, DDR_PHY020);
	__raw_writel(PHY_SLAVE_CTRL, DDR_PHY036);
	__raw_writel(PHY_SLAVE_CTRL, DDR_PHY052);

	__raw_writel(0x00001105, DDR_PHY050);
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
	__raw_writel(0x00000600, DDR_CR000);	// LPDDR2 or DDR3
	__raw_writel(0x00000020, DDR_CR002);	// TINIT F0
	// cold boot - 1ms??? 0x61a80
	__raw_writel(0x0000007c, DDR_CR010);	// reset during power on
						// warm boot - 200ns
	__raw_writel(0x00013880, DDR_CR011);	// 500us - 10ns
	__raw_writel(0x0000050c, DDR_CR012);	// CASLAT_LIN, WRLAT
	__raw_writel(0x15040404, DDR_CR013);	// trc, trrd, tccd, tbst_int_interval
	__raw_writel(0x1406040F, DDR_CR014);	// tfaw, trp, twtr, tras_min
	__raw_writel(0x04040000, DDR_CR016);	// tmrd, trtp
	__raw_writel(0x006db00c, DDR_CR017);	// tras_max, tmod
	__raw_writel(0x00000403, DDR_CR018);	// tckesr, tcke

	__raw_writel(0x01000403, DDR_CR020);	// ap, writeinterp, tckesr_f1, tcke_f1

//	__raw_writel(0x06060101, DDR_CR021);	// twr_int, trcd_int, tras_lockout, ccAP
	__raw_writel(0x00060000, DDR_CR021);

	__raw_writel(0x000b0000, DDR_CR022);	// tdal
	__raw_writel(0x03000200, DDR_CR023);	// bstlen, tmrr - lpddr2, tdll
	__raw_writel(0x00000006, DDR_CR024);	// addr_mirror, reg_dimm, trp_ab
	__raw_writel(0x00010000, DDR_CR025);	// tref_enable, auto_refresh, arefresh
#if PHYS_SDRAM_1_SIZE == (128 * 1024 * 1024)
	__raw_writel(0x0c28002c, DDR_CR026);	// tref, trfc
#elif  PHYS_SDRAM_1_SIZE == (256 * 1024 * 1024)
	__raw_writel(0x0c280040, DDR_CR026);	// tref, trfc
#elif  PHYS_SDRAM_1_SIZE == (512 * 1024 * 1024)
	__raw_writel(0x0c280066, DDR_CR026);	// tref, trfc
#else
#error "Unsupported memory size specified"
#endif
	__raw_writel(0x00000005, DDR_CR028);	// tref_interval fixed at 5
	__raw_writel(0x00000003, DDR_CR029);	// tpdex_f0

	__raw_writel(0x0000000a, DDR_CR030);	// txpdll
	__raw_writel(0x00440200, DDR_CR031);	// txsnr, txsr
	__raw_writel(0x00010000, DDR_CR033);	// cke_dly, en_quick_srefresh,
						// srefresh_exit_no_refresh,
						// pwr, srefresh_exit
	__raw_writel(0x00050500, DDR_CR034);	// cksrx_f0,
						// cksre_f0, lowpwr_ref_en

	//
	// Frequency change
	//
	__raw_writel(0x00000100, DDR_CR038);	// freq change...
	__raw_writel(0x04001002, DDR_CR039);	// PHY_INI: com, sta, freq_ch_dll_off

	__raw_writel(0x00000001, DDR_CR041);	// 15.02 - allow dfi_init_start
	__raw_writel(0x00000000, DDR_CR045);	// wrmd
	__raw_writel(0x00000000, DDR_CR046);	// rmd
	__raw_writel(0x00000000, DDR_CR047);	// REF_PER_AUTO_TEMPCHK: LPDDR2 set to 2, else 0

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

	__raw_writel(((MR1 << 16) | MR), DDR_CR048);	// mr0, ddr3 burst of 8 only
						// mr1, if freq < 125, dll_dis = 1, rtt = 0
						// if freq > 125, dll_dis = 0, rtt = 3
	__raw_writel(MR2, DDR_CR049);	// mr0_f1_0 & mr2_f0_0

	__raw_writel(0x00000000, DDR_CR051);	// mr3 & mrsingle_data
	__raw_writel(0x00000000, DDR_CR052);	// mr17, mr16

	//
	// ECC
	//
	__raw_writel(0x00000000, DDR_CR057);	// ctrl_raw
						// ctrl_raw, if DDR3, set 3, else 0
	__raw_writel(0x00000000, DDR_CR058);

	//
	// ZQ stuff
	//
	__raw_writel(0x01000200, DDR_CR066);	// zqcl, zqinit
	__raw_writel(0x02000040, DDR_CR067);	// zqcs
	__raw_writel(0x00000200, DDR_CR069);	// zq_on_sref_exit, qz_req

	__raw_writel(0x00000040, DDR_CR070);	// ref_per_zq
	__raw_writel(0x00000000, DDR_CR071);	// zqreset, ddr3 set to 0
	__raw_writel(0x01000000, DDR_CR072);	// zqcs_rotate, no_zq_init, zqreset_f1

	//
	// DRAM controller misc
	//
//	__raw_writel(0x0a020301, DDR_CR073);	// arebit, col_diff, row_diff, bank_diff

#if PHYS_SDRAM_1_SIZE == (128 * 1024 * 1024)
	__raw_writel(0x0a010300, DDR_CR073);	// arebit, col_diff, row_diff, bank_diff
#elif  PHYS_SDRAM_1_SIZE == (256 * 1024 * 1024)
	__raw_writel(0x0a010200, DDR_CR073);	// arebit, col_diff, row_diff, bank_diff
#elif  PHYS_SDRAM_1_SIZE == (512 * 1024 * 1024)
	__raw_writel(0x0a010100, DDR_CR073);	// arebit, col_diff, row_diff, bank_diff
#else
#error "Unsupported memory size specified"
#endif

	__raw_writel(0x0101ffff, DDR_CR074);	// bank_split, addr_cmp_en, cmd/age cnt
	__raw_writel(0x01010101, DDR_CR075);	// rw same pg, rw same en, pri en, plen
	__raw_writel(0x03030101, DDR_CR076);	// #q_entries_act_dis, (#cmdqueues
						// dis_rw_grp_w_bnk_conflict
						// w2r_split_en, cs_same_en
       	__raw_writel(0x01000101, DDR_CR077);	// cs_map, inhibit_dram_cmd, dis_interleave, swen //by devendra
	__raw_writel(0x0000000c, DDR_CR078);	// qfull, lpddr2_s4, reduc, burst_on_fly
	__raw_writel(0x01000000, DDR_CR079);	// ctrlupd_req_per aref en, ctrlupd_req
						// ctrller busy, in_ord_accept

	//
	// ODT
	//
	__raw_writel(0x01010000, DDR_CR087);	// odt: wr_map_cs, rd_map_cs, port_data_err_id
	__raw_writel(0x00040000, DDR_CR088);	// todtl_2cmd
						// todtl_2cmd = odtl_off = CWL + AL - 2ck
	__raw_writel(0x00000002, DDR_CR089);	// add_odt stuff
	__raw_writel(0x00020000, DDR_CR091);	
	__raw_writel(0x00000000, DDR_CR092);	// tdqsck_min, _max, w2w_smcsdl

	__raw_writel(0x00002819, DDR_CR096);	// wlmrd, wldqsen

#if 0
	__raw_writel(0x00020200, DDR_CR105);
	__raw_writel(0x02020000, DDR_CR106);
	__raw_writel(0x00000202, DDR_CR110);
	__raw_writel(0x00202000, DDR_CR114);
	__raw_writel(0x02020000, DDR_CR115);
#else
	__raw_writel(0x00202000, DDR_CR105);
	__raw_writel(0x20200000, DDR_CR106);
	__raw_writel(0x00002020, DDR_CR110);
	__raw_writel(0x00202000, DDR_CR114);
	__raw_writel(0x20200000, DDR_CR115);
#endif
	
	//
	// AXI ports
	//
//	__raw_writel(0x00000101, DDR_CR117);	// FIFO type (0-async, 1-2:1, 2-1:2, 3- sync, w_pri, r_prii
	__raw_writel(0x00020101, DDR_CR117);	// FIFO type (0-async, 1-2:1, 2-1:2, 3- sync, w_pri, r_prii

	__raw_writel(0x01010000, DDR_CR118);	// w_pri, rpri, en

//	__raw_writel(0x00000000, DDR_CR119);	// fifo_type
	__raw_writel(0x00000002, DDR_CR119);	// fifo_type/

	__raw_writel(0x02020000, DDR_CR120);
	__raw_writel(0x00000202, DDR_CR121);	// round robin port ordering
	__raw_writel(0x01010064, DDR_CR122);
	__raw_writel(0x00010101, DDR_CR123);
	__raw_writel(0x00000064, DDR_CR124);

	//
	// TDFI
	//
	__raw_writel(0x00000000, DDR_CR125);	// dll_rst_adj_dly, dll_rst_delay
	__raw_writel(0x00000b00, DDR_CR126);	// phy_rdlat
	__raw_writel(0x00000000, DDR_CR127);	// dram_ck_dis
//	__raw_writel(0x00003cc8, DDR_CR131);	// tdfi_ctrlupd_interval_f0
	__raw_writel(0, DDR_CR131);	// tdfi_ctrlupd_interval_f0
#define WRADJ	0x00000500
#define RDADJ	0x00000006
#define ADJLAT	(WRADJ | RDADJ)
	__raw_writel(ADJLAT, DDR_CR132);	// wrlat, rdlat 15.02
	__raw_writel(0x00020000, DDR_CR137);	// Phyctl_dl
	__raw_writel(0x04070303, DDR_CR139);

//	__raw_writel(0x000012e3, DDR_CR136);	// tdfi_ctrlupd_interval_f1
	__raw_writel(0, DDR_CR136);	// tdfi_ctrlupd_interval_f1

#if CONFIG_SYS_BOARD_REV >= 0x2A
	/* Silicon difference between 1.0 and 1.1 */
	__raw_writel(0x682C0000, DDR_CR154);	// pad_zq: _early_cmp_en_timer, _mode, _hw_for, _cmp_out_smp
#else
	__raw_writel(0x68200000, DDR_CR154);	// pad_zq: _early_cmp_en_timer, _mode, _hw_for, _cmp_out_smp
#endif
//	__raw_writel(0x00000202, DDR_CR155);	// pad: _ibe1, ibe0, pad_ibe: _sel1, _sel0,
	__raw_writel(0x00000212, DDR_CR155);	// pad: _ibe1, ibe0, pad_ibe: _sel1, _sel0,
						// axi_awcache, axi_cobuf, pad_odt: bate0, byte1
	__raw_writel(0x00000006, DDR_CR158);	// twr
	__raw_writel(0x00000006, DDR_CR159);	// todth

        ddr_phy_init();
        __raw_writel(0x1FFFFFFF, DDR_CR082);
	__raw_writel(0x00000601, DDR_CR000);	// LPDDR2 or DDR3, start


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

#ifdef CONFIG_VIDEO_MVF_DCU
void setup_iomux_dcu(void)
{
	/* Setup DCU0's IOMUX'es */
#define PAD_CTL_DSE_150ohm      (1 << 6)
#define MVF600_HIGH_DRV         PAD_CTL_DSE_150ohm
#define PAD_CTL_OBE_ENABLE      (1 << 1)
#define MVF600_DCU_PAD_CTRL     (MVF600_HIGH_DRV | PAD_CTL_OBE_ENABLE)
	int i;

	/* Backlight control pin */
	__raw_writel(MVF600_DCU_PAD_CTRL, IOMUXC_PAD_030);

#define IOMUXC_PAD_NUMBER 29
#define IOMUXC_DCU_PAD_FIRST (IOMUXC_BASE_ADDR + 0x01A4)
#define MVF600_MUX_MODE_TCON (1 << 20)
#define MVF600_DCU_PAD_CTRL_FLEXTIMER \
		(MVF600_DCU_PAD_CTRL | MVF600_MUX_MODE_TCON)

	/* Signal pins */
	for (i = 0; i < IOMUXC_PAD_NUMBER; i++) {
		__raw_writel(MVF600_DCU_PAD_CTRL_FLEXTIMER,
				IOMUXC_DCU_PAD_FIRST + 4 * i);
	}
}
#endif /* CONFIG_VIDEO_MVF_DCU */

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

#ifdef CONFIG_NAND_FSL_NFC
void setup_iomux_nfc(void)
{
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
#ifdef CONFIG_VIDEO_MVF_DCU
	setup_iomux_dcu();
#endif /* CONFIG_VIDEO_MVF_DCU */

	return 0;
}

#ifdef CONFIG_CMD_SET_QSPI_BOOT
int do_set_qspi_boot(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	typedef struct fuse_struct {
		char bankno;
		char word;
		char bytepos;
		char value;
	} fusedata_t, *pfuse_t;
#if 1
	// quadspi
	fusedata_t fuse[1] = {
		{ 0, 6, 0, 0x10 },
	};
#else
	// sdhc
	fusedata_t fuse[3] = {
		0, 5, 1, 0x28,	/* 4-bit, esdhc 1 */
				0, 5, 0, 0x62,	/* sd card, normal mode */
				0, 6, 0, 0x10,
//			0, 5, 2, 0x40
	};
#endif

	int sz, temp;
	char bank_num=0, word_num=0, byte_pos=0, fuse_val=0;
	int i;

	/* timings for 132MHz IPG clock
	   relax = 2, (2+1)/132000000=22ns > 16.2ns
	   read = 10,(10+1-2*(2+1))/132000000=37ns > 36ns
	   prog = 1325,(1325+1-2*(2+1))/132000000=10000ns */
	*(volatile unsigned long *)0x400a5010 = 0x4a252d;

	sz = (sizeof(fuse) / sizeof(fusedata_t));
	// manual provide value
	for (i = 0; i < sz; i++) {
		bank_num = fuse[i].bankno;
		word_num = fuse[i].word;
		byte_pos = fuse[i].bytepos;
		fuse_val = fuse[i].value;

		program_fuse_word((bank_num*8+word_num),(unsigned int)fuse_val<<(byte_pos*8));
		wait4Busy();
		read_fuse_word(bank_num*8+word_num);
		wait4Busy();
		
		if(((*(unsigned int *)HW_OCOTP_READ_FUSE_DATA_ADDR)&(unsigned int)fuse_val<<(byte_pos*8)) ^ ((unsigned int)fuse_val<<(byte_pos*8)) == 0x0)
			;  //no use. Just to remove if
		else
			printf("Successful programming of fuse bank number 0x%x, word_num 0x%x, at byte position 0x%x with fuse data 0x%x\n",bank_num,word_num,byte_pos,fuse_val);
		
		//Reload cache 
		HW_OCOTP_CTRL_WR(HW_OCOTP_CTRL_RD() | BM_OCOTP_CTRL_RELOAD_SHADOWS);
		//wait for busy
		wait4Busy();
	}
		

}

U_BOOT_CMD(
	set_qspi_boot,	5,		0,	do_set_qspi_boot,
	"sets eFuses of Vybrid to boot from QSPI",
	""
);
#endif

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

#ifndef CONFIG_SYS_DCACHE_OFF
void enable_caches(void)
{
        /* Enable D-cache. I-cache is already enabled in start.S */
        dcache_enable();
}

#if CONFIG_SYS_L2_PL310
void v7_outer_cache_enable(void)
{
	writel(0x132, &pl310->pl310_tag_latency_ctrl);
	writel(0x132, &pl310->pl310_data_latency_ctrl);
	writel(readl(&pl310->pl310_prefetch_ctrl) | 0x40800000,
			&pl310->pl310_prefetch_ctrl);
	writel(readl(&pl310->pl310_pwr_ctrl) | (1 << 1) | (1 << 0),
			&pl310->pl310_pwr_ctrl);
	v7_outer_cache_inval_all();
	writel(1, &pl310->pl310_ctrl);
}
#endif
#endif

#if defined(CONFIG_LCD) && defined(CONFIG_CMD_NAND)
static int splash_load_from_nand(u32 bmp_load_addr)
{
        struct bmp_header *bmp_hdr;
        int res, splash_screen_nand_offset = 0x100000;
        size_t bmp_size, bmp_header_size = sizeof(struct bmp_header);

        if (bmp_load_addr + bmp_header_size >= gd->start_addr_sp)
                goto splash_address_too_high;

        res = nand_read_skip_bad(&nand_info[nand_curr_device],
                        splash_screen_nand_offset, &bmp_header_size,
                        NULL, nand_info[nand_curr_device].size,
                        (u_char *)bmp_load_addr);
        if (res < 0)
                return res;

        bmp_hdr = (struct bmp_header *)bmp_load_addr;
        bmp_size = le32_to_cpu(bmp_hdr->file_size);

        if (bmp_load_addr + bmp_size >= gd->start_addr_sp)
                goto splash_address_too_high;

        return nand_read_skip_bad(&nand_info[nand_curr_device],
                        splash_screen_nand_offset, &bmp_size,
                        NULL, nand_info[nand_curr_device].size,
                        (u_char *)bmp_load_addr);

splash_address_too_high:
        printf("Error: splashimage address too high. Data overwrites U-Boot "
                "and/or placed beyond DRAM boundaries.\n");

        return -1;
}
#endif
