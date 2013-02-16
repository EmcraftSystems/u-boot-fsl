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

#ifndef __ASM_ARCH_TIMER_H
#define __ASM_ARCH_TIMER_H

struct ftm_regs {
	u32 sc;		/* 0x00 */
	u32 cnt;	/* 0x04 */
	u32 mod;	/* 0x08 */
	u32 c0sc;	/* 0x0C */
	u32 c0v;	/* 0x10 */
	u32 c1sc;	/* 0x14 */
	u32 c1v;	/* 0x18 */
	u32 c2sc;	/* 0x1C */
	u32 c2v;	/* 0x20 */
	u32 c3sc;	/* 0x24 */
	u32 c3v;	/* 0x28 */
	u32 c4sc;	/* 0x2C */
	u32 c4v;	/* 0x30 */
	u32 c5sc;	/* 0x34 */
	u32 c5v;	/* 0x38 */
	u32 c6sc;	/* 0x3C */
	u32 c6v;	/* 0x40 */
	u32 c7sc;	/* 0x44 */
	u32 c7v;	/* 0x48 */
	u32 cntin;	/* 0x4C */
	u32 status;	/* 0x50 */
	u32 mode;	/* 0x54 */
	u32 sync;	/* 0x58 */
	u32 outinit;	/* 0x5C */
	u32 outmask;	/* 0x60 */
	u32 combine;	/* 0x64 */
	u32 deadtime;	/* 0x68 */
	u32 exttrig;	/* 0x6C */
	u32 pol;	/* 0x70 */
	u32 fms;	/* 0x74 */
	u32 filter;	/* 0x78 */
	u32 fltctrl;	/* 0x7C */
	u32 qdctrl;	/* 0x80 */
	u32 conf;	/* 0x84 */
	u32 fltpol;	/* 0x88 */
	u32 synconf;	/* 0x8C */
	u32 invctrl;	/* 0x90 */
	u32 swoctrl;	/* 0x94 */
	u32 pwmload;	/* 0x98 */
};

struct pit2_regs {
	u32 ldval;	/* 0x00 */
	u32 cval;	/* 0x04 */
	u32 tctrl;	/* 0x08 */
	u32 tflg;	/* 0x0C */
};

struct pit_regs {
	u32 mcr;	/* 0x00 */
	u32 rsvd0[55];
	u32 ltmr64h;	/* 0xE0 */
	u32 ltmr64l;	/* 0xE4 */
	u32 rsvd1[6];
	struct pit2_regs counter[8];	/* 0x100 */
};

struct lptmr_regs {
	u32 csr;	/* 0x00 */
	u32 psr;	/* 0x04 */
	u32 cmr;	/* 0x08 */
	u32 cnr;	/* 0x0C */
};

/* Flex Timer */
#define FTM_SC			(FTM_BASE_ADDR + 0x00)
#define FTM_CNT			(FTM_BASE_ADDR + 0x04)
#define FTM_MOD			(FTM_BASE_ADDR + 0x08)
#define FTM_C0SC		(FTM_BASE_ADDR + 0x0C)
#define FTM_C0V			(FTM_BASE_ADDR + 0x10)
#define FTM_C1SC		(FTM_BASE_ADDR + 0x14)
#define FTM_C1V			(FTM_BASE_ADDR + 0x18)
#define FTM_C2SC		(FTM_BASE_ADDR + 0x1C)
#define FTM_C2V			(FTM_BASE_ADDR + 0x20)
#define FTM_C3SC		(FTM_BASE_ADDR + 0x24)
#define FTM_C3V			(FTM_BASE_ADDR + 0x28)
#define FTM_C4SC		(FTM_BASE_ADDR + 0x2C)
#define FTM_C4V			(FTM_BASE_ADDR + 0x30)
#define FTM_C5SC		(FTM_BASE_ADDR + 0x34)
#define FTM_C5V			(FTM_BASE_ADDR + 0x38)
#define FTM_C6SC		(FTM_BASE_ADDR + 0x3C)
#define FTM_C6V			(FTM_BASE_ADDR + 0x40)
#define FTM_C7SC		(FTM_BASE_ADDR + 0x44)
#define FTM_C7V			(FTM_BASE_ADDR + 0x48)
#define FTM_CNTIN		(FTM_BASE_ADDR + 0x4C)
#define FTM_STATUS		(FTM_BASE_ADDR + 0x50)
#define FTM_MODE		(FTM_BASE_ADDR + 0x54)
#define FTM_SYNC		(FTM_BASE_ADDR + 0x58)
#define FTM_OUTINIT		(FTM_BASE_ADDR + 0x5C)
#define FTM_OUTMASK		(FTM_BASE_ADDR + 0x60)
#define FTM_COMBINE		(FTM_BASE_ADDR + 0x64)
#define FTM_DEADTIME		(FTM_BASE_ADDR + 0x68)
#define FTM_EXTTRIG		(FTM_BASE_ADDR + 0x6C)
#define FTM_POL			(FTM_BASE_ADDR + 0x70)
#define FTM_FMS			(FTM_BASE_ADDR + 0x74)
#define FTM_FILTER		(FTM_BASE_ADDR + 0x78)
#define FTM_FLTCTRL		(FTM_BASE_ADDR + 0x7C)
#define FTM_QDCTRL		(FTM_BASE_ADDR + 0x80)
#define FTM_CONF		(FTM_BASE_ADDR + 0x84)
#define FTM_FLTPOL		(FTM_BASE_ADDR + 0x88)
#define FTM_SYNCONF		(FTM_BASE_ADDR + 0x8C)
#define FTM_INVCTRL		(FTM_BASE_ADDR + 0x90)
#define FTM_SWOCTRL		(FTM_BASE_ADDR + 0x94)
#define FTM_PWMLOAD		(FTM_BASE_ADDR + 0x98)

#define FTM_SC_TOF		(1 << 7)
#define FTM_SC_TOIE		(1 << 6)
#define FTM_SC_CPWMS		(1 << 5)
#define FTM_SC_CLKS(x)		((x & 3) << 3)
#define FTM_SC_PS(x)		((x & 7) << 0)

#define FTM_CNT_COUNT(x)	(x & 0xFFFF)

#define FTM_MOD_MOD(x)		(x & 0xFFFF)

#define FTM_CnSC_CHF		(1 << 7)
#define FTM_CnSC_CHIE		(1 << 6)
#define FTM_CnSC_MSB		(1 << 5)
#define FTM_CnSC_MSA		(1 << 4)
#define FTM_CnSC_ELSB		(1 << 3)
#define FTM_CnSC_ELSA		(1 << 2)
#define FTM_CnSC_DMA		(1 << 0)

#define FTM_CnV_VAL(x)		(x & 0xFFFF)

#define FTM_CNTIN_INIT(x)	(x & 0xFFFF)

#define FTM_STATUS_CH7F		(1 << 7)
#define FTM_STATUS_CH6F		(1 << 6)
#define FTM_STATUS_CH5F		(1 << 5)
#define FTM_STATUS_CH4F		(1 << 4)
#define FTM_STATUS_CH3F		(1 << 3)
#define FTM_STATUS_CH2F		(1 << 2)
#define FTM_STATUS_CH1F		(1 << 1)
#define FTM_STATUS_CH0F		(1 << 0)

#define FTM_MODE_FAULTIE	(1 << 7)
#define FTM_MODE_FAULTM(x)	((x & 3) << 5)
#define FTM_MODE_CAPTEST	(1 << 4)
#define FTM_MODE_PWMSYNC	(1 << 3)
#define FTM_MODE_WPDIS		(1 << 2)
#define FTM_MODE_INIT		(1 << 1)
#define FTM_MODE_FTMEN		(1 << 0)

#define FTM_SYNC_SWSYNC		(1 << 7)
#define FTM_SYNC_TRIG2		(1 << 6)
#define FTM_SYNC_TRIG1		(1 << 5)
#define FTM_SYNC_TRIG0		(1 << 4)
#define FTM_SYNC_SYNCHOM	(1 << 3)
#define FTM_SYNC_REINIT		(1 << 2)
#define FTM_SYNC_CNTMAX		(1 << 1)
#define FTM_SYNC_CNTMIN		(1 << 0)

#define FTM_OUTINIT_CH7OI	(1 << 7)
#define FTM_OUTINIT_CH6OI	(1 << 6)
#define FTM_OUTINIT_CH5OI	(1 << 5)
#define FTM_OUTINIT_CH4OI	(1 << 4)
#define FTM_OUTINIT_CH3OI	(1 << 3)
#define FTM_OUTINIT_CH2OI	(1 << 2)
#define FTM_OUTINIT_CH1OI	(1 << 1)
#define FTM_OUTINIT_CH0OI	(1 << 0)

#define FTM_OUTMASK_CH7OM	(1 << 7)
#define FTM_OUTMASK_CH6OM	(1 << 6)
#define FTM_OUTMASK_CH5OM	(1 << 5)
#define FTM_OUTMASK_CH4OM	(1 << 4)
#define FTM_OUTMASK_CH3OM	(1 << 3)
#define FTM_OUTMASK_CH2OM	(1 << 2)
#define FTM_OUTMASK_CH1OM	(1 << 1)
#define FTM_OUTMASK_CH0OM	(1 << 0)

#define FTM_COMBINE_FAULTEN3	(1 << 30)
#define FTM_COMBINE_SYNCEN3	(1 << 29)
#define FTM_COMBINE_DTEN3	(1 << 28)
#define FTM_COMBINE_DECAP3	(1 << 27)
#define FTM_COMBINE_DECAPEN3	(1 << 26)
#define FTM_COMBINE_COMP3	(1 << 25)
#define FTM_COMBINE_COMBINE3	(1 << 24)
#define FTM_COMBINE_FAULTEN2	(1 << 22)
#define FTM_COMBINE_SYNCEN2	(1 << 21)
#define FTM_COMBINE_DTEN2	(1 << 20)
#define FTM_COMBINE_DECAP2	(1 << 19)
#define FTM_COMBINE_DECAPEN2	(1 << 18)
#define FTM_COMBINE_COMP2	(1 << 17)
#define FTM_COMBINE_COMBINE2	(1 << 16)
#define FTM_COMBINE_FAULTEN1	(1 << 14)
#define FTM_COMBINE_SYNCEN1	(1 << 13)
#define FTM_COMBINE_DTEN1	(1 << 12)
#define FTM_COMBINE_DECAP1	(1 << 11)
#define FTM_COMBINE_DECAPEN1	(1 << 10)
#define FTM_COMBINE_COMP1	(1 << 9)
#define FTM_COMBINE_COMBINE1	(1 << 8)
#define FTM_COMBINE_FAULTEN0	(1 << 6)
#define FTM_COMBINE_SYNCEN0	(1 << 5)
#define FTM_COMBINE_DTEN0	(1 << 4)
#define FTM_COMBINE_DECAP0	(1 << 3)
#define FTM_COMBINE_DECAPEN0	(1 << 2)
#define FTM_COMBINE_COMP0	(1 << 1)
#define FTM_COMBINE_COMBINE0	(1 << 0)

#define FTM_DEADTIME_DTPS(x)	((x & 3) << 6)
#define FTM_DEADTIME_DTVAL(x)	((x & 0x3F) << 0)

#define FTM_EXTTRIG_TRIGF	(1 << 7)
#define FTM_EXTTRIG_INITTRIGEN	(1 << 6)
#define FTM_EXTTRIG_CH1TRIG	(1 << 5)
#define FTM_EXTTRIG_CH0TRIG	(1 << 4)
#define FTM_EXTTRIG_CH5TRIG	(1 << 3)
#define FTM_EXTTRIG_CH4TRIG	(1 << 2)
#define FTM_EXTTRIG_CH3TRIG	(1 << 1)
#define FTM_EXTTRIG_CH2TRIG	(1 << 0)

#define FTM_POL_POL7		(1 << 7)
#define FTM_POL_POL6		(1 << 6)
#define FTM_POL_POL5		(1 << 5)
#define FTM_POL_POL4		(1 << 4)
#define FTM_POL_POL3		(1 << 3)
#define FTM_POL_POL2		(1 << 2)
#define FTM_POL_POL1		(1 << 1)
#define FTM_POL_POL0		(1 << 0)

#define FTM_FMS_FAULTF		(1 << 7)
#define FTM_FMS_WPEN		(1 << 6)
#define FTM_FMS_FAULTIN		(1 << 5)
#define FTM_FMS_FAULTF3		(1 << 3)
#define FTM_FMS_FAULTF2		(1 << 2)
#define FTM_FMS_FAULTF1		(1 << 1)
#define FTM_FMS_FAULTF0		(1 << 0)

#define FTM_FILTER_CH3FVAL(x)	((x & 0xF) << 12)
#define FTM_FILTER_CH2FVAL(x)	((x & 0xF) << 8)
#define FTM_FILTER_CH1FVAL(x)	((x & 0xF) << 4)
#define FTM_FILTER_CH0FVAL(x)	((x & 0xF) << 0)

#define FTM_FLTCTRL_FFVAL(x)	((x & 0xF) << 8)
#define FTM_FLTCTRL_FFLTR3EN	(1 << 7)
#define FTM_FLTCTRL_FFLTR2EN	(1 << 6)
#define FTM_FLTCTRL_FFLTR1EN	(1 << 5)
#define FTM_FLTCTRL_FFLTR0EN	(1 << 4)
#define FTM_FLTCTRL_FAULT3EN	(1 << 3)
#define FTM_FLTCTRL_FAULT2EN	(1 << 2)
#define FTM_FLTCTRL_FAULT1EN	(1 << 1)
#define FTM_FLTCTRL_FAULT0EN	(1 << 0)

#define FTM_QDCTRL_PHAFLTREN	(1 << 7)
#define FTM_QDCTRL_PHBFLTREN	(1 << 6)
#define FTM_QDCTRL_PHAPOL	(1 << 5)
#define FTM_QDCTRL_PHBPOL	(1 << 4)
#define FTM_QDCTRL_QUADMODE	(1 << 3)
#define FTM_QDCTRL_QUADIR	(1 << 2)
#define FTM_QDCTRL_TOFDIR	(1 << 1)
#define FTM_QDCTRL_QUADEN	(1 << 0)

#define FTM_CONF_GTBEOUT	(1 << 10)
#define FTM_CONF_GTBEEN		(1 << 9)
#define FTM_CONF_BDMMODE(x)	((x & 3) << 6)
#define FTM_CONF_NUMTOF(x)	((x & 0x1F) << 0)

#define FTM_FLTPOL_FLT3POL	(1 << 3)
#define FTM_FLTPOL_FLT2POL	(1 << 2)
#define FTM_FLTPOL_FLT1POL	(1 << 1)
#define FTM_FLTPOL_FLT0POL	(1 << 0)

#define FTM_SYNCONF_HWSOC	(1 << 20)
#define FTM_SYNCONF_HWINVC	(1 << 19)
#define FTM_SYNCONF_HWOM	(1 << 18)
#define FTM_SYNCONF_HWWRBUF	(1 << 17)
#define FTM_SYNCONF_HWRSTCNT	(1 << 16)
#define FTM_SYNCONF_SWSOC	(1 << 12)
#define FTM_SYNCONF_SWINVC	(1 << 11)
#define FTM_SYNCONF_SWOM	(1 << 10)
#define FTM_SYNCONF_SWWRBUF	(1 << 9)
#define FTM_SYNCONF_SWRSTCNT	(1 << 8)
#define FTM_SYNCONF_SYNCMODE	(1 << 7)
#define FTM_SYNCONF_SWOC	(1 << 5)
#define FTM_SYNCONF_INVC	(1 << 4)
#define FTM_SYNCONF_CNTINC	(1 << 2)
#define FTM_SYNCONF_HWTRIGMODE	(1 << 0)

#define FTM_INVCTRL_INV3EN	(1 << 3)
#define FTM_INVCTRL_INV2EN	(1 << 2)
#define FTM_INVCTRL_INV1EN	(1 << 1)
#define FTM_INVCTRL_INV0EN	(1 << 0)

#define FTM_SWOCTRL_CH7OCV	(1 << 15)
#define FTM_SWOCTRL_CH6OCV	(1 << 14)
#define FTM_SWOCTRL_CH5OCV	(1 << 13)
#define FTM_SWOCTRL_CH4OCV	(1 << 12)
#define FTM_SWOCTRL_CH3OCV	(1 << 11)
#define FTM_SWOCTRL_CH2OCV	(1 << 10)
#define FTM_SWOCTRL_CH1OCV	(1 << 9)
#define FTM_SWOCTRL_CH0OCV	(1 << 8)
#define FTM_SWOCTRL_CH7OC	(1 << 7)
#define FTM_SWOCTRL_CH6OC	(1 << 6)
#define FTM_SWOCTRL_CH5OC	(1 << 5)
#define FTM_SWOCTRL_CH4OC	(1 << 4)
#define FTM_SWOCTRL_CH3OC	(1 << 3)
#define FTM_SWOCTRL_CH2OC	(1 << 2)
#define FTM_SWOCTRL_CH1OC	(1 << 1)
#define FTM_SWOCTRL_CH0OC	(1 << 0)

#define FTM_PWMLOAD_LDOK	(1 << 9)
#define FTM_PWMLOAD_CH7SEL	(1 << 7)
#define FTM_PWMLOAD_CH6SEL	(1 << 6)
#define FTM_PWMLOAD_CH5SEL	(1 << 5)
#define FTM_PWMLOAD_CH4SEL	(1 << 4)
#define FTM_PWMLOAD_CH3SEL	(1 << 3)
#define FTM_PWMLOAD_CH2SEL	(1 << 2)
#define FTM_PWMLOAD_CH1SEL	(1 << 1)
#define FTM_PWMLOAD_CH0SEL	(1 << 0)

/* Periodic Interrupt Timer */
#define PIT_MCR			(PIT_BASE_ADDR + 0x00)
#define PIT_LTMR64H		(PIT_BASE_ADDR + 0xE0)
#define PIT_LTMR64L		(PIT_BASE_ADDR + 0xE4)
#define PIT_LDVAL0		(PIT_BASE_ADDR + 0x100)
#define PIT_CVAL0		(PIT_BASE_ADDR + 0x104)
#define PIT_TCTRL0		(PIT_BASE_ADDR + 0x108)
#define PIT_TFLG0		(PIT_BASE_ADDR + 0x10C)
#define PIT_LDVAL1		(PIT_BASE_ADDR + 0x110)
#define PIT_CVAL1		(PIT_BASE_ADDR + 0x114)
#define PIT_TCTRL1		(PIT_BASE_ADDR + 0x118)
#define PIT_TFLG1		(PIT_BASE_ADDR + 0x11C)
#define PIT_LDVAL2		(PIT_BASE_ADDR + 0x120)
#define PIT_CVAL2		(PIT_BASE_ADDR + 0x124)
#define PIT_TCTRL2		(PIT_BASE_ADDR + 0x128)
#define PIT_TFLG2		(PIT_BASE_ADDR + 0x12C)
#define PIT_LDVAL3		(PIT_BASE_ADDR + 0x130)
#define PIT_CVAL3		(PIT_BASE_ADDR + 0x134)
#define PIT_TCTRL3		(PIT_BASE_ADDR + 0x138)
#define PIT_TFLG3		(PIT_BASE_ADDR + 0x13C)
#define PIT_LDVAL4		(PIT_BASE_ADDR + 0x140)
#define PIT_CVAL4		(PIT_BASE_ADDR + 0x144)
#define PIT_TCTRL4		(PIT_BASE_ADDR + 0x148)
#define PIT_TFLG4		(PIT_BASE_ADDR + 0x14C)
#define PIT_LDVAL5		(PIT_BASE_ADDR + 0x150)
#define PIT_CVAL5		(PIT_BASE_ADDR + 0x154)
#define PIT_TCTRL5		(PIT_BASE_ADDR + 0x158)
#define PIT_TFLG5		(PIT_BASE_ADDR + 0x15C)
#define PIT_LDVAL6		(PIT_BASE_ADDR + 0x160)
#define PIT_CVAL6		(PIT_BASE_ADDR + 0x164)
#define PIT_TCTRL6		(PIT_BASE_ADDR + 0x168)
#define PIT_TFLG6		(PIT_BASE_ADDR + 0x16C)
#define PIT_LDVAL7		(PIT_BASE_ADDR + 0x170)
#define PIT_CVAL7		(PIT_BASE_ADDR + 0x174)
#define PIT_TCTRL7		(PIT_BASE_ADDR + 0x178)
#define PIT_TFLG7		(PIT_BASE_ADDR + 0x17C)

#define PIT_MCR_MDIS		(1 << 1)
#define PIT_MCR_FRZ		(1 << 0)

#define PIT_TCTRL_CHN		(1 << 2)
#define PIT_TCTRL_TIE		(1 << 1)
#define PIT_TCTRL_TEN		(1 << 0)

#define PIT_TFLG_TIF		(1 << 0)

/* Low Power Timer */
#define LPTMR_CSR		(LPTMR_BASE_ADDR + 0x00)
#define LPTMR_PSR		(LPTMR_BASE_ADDR + 0x04)
#define LPTMR_CMR		(LPTMR_BASE_ADDR + 0x08)
#define LPTMR_CNR		(LPTMR_BASE_ADDR + 0x0C)

#define LPTMR_CSR_TCF		(1 << 7)
#define LPTMR_CSR_TIE		(1 << 6)
#define LPTMR_CSR_TPS(x)	((x & 3) << 4)
#define LPTMR_CSR_TPP		(1 << 3)
#define LPTMR_CSR_TFC		(1 << 2)
#define LPTMR_CSR_TMS		(1 << 1)
#define LPTMR_CSR_TEN		(1 << 0)

#define LPTMR_PSR_PRESCALE(x)	((x & 0x0F) << 3)
#define LPTMR_PSR_PBYP		(1 << 2)
#define LPTMR_PSR_PCS(x)	((x & 0x03) << 0)

#define LPTMR_CMR_COMPARE(x)	(x & 0xFFFF)
#define LPTMR_CNR_COUNTER(x)	(x & 0xFFFF)

#endif /* __ASM_ARCH_TIMER_H */
