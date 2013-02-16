/*
 * Copyright (C) 2011 Samsung Electronics
 * Heungjun Kim <riverful.kim@samsung.com>
 * Kyungmin Park <kyungmin.park@samsung.com>
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
#include <asm/arch/cpu.h>
#include <asm/arch/gpio.h>
#include <asm/arch/mmc.h>
#include <asm/arch/clock.h>
#include <asm/arch/watchdog.h>
#include <asm/arch/power.h>
#include <pmic.h>
#include <usb/s3c_udc.h>
#include <max8998_pmic.h>

#include "setup.h"

DECLARE_GLOBAL_DATA_PTR;

unsigned int board_rev;

#ifdef CONFIG_REVISION_TAG
u32 get_board_rev(void)
{
	return board_rev;
}
#endif

static void check_hw_revision(void);

int board_init(void)
{
	gd->bd->bi_boot_params = PHYS_SDRAM_1 + 0x100;

	check_hw_revision();
	printf("HW Revision:\t0x%x\n", board_rev);

#if defined(CONFIG_PMIC)
	pmic_init();
#endif

	return 0;
}

int dram_init(void)
{
	gd->ram_size = get_ram_size((long *)PHYS_SDRAM_1, PHYS_SDRAM_1_SIZE) +
		get_ram_size((long *)PHYS_SDRAM_2, PHYS_SDRAM_2_SIZE);

	return 0;
}

void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
	gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
	gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
}

static unsigned int get_hw_revision(void)
{
	struct exynos4_gpio_part1 *gpio =
		(struct exynos4_gpio_part1 *)samsung_get_base_gpio_part1();
	int hwrev = 0;
	int i;

	/* hw_rev[3:0] == GPE1[3:0] */
	for (i = 0; i < 4; i++) {
		s5p_gpio_cfg_pin(&gpio->e1, i, GPIO_INPUT);
		s5p_gpio_set_pull(&gpio->e1, i, GPIO_PULL_NONE);
	}

	udelay(1);

	for (i = 0; i < 4; i++)
		hwrev |= (s5p_gpio_get_value(&gpio->e1, i) << i);

	debug("hwrev 0x%x\n", hwrev);

	return hwrev;
}

static void check_hw_revision(void)
{
	int hwrev;

	hwrev = get_hw_revision();

	board_rev |= hwrev;
}

#ifdef CONFIG_DISPLAY_BOARDINFO
int checkboard(void)
{
	puts("Board:\tTRATS\n");
	return 0;
}
#endif

#ifdef CONFIG_GENERIC_MMC
int board_mmc_init(bd_t *bis)
{
	struct exynos4_gpio_part2 *gpio =
		(struct exynos4_gpio_part2 *)samsung_get_base_gpio_part2();
	int i, err;

	/* eMMC_EN: SD_0_CDn: GPK0[2] Output High */
	s5p_gpio_direction_output(&gpio->k0, 2, 1);
	s5p_gpio_set_pull(&gpio->k0, 2, GPIO_PULL_NONE);

	/*
	 * eMMC GPIO:
	 * SDR 8-bit@48MHz at MMC0
	 * GPK0[0]	SD_0_CLK(2)
	 * GPK0[1]	SD_0_CMD(2)
	 * GPK0[2]	SD_0_CDn	-> Not used
	 * GPK0[3:6]	SD_0_DATA[0:3](2)
	 * GPK1[3:6]	SD_0_DATA[0:3](3)
	 *
	 * DDR 4-bit@26MHz at MMC4
	 * GPK0[0]	SD_4_CLK(3)
	 * GPK0[1]	SD_4_CMD(3)
	 * GPK0[2]	SD_4_CDn	-> Not used
	 * GPK0[3:6]	SD_4_DATA[0:3](3)
	 * GPK1[3:6]	SD_4_DATA[4:7](4)
	 */
	for (i = 0; i < 7; i++) {
		if (i == 2)
			continue;
		/* GPK0[0:6] special function 2 */
		s5p_gpio_cfg_pin(&gpio->k0, i, 0x2);
		/* GPK0[0:6] pull disable */
		s5p_gpio_set_pull(&gpio->k0, i, GPIO_PULL_NONE);
		/* GPK0[0:6] drv 4x */
		s5p_gpio_set_drv(&gpio->k0, i, GPIO_DRV_4X);
	}

	for (i = 3; i < 7; i++) {
		/* GPK1[3:6] special function 3 */
		s5p_gpio_cfg_pin(&gpio->k1, i, 0x3);
		/* GPK1[3:6] pull disable */
		s5p_gpio_set_pull(&gpio->k1, i, GPIO_PULL_NONE);
		/* GPK1[3:6] drv 4x */
		s5p_gpio_set_drv(&gpio->k1, i, GPIO_DRV_4X);
	}

	/*
	 * MMC device init
	 * mmc0	 : eMMC (8-bit buswidth)
	 * mmc2	 : SD card (4-bit buswidth)
	 */
	err = s5p_mmc_init(0, 8);

	/* T-flash detect */
	s5p_gpio_cfg_pin(&gpio->x3, 4, 0xf);
	s5p_gpio_set_pull(&gpio->x3, 4, GPIO_PULL_UP);

	/*
	 * Check the T-flash  detect pin
	 * GPX3[4] T-flash detect pin
	 */
	if (!s5p_gpio_get_value(&gpio->x3, 4)) {
		/*
		 * SD card GPIO:
		 * GPK2[0]	SD_2_CLK(2)
		 * GPK2[1]	SD_2_CMD(2)
		 * GPK2[2]	SD_2_CDn	-> Not used
		 * GPK2[3:6]	SD_2_DATA[0:3](2)
		 */
		for (i = 0; i < 7; i++) {
			if (i == 2)
				continue;
			/* GPK2[0:6] special function 2 */
			s5p_gpio_cfg_pin(&gpio->k2, i, 0x2);
			/* GPK2[0:6] pull disable */
			s5p_gpio_set_pull(&gpio->k2, i, GPIO_PULL_NONE);
			/* GPK2[0:6] drv 4x */
			s5p_gpio_set_drv(&gpio->k2, i, GPIO_DRV_4X);
		}
		err = s5p_mmc_init(2, 4);
	}

	return err;
}
#endif

#ifdef CONFIG_USB_GADGET
static int s5pc210_phy_control(int on)
{
	int ret = 0;
	struct pmic *p = get_pmic();

	if (pmic_probe(p))
		return -1;

	if (on) {
		ret |= pmic_set_output(p,
				       MAX8998_REG_BUCK_ACTIVE_DISCHARGE3,
				       MAX8998_SAFEOUT1, LDO_ON);
		ret |= pmic_set_output(p, MAX8998_REG_ONOFF1,
				      MAX8998_LDO3, LDO_ON);
		ret |= pmic_set_output(p, MAX8998_REG_ONOFF2,
				      MAX8998_LDO8, LDO_ON);

	} else {
		ret |= pmic_set_output(p, MAX8998_REG_ONOFF2,
				      MAX8998_LDO8, LDO_OFF);
		ret |= pmic_set_output(p, MAX8998_REG_ONOFF1,
				      MAX8998_LDO3, LDO_OFF);
		ret |= pmic_set_output(p,
				       MAX8998_REG_BUCK_ACTIVE_DISCHARGE3,
				       MAX8998_SAFEOUT1, LDO_OFF);
	}

	if (ret) {
		puts("MAX8998 LDO setting error!\n");
		return -1;
	}

	return 0;
}

struct s3c_plat_otg_data s5pc210_otg_data = {
	.phy_control	= s5pc210_phy_control,
	.regs_phy	= EXYNOS4_USBPHY_BASE,
	.regs_otg	= EXYNOS4_USBOTG_BASE,
	.usb_phy_ctrl	= EXYNOS4_USBPHY_CONTROL,
	.usb_flags	= PHY0_SLEEP,
};
#endif

static void pmic_reset(void)
{
	struct exynos4_gpio_part2 *gpio =
		(struct exynos4_gpio_part2 *)samsung_get_base_gpio_part2();

	s5p_gpio_direction_output(&gpio->x0, 7, 1);
	s5p_gpio_set_pull(&gpio->x2, 7, GPIO_PULL_NONE);
}

static void board_clock_init(void)
{
	struct exynos4_clock *clk =
		(struct exynos4_clock *)samsung_get_base_clock();

	writel(CLK_SRC_CPU_VAL, (unsigned int)&clk->src_cpu);
	writel(CLK_SRC_TOP0_VAL, (unsigned int)&clk->src_top0);
	writel(CLK_SRC_FSYS_VAL, (unsigned int)&clk->src_fsys);
	writel(CLK_SRC_PERIL0_VAL, (unsigned int)&clk->src_peril0);

	writel(CLK_DIV_CPU0_VAL, (unsigned int)&clk->div_cpu0);
	writel(CLK_DIV_CPU1_VAL, (unsigned int)&clk->div_cpu1);
	writel(CLK_DIV_DMC0_VAL, (unsigned int)&clk->div_dmc0);
	writel(CLK_DIV_DMC1_VAL, (unsigned int)&clk->div_dmc1);
	writel(CLK_DIV_LEFTBUS_VAL, (unsigned int)&clk->div_leftbus);
	writel(CLK_DIV_RIGHTBUS_VAL, (unsigned int)&clk->div_rightbus);
	writel(CLK_DIV_TOP_VAL, (unsigned int)&clk->div_top);
	writel(CLK_DIV_FSYS1_VAL, (unsigned int)&clk->div_fsys1);
	writel(CLK_DIV_FSYS2_VAL, (unsigned int)&clk->div_fsys2);
	writel(CLK_DIV_FSYS3_VAL, (unsigned int)&clk->div_fsys3);
	writel(CLK_DIV_PERIL0_VAL, (unsigned int)&clk->div_peril0);
	writel(CLK_DIV_PERIL3_VAL, (unsigned int)&clk->div_peril3);

	writel(PLL_LOCKTIME, (unsigned int)&clk->apll_lock);
	writel(PLL_LOCKTIME, (unsigned int)&clk->mpll_lock);
	writel(PLL_LOCKTIME, (unsigned int)&clk->epll_lock);
	writel(PLL_LOCKTIME, (unsigned int)&clk->vpll_lock);
	writel(APLL_CON1_VAL, (unsigned int)&clk->apll_con1);
	writel(APLL_CON0_VAL, (unsigned int)&clk->apll_con0);
	writel(MPLL_CON1_VAL, (unsigned int)&clk->mpll_con1);
	writel(MPLL_CON0_VAL, (unsigned int)&clk->mpll_con0);
	writel(EPLL_CON1_VAL, (unsigned int)&clk->epll_con1);
	writel(EPLL_CON0_VAL, (unsigned int)&clk->epll_con0);
	writel(VPLL_CON1_VAL, (unsigned int)&clk->vpll_con1);
	writel(VPLL_CON0_VAL, (unsigned int)&clk->vpll_con0);

	writel(CLK_GATE_IP_CAM_VAL, (unsigned int)&clk->gate_ip_cam);
	writel(CLK_GATE_IP_VP_VAL, (unsigned int)&clk->gate_ip_tv);
	writel(CLK_GATE_IP_MFC_VAL, (unsigned int)&clk->gate_ip_mfc);
	writel(CLK_GATE_IP_G3D_VAL, (unsigned int)&clk->gate_ip_g3d);
	writel(CLK_GATE_IP_IMAGE_VAL, (unsigned int)&clk->gate_ip_image);
	writel(CLK_GATE_IP_LCD0_VAL, (unsigned int)&clk->gate_ip_lcd0);
	writel(CLK_GATE_IP_LCD1_VAL, (unsigned int)&clk->gate_ip_lcd1);
	writel(CLK_GATE_IP_FSYS_VAL, (unsigned int)&clk->gate_ip_fsys);
	writel(CLK_GATE_IP_GPS_VAL, (unsigned int)&clk->gate_ip_gps);
	writel(CLK_GATE_IP_PERIL_VAL, (unsigned int)&clk->gate_ip_peril);
	writel(CLK_GATE_IP_PERIR_VAL, (unsigned int)&clk->gate_ip_perir);
	writel(CLK_GATE_BLOCK_VAL, (unsigned int)&clk->gate_block);
}

static void board_power_init(void)
{
	struct exynos4_power *pwr =
		(struct exynos4_power *)samsung_get_base_power();

	/* PS HOLD */
	writel(EXYNOS4_PS_HOLD_CON_VAL, (unsigned int)&pwr->ps_hold_control);

	/* Set power down */
	writel(0, (unsigned int)&pwr->cam_configuration);
	writel(0, (unsigned int)&pwr->tv_configuration);
	writel(0, (unsigned int)&pwr->mfc_configuration);
	writel(0, (unsigned int)&pwr->g3d_configuration);
	writel(0, (unsigned int)&pwr->lcd1_configuration);
	writel(0, (unsigned int)&pwr->gps_configuration);
	writel(0, (unsigned int)&pwr->gps_alive_configuration);
}

static void board_uart_init(void)
{
	struct exynos4_gpio_part1 *gpio1 =
		(struct exynos4_gpio_part1 *)samsung_get_base_gpio_part1();
	struct exynos4_gpio_part2 *gpio2 =
		(struct exynos4_gpio_part2 *)samsung_get_base_gpio_part2();
	int i;

	/*
	 * UART2 GPIOs
	 * GPA1CON[0] = UART_2_RXD(2)
	 * GPA1CON[1] = UART_2_TXD(2)
	 * GPA1CON[2] = I2C_3_SDA (3)
	 * GPA1CON[3] = I2C_3_SCL (3)
	 */

	for (i = 0; i < 4; i++) {
		s5p_gpio_set_pull(&gpio1->a1, i, GPIO_PULL_NONE);
		s5p_gpio_cfg_pin(&gpio1->a1, i, GPIO_FUNC((i > 1) ? 0x3 : 0x2));
	}

	/* UART_SEL GPY4[7] (part2) at EXYNOS4 */
	s5p_gpio_set_pull(&gpio2->y4, 7, GPIO_PULL_UP);
	s5p_gpio_direction_output(&gpio2->y4, 7, 1);
}

int board_early_init_f(void)
{
	wdt_stop();
	pmic_reset();
	board_clock_init();
	board_uart_init();
	board_power_init();

	return 0;
}
