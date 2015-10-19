/*
 * Copyright (C) 2009
 * Guennadi Liakhovetski, DENX Software Engineering, <lg@denx.de>
 *
 * Copyright (C) 2011
 * Stefano Babic, DENX Software Engineering, <sbabic@denx.de>
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
#include <asm/gpio.h>
#include <asm/io.h>
#include <errno.h>

#define GPIO_PDOR	0x0
#define GPIO_PSOR	0x4
#define GPIO_PCOR	0x8
#define GPIO_PTOR	0xC
#define GPIO_PDIR	0x10

static unsigned long gpio_port_base(unsigned gpio)
{
	return GPIO_BASE_ADDR + (gpio / 32) * 0x40;
}

static int gpio_off(unsigned gpio)
{
	return gpio % 32;
}

int gpio_set_value(unsigned gpio, int value)
{
	unsigned long reg = gpio_port_base(gpio) + GPIO_PDOR;
	int off = gpio_off(gpio);
	int reg_val = (__raw_readl(reg) & (~(1 << off))) | (!!value << off);
	__raw_writel(reg_val, reg);
	return 0;
}

int gpio_get_value(unsigned gpio)
{
	unsigned long reg = gpio_port_base(gpio) + GPIO_PDIR;
	int off = gpio_off(gpio);
	return !!(__raw_readl(reg) & (1 << off));
}

int gpio_request(unsigned gpio, const char *label)
{
	return (gpio < (5*32)) ? 0 : -1;
}

int gpio_free(unsigned gpio)
{
	return 0;
}

int gpio_direction_input(unsigned gpio)
{
	return 0;
}

int gpio_direction_output(unsigned gpio, int value)
{
	gpio_set_value(gpio, value);
	return 0;
}
