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
#include <asm/arch/vybrid-regs.h>
#include <asm/arch/vybrid-pins.h>
#include <asm/arch/iomux.h>
#include <asm/arch/sys_proto.h>

void pad_iomux_set(u32 pad_addr, pad_iomux_t *padio)
{
	u32 value;

	value = (((padio->mod & 7) << 20) | ((padio->spd & 3) << 12) |	\
		 ((padio->sre & 1) << 11) | ((padio->ode & 1) << 10) |	\
		 ((padio->hys & 1) << 9) | ((padio->dse & 7) << 6) |	\
		 ((padio->pus & 3) << 4) | ((padio->pke & 1) << 3) |	\
		 ((padio->pue & 1) << 2) | ((padio->obe & 1) << 1) |	\
		 ((padio->ibe & 1) << 0));

	__raw_writel(pad_addr, value);
}
