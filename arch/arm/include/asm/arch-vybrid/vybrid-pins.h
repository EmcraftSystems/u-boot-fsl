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

#ifndef __ASM_ARCH_VYBRID_PINS_H__
#define __ASM_ARCH_VYBRID_PINS_H__

#ifndef __ASSEMBLY__

/*!
 * Starting bit position within each entry of \b iomux_pins to represent the
 * MUX control register offset
 */
#define MUX_I			0
/*!
 * Starting bit position within each entry of \b iomux_pins to represent the
 * PAD control register offset
 */
#define PAD_I			10
/*!
 * Starting bit position within each entry of \b iomux_pins to represent which
 * mux mode is for GPIO (0-based)
 */
#define GPIO_I			21

#define MUX_IO_P                29
#define MUX_IO_I                24
#define IOMUX_TO_GPIO(pin)      ((((unsigned int)pin >> MUX_IO_P) * \
					GPIO_NUM_PIN) + ((pin >> MUX_IO_I) &\
					((1 << (MUX_IO_P - MUX_IO_I)) - 1)))
#define IOMUX_TO_IRQ(pin)       (MXC_GPIO_INT_BASE + IOMUX_TO_GPIO(pin))
#define GPIO_TO_PORT(n)         (n / GPIO_NUM_PIN)
#define GPIO_TO_INDEX(n)        (n % GPIO_NUM_PIN)

#define NON_GPIO_PORT		0x7
#define PIN_TO_MUX_MASK		((1 << (PAD_I - MUX_I)) - 1)
#define PIN_TO_PAD_MASK		((1 << (GPIO_I - PAD_I)) - 1)
#define PIN_TO_ALT_GPIO_MASK		((1 << (MUX_IO_I - GPIO_I)) - 1)

#define NON_MUX_I              PIN_TO_MUX_MASK
#define NON_PAD_I              PIN_TO_PAD_MASK

#define MUX_I_START		0x001C
#define PAD_I_START		0x3F0
#define INPUT_CTL_START		0x8C4
#define MUX_I_END		(PAD_I_START - 4)

#define _VYBRID_BUILD_PIN(gp, gi, ga, mi, pi) \
	(((gp) << MUX_IO_P) | ((gi) << MUX_IO_I) | \
	((mi) << MUX_I) | \
	((pi - PAD_I_START) << PAD_I) | \
	((ga) << GPIO_I))

#define _VYBRID_BUILD_GPIO_PIN(gp, gi, ga, mi, pi) \
	_VYBRID_BUILD_PIN(gp, gi, ga, mi, pi)

#define _VYBRID_BUILD_NON_GPIO_PIN(mi, pi) \
	_VYBRID_BUILD_PIN(NON_GPIO_PORT, 0, 0, mi, pi)

#define PIN_TO_IOMUX_MUX(pin)	((pin >> MUX_I) & PIN_TO_MUX_MASK)
#define PIN_TO_IOMUX_PAD(pin)	((pin >> PAD_I) & PIN_TO_PAD_MASK)
#define PIN_TO_ALT_GPIO(pin)	((pin >> GPIO_I) & PIN_TO_ALT_GPIO_MASK)
#define PIN_TO_IOMUX_INDEX(pin)	(PIN_TO_IOMUX_MUX(pin) >> 2)

typedef enum iomux_input_select {
	AUDMUX_P4_INPUT = 0,
} iomux_input_select_t;

#endif				/* __ASSEMBLY__ */
#endif				/* __ASM_ARCH_VYBRID_PINS_H__ */
