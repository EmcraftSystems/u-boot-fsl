/*
 * Copyright (C) 2007, Guennadi Liakhovetski <lg@denx.de>
 *
 * Copyright 2012 Freescale Semiconductor, Inc.
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

#include <config.h>
#include <asm/arch/vybrid-regs.h>
#include <generated/asm-offsets.h>

/* VFPU setup/invalidation/disable */
.macro init_vfpu
	ldr r0, =(0xF << 20)
	mcr p15, 0, r0, c1, c0, 2
	mov r3, #0x40000000
	.long 0xeee83a10
/*	vmsr FPEXC, r3 */
.endm /* init_vfpu */

/* L2CC Cache setup/invalidation/disable */
.macro init_l2cc
	/* explicitly disable L2 cache */
	mrc 15, 0, r0, c1, c0, 1
	bic r0, r0, #0x2
	mcr 15, 0, r0, c1, c0, 1

	/* reconfigure L2 cache aux control reg */
	mov r0, #0xC0			/* tag RAM */
	add r0, r0, #0x4		/* data RAM */
	orr r0, r0, #(1 << 24)		/* disable write allocate delay */
	orr r0, r0, #(1 << 23)		/* disable write allocate combine */
	orr r0, r0, #(1 << 22)		/* disable write allocate */

	ldr r1, =0x00000000
	ldr r3, [r1, #ROM_SI_REV]	/* problem ********* */
	cmp r3, #0x10    /* r3 contains the silicon rev */
	/* disable write combine for TO 2 and lower revs */
	orrls r0, r0, #(1 << 25)

	mcr 15, 1, r0, c9, c0, 2
.endm /* init_l2cc */

/*
 * AIPS setup - Only setup MPROTx registers.
 * The PACR default values are good.
 */
.macro init_aips
	/*
	 * Set all MPROTx to be non-bufferable, trusted for R/W,
	 * not forced to user-mode.
	 */
	ldr r0, =AIPS0_BASE_ADDR
	ldr r1, =0x77777000
	str r1, [r0, #0x0]
	str r1, [r0, #0x4]
	ldr r0, =AIPS1_BASE_ADDR
	str r1, [r0, #0x0]
	str r1, [r0, #0x4]
.endm /* init_aips */

/* MAX (Multi-Layer AHB Crossbar Switch) setup */
.macro init_max
.endm /* init_max */

/* M4IF setup */
.macro init_m4if
.endm /* init_m4if */

/* DDR */
.macro  init_drive_strength
.endm /* init_drive_strength */

.macro setup_pll pll, freq
.endm

.macro init_clock
	ldr r0, =CCM_BASE_ADDR

	ldr r1, =CONFIG_SYS_CLKCTRL_CLPCR
	str r1, [r0, #CLKCTL_CLPCR]

	/* Gate of clocks to the peripherals first */
	ldr r1, =CONFIG_SYS_CLKCTL_CCGR0
	str r1, [r0, #CLKCTL_CCGR0]
	ldr r1, =CONFIG_SYS_CLKCTL_CCGR1
	str r1, [r0, #CLKCTL_CCGR1]
	ldr r1, =CONFIG_SYS_CLKCTL_CCGR2
	str r1, [r0, #CLKCTL_CCGR2]
	ldr r1, =CONFIG_SYS_CLKCTL_CCGR3
	str r1, [r0, #CLKCTL_CCGR3]
	ldr r1, =CONFIG_SYS_CLKCTL_CCGR4
	str r1, [r0, #CLKCTL_CCGR4]
	ldr r1, =CONFIG_SYS_CLKCTL_CCGR5
	str r1, [r0, #CLKCTL_CCGR5]
	ldr r1, =CONFIG_SYS_CLKCTL_CCGR6
	str r1, [r0, #CLKCTL_CCGR6]
	ldr r1, =CONFIG_SYS_CLKCTL_CCGR7
	str r1, [r0, #CLKCTL_CCGR7]
	ldr r1, =CONFIG_SYS_CLKCTL_CCGR8
	str r1, [r0, #CLKCTL_CCGR8]
	ldr r1, =CONFIG_SYS_CLKCTL_CCGR9
	str r1, [r0, #CLKCTL_CCGR9]
	ldr r1, =CONFIG_SYS_CLKCTL_CCGR10
	str r1, [r0, #CLKCTL_CCGR10]
	ldr r1, =CONFIG_SYS_CLKCTL_CCGR11
	str r1, [r0, #CLKCTL_CCGR11]

	ldr r2, =ANATOP_BASE_ADDR
	ldr r1, =CONFIG_SYS_ANADIG_528_CTRL
	str r1, [r2, #0x30]
	ldr r1, =CONFIG_SYS_ANADIG_ENET_CTRL
	str r1, [r2, #0xE0]
	ldr r1, =CONFIG_SYS_ANADIG_SYS_CTRL
	str r1, [r2, #0x270]
	/* check for lock */

	ldr r1, =CONFIG_SYS_CLKCTRL_CCR
	str r1, [r0, #CLKCTL_CCR]

	ldr r1, =CONFIG_SYS_CLKCTRL_CCSR
	str r1, [r0, #CLKCTL_CCSR]

	ldr r1, =CONFIG_SYS_CLKCTRL_CACRR
	str r1, [r0, #CLKCTL_CACRR]

	ldr r1, =CONFIG_SYS_CLKCTRL_CSCMR1
	str r1, [r0, #CLKCTL_CSCMR1]

	ldr r1, =CONFIG_SYS_CLKCTRL_CSCDR1
	str r1, [r0, #CLKCTL_CSCDR1]

	ldr r1, =CONFIG_SYS_CLKCTRL_CSCDR2
	str r1, [r0, #CLKCTL_CSCDR2]

	ldr r1, =CONFIG_SYS_CLKCTRL_CSCDR3
	str r1, [r0, #CLKCTL_CSCDR3]

	ldr r1, =CONFIG_SYS_CLKCTRL_CSCMR2
	str r1, [r0, #CLKCTL_CSCMR2]

	ldr r1, =CONFIG_SYS_CLKCTRL_CSCDR4
	str r1, [r0, #CLKCTL_CSCDR4]
.endm

.macro setup_wdog
	ldr r0, =WDOG_A5_BASE_ADDR
	mov r1, #0x30
	strh r1, [r0]
.endm

.section ".text.init", "x"

.globl lowlevel_init
lowlevel_init:
/*
	ldr r0, =GPIO1_BASE_ADDR
	ldr r1, [r0, #0x0]
	orr r1, r1, #(1 << 23)
	str r1, [r0, #0x0]
	ldr r1, [r0, #0x4]
	orr r1, r1, #(1 << 23)
	str r1, [r0, #0x4]
*/
	/* ARM errata ID #468414 */
	mrc 15, 0, r1, c1, c0, 1
	orr r1, r1, #(1 << 5)    /* enable L1NEON bit */
	mcr 15, 0, r1, c1, c0, 1
/*
	init_vfpu

	init_l2cc

	init_aips

	init_max

	init_m4if

	init_drive_strength
*/
	init_clock

	/* return from mxc_nand_load */
	/* b mxc_nand_load */

	/* r12 saved upper lr*/
	mov pc,lr

/* Board level setting value */
DDR_PERCHARGE_CMD:	.word 0x04008008
DDR_REFRESH_CMD:	.word 0x00008010
DDR_LMR1_W:		.word 0x00338018
DDR_LMR_CMD:		.word 0xB2220000
DDR_TIMING_W:		.word 0xB02567A9
DDR_MISC_W:		.word 0x000A0104
