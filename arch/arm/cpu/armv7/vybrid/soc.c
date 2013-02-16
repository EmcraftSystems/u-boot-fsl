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
#include <asm/arch/vybrid-regs.h>
#include <asm/arch/clock.h>
#include <asm/arch/sys_proto.h>

#include <asm/errno.h>
#include <asm/io.h>

#if !(defined(CONFIG_VYBRID))
#error "CPU_TYPE not defined"
#endif

u32 get_cpu_rev(void)
{
	int system_rev = 0x600000;
	int reg = __raw_readl(ROM_SI_REV);

	system_rev |= reg;
	return system_rev;
}

void set_chipselect_size(int const cs_size)
{
}
