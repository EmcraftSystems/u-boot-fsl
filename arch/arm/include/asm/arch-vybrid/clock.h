/*
 * (C) Copyright 2009
 * Stefano Babic, DENX Software Engineering, sbabic@denx.de.
 *
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

#ifndef __ASM_ARCH_CLOCK_H
#define __ASM_ARCH_CLOCK_H

enum vybrid_clock {
	VYBRID_ARM_CLK = 0,
	VYBRID_AHB_CLK,
	VYBRID_IPG_CLK,
	VYBRID_IPG_PERCLK,
	VYBRID_UART_CLK,
	VYBRID_CSPI_CLK,
	VYBRID_FEC_CLK,
};

unsigned int vybrid_decode_pll(unsigned int pll, unsigned int f_ref);

u32 vybrid_get_uartclk(void);
u32 vybrid_get_fecclk(void);
unsigned int vybrid_get_clock(enum vybrid_clock clk);

void set_usb_phy2_clk(void);
void enable_usb_phy2_clk(unsigned char enable);
void set_usboh3_clk(void);
void enable_usboh3_clk(unsigned char enable);

#endif /* __ASM_ARCH_CLOCK_H */
