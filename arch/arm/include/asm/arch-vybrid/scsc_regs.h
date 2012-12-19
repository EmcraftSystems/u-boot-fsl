/*
 * Copyright 2012 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright 2012 Timesys Corporation All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */


#ifndef __ARCH_ARM_MACH_VYBRID_SCSC_REGS_H__
#define __ARCH_ARM_MACH_VYBRID_SCSC_REGS_H__

struct vybrid_scsc_reg {
	u32 sicr_ctr;	/* 0x0000 */
	u32 sosc_ctr;
	u32 lfsr_ctr;
};

/* Define the bits in register sicr_ctr */
#define VYBRID_SCSC_SICR_CTR_SIRC_DIV_MASK		(0x7F << 8)
#define VYBRID_SCSC_SICR_CTR_SIRC_DIV_OFFSET		8
#define VYBRID_SCSC_SICR_CTR_SIRC_SIRC_ON_FAIL		(1 << 4)
#define VYBRID_SCSC_SICR_CTR_SIRC_SIRC_EN		(1 << 0)

/* Define the bits in register sosc_ctr */
#define VYBRID_SCSC_SICR_CTR_SOSC_BYPASS		(1 << 3)
#define VYBRID_SCSC_SICR_CTR_SOSC_EN			(1 << 0)

#define VYBRID_SCSC_LFSR_POLY_LFSR_MASK			(0xFFFF << 16)
#define VYBRID_SCSC_LFSR_POLY_LFSR_OFFSET		16
#define VYBRID_SCSC_LFSR_EN_TZASC_VIO			(1 << 14)
#define VYBRID_SCSC_LFSR_LFSR1_SEED 			(1 << 13)
#define VYBRID_SCSC_LFSR_LFSR0_SEED 			(1 << 12)
#define VYBRID_SCSC_LFSR_ACTIVE_SEL_4_5			(1 << 11)
#define VYBRID_SCSC_LFSR_ACTIVE_SEL_2_3			(1 << 10)
#define VYBRID_SCSC_LFSR_TAMPER5_INV			(1 << 9)
#define VYBRID_SCSC_LFSR_TAMPER4_INV			(1 << 8)
#define VYBRID_SCSC_LFSR_TAMPER3_INV			(1 << 7)
#define VYBRID_SCSC_LFSR_TAMPER2_INV			(1 << 6)
#define VYBRID_SCSC_LFSR_TAMPER1_INV			(1 << 5)
#define VYBRID_SCSC_LFSR_TAMPER0_INV			(1 << 4)

#endif /*__ARCH_ARM_MACH_VYBRID_SCSC_REGS_H__ */
