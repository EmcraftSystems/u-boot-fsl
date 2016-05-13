/*
 * (C) Copyright 2011, 2012, 2013
 *
 * Pavel Boldin, Emcraft Systems, paboldin@emcraft.com
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
#include <lcd.h>
#include <asm/io.h>
#include <mvf_dcu.h>

vidinfo_t panel_info = {
	vl_col:		LCD_XRES,
	vl_row:		LCD_YRES,
	vl_bpix:	LCD_BIT_PER_PIXEL
};

/*#define CONFIG_VIDEO_MVF_DCU_COLBAR*/	/* Activate to test board-to-lcd connectivity
				   via colorbar generation */

void *lcd_base;			/* Start of framebuffer memory */
void *lcd_console_address;	/* Start of console buffer     */

int lcd_line_length;
int lcd_color_fg;
int lcd_color_bg;

short console_col;
short console_row;

static inline u32 dcu_writel(u32 val, u32 offset)
{
	return writel(val, DCU_BASE_ADDR + offset);
}

static inline u32 dcu_readl(u32 offset)
{
	return readl(DCU_BASE_ADDR + offset);
}

static inline void dcu_update(void)
{
	dcu_writel(DCU_UPDATE_MODE_READREG(1), DCU_UPDATE_MODE);
}

void __lcd_enable(void)
{
	/* backlight on */
	writel(1 << CONFIG_DCU_BACKLIGHT_GPIO_NUM,
		CONFIG_DCU_BACKLIGHT_GPIO_ADDR_SET);
}
void lcd_enable(void) __attribute__((weak, alias("__lcd_enable")));

void __lcd_disable(void)
{
	/* backlight off */
	writel(1 << CONFIG_DCU_BACKLIGHT_GPIO_NUM,
		CONFIG_DCU_BACKLIGHT_GPIO_ADDR_CLEAR);
}
void lcd_disable(void) __attribute__((weak, alias("__lcd_disable")));

ulong calc_fbsize(void)
{
#ifdef CONFIG_FB_ADDR
# ifdef CONFIG_SYS_L2CACHE_OFF
	return 0xFFFFF;
# else
	return 0x7FFFF;
# endif
#else
	return 2 * ((panel_info.vl_col * panel_info.vl_row *
		NBITS(panel_info.vl_bpix)) / 8);
#endif
}

int overwrite_console(void)
{
	/* Keep stdout / stderr on serial, our LCD is for splashscreen only */
	return 1;
}

static void dcu_enable(void)
{
	u32 dcu_mode;

	dcu_mode = dcu_readl(DCU_DCU_MODE);
#ifdef CONFIG_VIDEO_MVF_DCU_COLBAR
	/* Temporary colbar mode */
	dcu_writel(dcu_mode | DCU_MODE_DCU_MODE(3), DCU_DCU_MODE);
#else
	dcu_writel(dcu_mode | DCU_MODE_DCU_MODE(1), DCU_DCU_MODE);
#endif
}


void lcd_ctrl_init(void *lcdbase)
{
	u32 flags;
	u32 dcu_bpix = 0;
	int i;

	if (!lcdbase)
		return;

	/* Disable TCON, set pass-by */
	writel(0x20000000, 0x4003D000);

	/* Configure timing */
#define CCM_CSCDR3 (CCM_BASE_ADDR + 0x1C)
#define DCU0_DIV4_ENABLE (0x000B0000)
	flags = readl(CCM_CSCDR3);
	writel(flags | DCU0_DIV4_ENABLE, CCM_CSCDR3);

	/* Clean status, disable all layers */
	dcu_writel(0, DCU_INT_STATUS);
	dcu_writel(0, DCU_PARR_ERR_STA_1);
	dcu_writel(0, DCU_PARR_ERR_STA_2);
	dcu_writel(0, DCU_PARR_ERR_STA_3);

	for (i = 0; i < 64; i++) {
		dcu_writel(0, DCU_CTRLDESCLN_3(i));
	}


	dcu_writel(DCU_BGND_R(0) | DCU_BGND_G(0) | DCU_BGND_B(0), DCU_BGND);


	dcu_writel(DCU_DISP_SIZE_DELTA_Y(panel_info.vl_row) |
			DCU_DISP_SIZE_DELTA_X(panel_info.vl_col / 16),
			DCU_DISP_SIZE);


	/* Init LCD screen timing parameters */
	dcu_writel(DCU_HSYN_PARA_BP(LCD_LEFT_MARGIN) |
			DCU_HSYN_PARA_PW(LCD_HSYNC_LEN) |
			DCU_HSYN_PARA_FP(LCD_RIGHT_MARGIN),
			DCU_HSYN_PARA);

	dcu_writel(DCU_VSYN_PARA_BP(LCD_UPPER_MARGIN) |
			DCU_VSYN_PARA_PW(LCD_VSYNC_LEN) |
			DCU_VSYN_PARA_FP(LCD_LOWER_MARGIN),
			DCU_VSYN_PARA);

	dcu_writel(DCU_MODE_BLEND_ITER(3) | DCU_MODE_RASTER_EN(1),
			DCU_DCU_MODE);

	/* Timing divider for pixel_clock */
	dcu_writel(LCD_DCU_DIV_RATIO,  DCU_DIV_RATIO);

	dcu_writel(DCU_SYN_POL_INV_PXCK(LCD_SYN_POL_INV_PXCK) |
			DCU_SYN_POL_NEG(LCD_SYN_POL_NEG) |
			DCU_SYN_POL_INV_VS(LCD_SYN_POL_INV_VS) |
			DCU_SYN_POL_INV_HS(LCD_SYN_POL_INV_HS),
			DCU_SYN_POL);
	dcu_writel(DCU_THRESHOLD_LS_BF_VS(LCD_DCU_THRESHOLD_LS_BF_VS) |
		DCU_THRESHOLD_OUT_BUF_HIGH(LCD_DCU_THRESHOLD_OUT_BUF_HIGH) |
		DCU_THRESHOLD_OUT_BUF_LOW(LCD_DCU_THRESHOLD_OUT_BUF_LOW),
		DCU_THRESHOLD);

#ifdef CONFIG_VIDEO_MVF_DCU_COLBAR
	dcu_writel(0x00FF00, DCU_COLBAR_1);
	dcu_writel(0x0000FF, DCU_COLBAR_2);
	dcu_writel(0xFF00FF, DCU_COLBAR_3);
#else
	/* Enable single layer, use it for displaying image */
	i = 0;
	dcu_writel(DCU_CTRLDESCLN_0_HEIGHT(panel_info.vl_row) |
			DCU_CTRLDESCLN_0_WIDTH(panel_info.vl_col),
			DCU_CTRLDESCLN_0(i));

	dcu_writel(DCU_CTRLDESCLN_1_POSY(0) | DCU_CTRLDESCLN_1_POSX(0),
			DCU_CTRLDESCLN_1(i));

	dcu_writel((u32)lcdbase, DCU_CTRLDESCLN_2(i));

	switch (NBITS(panel_info.vl_bpix)) {
		case 24:
			dcu_bpix = 5; /* 0b0101 */
			break;
		case 32:
			dcu_bpix = 6; /* 0b0110 */
			break;
		default:
			puts("ERROR: Unknown BPP for LCD\n");
			return;
	}

	dcu_writel(DCU_CTRLDESCLN_3_EN(1) |
			DCU_CTRLDESCLN_3_BPP(dcu_bpix),
			DCU_CTRLDESCLN_3(i));


	dcu_writel(DCU_CTRLDESCLN_4_CKMAX_R(255) |
			DCU_CTRLDESCLN_4_CKMAX_G(255) |
			DCU_CTRLDESCLN_4_CKMAX_B(255),
			DCU_CTRLDESCLN_4(i));
#endif /* CONFIG_VIDEO_MVF_DCU_COLBAR */

	dcu_enable();

	dcu_update();
}

void lcd_setcolreg (ushort regno, ushort red, ushort green, ushort blue)
{
}
