/*
 * Copyright 2013 Emcraft Systems
 *
 * Pavel Boldin <paboldin@emcraft.com>
 *
 * Configuration settings for the Vybrid Boards LCDs
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */


#ifndef __VYRBID_LCD_CONFIG_H
#define __VYBRID_LCD_CONFIG_H

#if defined(LCD_TWR_RGB)

#	define LCD_XRES			480
#	define LCD_YRES			272
#	define LCD_BIT_PER_PIXEL	LCD_BPP

#	define LCD_LEFT_MARGIN	2
#	define LCD_HSYNC_LEN	41
#	define LCD_RIGHT_MARGIN	2

#	define LCD_UPPER_MARGIN	1
#	define LCD_VSYNC_LEN	2
#	define LCD_LOWER_MARGIN	1

#	define LCD_DCU_DIV_RATIO	9

#	define LCD_SYN_POL_INV_PXCK	0
#	define LCD_SYN_POL_NEG		0
#	define LCD_SYN_POL_INV_VS		1
#	define LCD_SYN_POL_INV_HS		1

#	define LCD_BOOTARG_NAME		"twr-lcd-rgb"

#elif defined(LCD_ADDON)

#	define LCD_XRES			480
#	define LCD_YRES			272
#	define LCD_BIT_PER_PIXEL	LCD_BPP

#	define LCD_LEFT_MARGIN	5
#	define LCD_HSYNC_LEN	2
#	define LCD_RIGHT_MARGIN	40

#	define LCD_UPPER_MARGIN	8
#	define LCD_VSYNC_LEN	2
#	define LCD_LOWER_MARGIN	8

#	define LCD_DCU_DIV_RATIO	9

#	define LCD_SYN_POL_INV_PXCK	1
#	define LCD_SYN_POL_NEG		0
#	define LCD_SYN_POL_INV_VS		1
#	define LCD_SYN_POL_INV_HS		1

#	define LCD_BOOTARG_NAME		"lcd-addon"

#elif defined(LCD_FUSION7)

#	define LCD_XRES			800
#	define LCD_YRES			480
#	define LCD_BIT_PER_PIXEL	LCD_BPP

#	define LCD_LEFT_MARGIN	43
#	define LCD_HSYNC_LEN	5
#	define LCD_RIGHT_MARGIN	104

#	define LCD_UPPER_MARGIN	20
#	define LCD_VSYNC_LEN	5
#	define LCD_LOWER_MARGIN	24

#	define LCD_DCU_DIV_RATIO	5

#	define LCD_SYN_POL_INV_PXCK		0
#	define LCD_SYN_POL_NEG		0
#	define LCD_SYN_POL_INV_VS		1
#	define LCD_SYN_POL_INV_HS		1

#	define LCD_BOOTARG_NAME		"twr-pim-fusion7"

#elif defined(LCD_PROMATE7)


#	define LCD_XRES			480
#	define LCD_YRES			800
#	define LCD_BIT_PER_PIXEL	LCD_BPP

#	define LCD_LEFT_MARGIN			0
#	define LCD_HSYNC_LEN			256
#	define LCD_RIGHT_MARGIN			0

#	define LCD_UPPER_MARGIN			0
#	define LCD_VSYNC_LEN			45
#	define LCD_LOWER_MARGIN			0

#	define LCD_DCU_DIV_RATIO		2

#	define LCD_SYN_POL_INV_PXCK		1
#	define LCD_SYN_POL_NEG			0
#	define LCD_SYN_POL_INV_VS		1
#	define LCD_SYN_POL_INV_HS		1

#	define LCD_BOOTARG_NAME		"promate7"

#elif defined(LCD_VF6_BSB)

#	define LCD_XRES			480
#	define LCD_YRES			272
#	define LCD_BIT_PER_PIXEL	LCD_BPP

#	define LCD_LEFT_MARGIN	2
#	define LCD_HSYNC_LEN	41
#	define LCD_RIGHT_MARGIN	2

#	define LCD_UPPER_MARGIN	2
#	define LCD_VSYNC_LEN	10
#	define LCD_LOWER_MARGIN	2

#	define LCD_DCU_DIV_RATIO	5

#	define LCD_SYN_POL_INV_PXCK	0
#	define LCD_SYN_POL_NEG		0
#	define LCD_SYN_POL_INV_VS		0
#	define LCD_SYN_POL_INV_HS		0

#	define LCD_BOOTARG_NAME		"lcd-vf6-bsb"

#elif defined(CONFIG_VIDEO_MVF_DCU)
#	error "MVF DCU is enabled but no LCD configured"
#endif

#define LCD_DCU_THRESHOLD_LS_BF_VS	0x3
#define LCD_DCU_THRESHOLD_OUT_BUF_HIGH	0x78
#define LCD_DCU_THRESHOLD_OUT_BUF_LOW	0

#ifdef LCD_BOOTARG_NAME
# define LCD_BOOTARG "vybrid_lcd=" LCD_BOOTARG_NAME
#else
# define LCD_BOOTARG ""
#endif

#endif /* __VYBRID_LCD_CONFIG_H */
