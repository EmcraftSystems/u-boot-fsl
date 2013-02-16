/*
 * Copyright 2012 Freescale Semiconductor, Inc.
 * Adapted from Linux v2.6.36 kernel: arch/powerpc/kernel/asm-offsets.c
 *
 * This program is used to generate definitions needed by
 * assembly language modules.
 *
 * We use the technique used in the OSF Mach kernel code:
 * generate asm statements containing #defines,
 * compile this file to assembler, and then extract the
 * #defines from the assembly-language output.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <common.h>
#include <asm/arch/vybrid-regs.h>

#include <linux/kbuild.h>

int main(void)
{
	/* Round up to make sure size gives nice stack alignment */
	DEFINE(CLKCTL_CCR, offsetof(struct clkctl, ccr));
	DEFINE(CLKCTL_CSR, offsetof(struct clkctl, csr));
	DEFINE(CLKCTL_CCSR, offsetof(struct clkctl, ccsr));
	DEFINE(CLKCTL_CACRR, offsetof(struct clkctl, cacrr));
	DEFINE(CLKCTL_CSCMR1, offsetof(struct clkctl, cscmr1));
	DEFINE(CLKCTL_CSCDR1, offsetof(struct clkctl, cscdr1));
	DEFINE(CLKCTL_CSCDR2, offsetof(struct clkctl, cscdr2));
	DEFINE(CLKCTL_CSCDR3, offsetof(struct clkctl, cscdr3));
	DEFINE(CLKCTL_CSCMR2, offsetof(struct clkctl, cscmr2));
	DEFINE(CLKCTL_CSCDR4, offsetof(struct clkctl, cscdr4));
	DEFINE(CLKCTL_CTOR, offsetof(struct clkctl, ctor));
	DEFINE(CLKCTL_CLPCR, offsetof(struct clkctl, clpcr));
	DEFINE(CLKCTL_CISR, offsetof(struct clkctl, cisr));
	DEFINE(CLKCTL_CIMR, offsetof(struct clkctl, cimr));
	DEFINE(CLKCTL_CCOSR, offsetof(struct clkctl, ccosr));

	DEFINE(CLKCTL_CGPR, offsetof(struct clkctl, cgpr));
	DEFINE(CLKCTL_CCGR0, offsetof(struct clkctl, ccgr0));
	DEFINE(CLKCTL_CCGR1, offsetof(struct clkctl, ccgr1));
	DEFINE(CLKCTL_CCGR2, offsetof(struct clkctl, ccgr2));
	DEFINE(CLKCTL_CCGR3, offsetof(struct clkctl, ccgr3));
	DEFINE(CLKCTL_CCGR4, offsetof(struct clkctl, ccgr4));
	DEFINE(CLKCTL_CCGR5, offsetof(struct clkctl, ccgr5));
	DEFINE(CLKCTL_CCGR6, offsetof(struct clkctl, ccgr6));
	DEFINE(CLKCTL_CCGR7, offsetof(struct clkctl, ccgr7));
	DEFINE(CLKCTL_CCGR8, offsetof(struct clkctl, ccgr8));
	DEFINE(CLKCTL_CCGR9, offsetof(struct clkctl, ccgr9));
	DEFINE(CLKCTL_CCGR10, offsetof(struct clkctl, ccgr10));
	DEFINE(CLKCTL_CCGR11, offsetof(struct clkctl, ccgr11));
	DEFINE(CLKCTL_CMEOR0, offsetof(struct clkctl, cmeor0));
	DEFINE(CLKCTL_CMEOR1, offsetof(struct clkctl, cmeor1));
	DEFINE(CLKCTL_CMEOR2, offsetof(struct clkctl, cmeor2));
	DEFINE(CLKCTL_CMEOR3, offsetof(struct clkctl, cmeor3));
	DEFINE(CLKCTL_CMEOR4, offsetof(struct clkctl, cmeor4));
	DEFINE(CLKCTL_CMEOR5, offsetof(struct clkctl, cmeor5));
	DEFINE(CLKCTL_CPPDSR, offsetof(struct clkctl, cppdsr));
	DEFINE(CLKCTL_CCOWR, offsetof(struct clkctl, ccowr));
	DEFINE(CLKCTL_CCPGR0, offsetof(struct clkctl, ccpgr0));
	DEFINE(CLKCTL_CCPGR1, offsetof(struct clkctl, ccpgr1));
	DEFINE(CLKCTL_CCPGR2, offsetof(struct clkctl, ccpgr2));
	DEFINE(CLKCTL_CCPGR3, offsetof(struct clkctl, ccpgr3));

	return 0;
}
