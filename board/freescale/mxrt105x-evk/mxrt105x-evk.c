/*
 *  (C) Copyright 2017 Emcraft Systems
 *  Dmitry Konyshev <probables@emcraft.com>
 *
 *  SPDX-License-Identifier:	GPL-2.0+
*/

#include <common.h>
#include <serial.h>
#include <phy.h>
#include <linux/compiler.h>
#include "drivers/fsl_lpuart.h"
#include <asm/armv7m_mpu.h>
#include <fsl_esdhc.h>
#include <fsl_gpio.h>
#include <fsl_enet.h>

DECLARE_GLOBAL_DATA_PTR;

void BOARD_BootClockRUN(void);
void BOARD_InitPins(void);
void BOARD_Ethernet_InitPins(void);
void SDRAM_Init(uint32_t bl,uint32_t cl);
status_t PHY_Init(ENET_Type *base, uint32_t phyAddr, uint32_t srcClock_Hz);

static void mxrt105x_evk_usb_init(void)
{
	/* Enable USB1/2 PLLs and USB clock gate */
	*(volatile long *)0x400d8010 |= 0x3040;
	*(volatile long *)0x400d8020 |= 0x3040;
	*(volatile long *)0x400fc080 |= 3;
	/* Configure IOMUX */
	*(volatile long *)0x401f80c0 = 3;
	*(volatile long *)0x401f80c4 = 3;
	*(volatile long *)0x401f80c8 = 3;
}

int board_early_init_f(void)
{
	/* Init board hardware. */
	BOARD_InitPins();
	BOARD_BootClockRUN();

	CLOCK_SetMux(kCLOCK_UartMux,1);
	CLOCK_EnableClock(kCLOCK_Lpuart1);

	mxrt105x_evk_usb_init();

	return 0;
}

#ifdef CONFIG_FEC_MXC

#define MII_PHY_CTRL2	0x1f
#define MII_PHY_CTRL2_RMII_CLK_50MHZ	(1 << 7)
#define MII_PHY_CTRL2_RMII_LED_MODE	(1 << 4)

int board_phy_config(struct phy_device *phydev)
{
	phy_write(phydev, MDIO_DEVAD_NONE, MII_PHY_CTRL2,
		phy_read(phydev, MDIO_DEVAD_NONE, MII_PHY_CTRL2) |
			MII_PHY_CTRL2_RMII_CLK_50MHZ |
			MII_PHY_CTRL2_RMII_LED_MODE);
	phy_write(phydev, MDIO_DEVAD_NONE, MII_BMCR,
		phy_read(phydev, MDIO_DEVAD_NONE, MII_BMCR) & ~BMCR_ISOLATE);
}

int board_eth_init(bd_t *bis)
{
	const clock_enet_pll_config_t pll_config =
	{
		true,
		false,
		false,
		1,
		1
	};

	BOARD_Ethernet_InitPins();

	CLOCK_InitEnetPll(&pll_config);

	gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
	GPIO_PinInit(GPIO1, 9, &gpio_config);
	GPIO_PinInit(GPIO1, 10, &gpio_config);
	// pull up the ENET_INT before RESET
	GPIO_WritePinOutput(GPIO1, 10, 1);
	GPIO_WritePinOutput(GPIO1, 9, 0);
	udelay(100);
	GPIO_WritePinOutput(GPIO1, 9, 1);

	/* Set SMI to get PHY link status. */
	CLOCK_EnableClock(kCLOCK_Enet);
	ENET_SetSMI(ENET, CLOCK_GetFreq(kCLOCK_IpgClk), false);

	return 0;
}

void imx_get_mac_from_fuse(int dev_id, unsigned char *mac)
{
	memset(mac, 0, 6);
}

u32 imx_get_fecclk(void)
{
	return CLOCK_GetFreq(kCLOCK_IpgClk);
}
#endif

#ifdef CONFIG_VIDEO_MXS
static void setup_lcd(void)
{
	struct {
		u32	mux_reg;
		u32	conf_reg;
		u32	mux_mode;
		u32	conf_val;
	} pin[] = {
		{ 0x013c, 0x032c, 0x0, 0x1b0b0 }, /* PAD_B0_00_LCD_CLK */
		{ 0x0140, 0x0330, 0x0, 0x1b0b0 }, /* PAD_B0_01_LCD_ENABLE */
		{ 0x0144, 0x0334, 0x0, 0x1b0b0 }, /* PAD_B0_02_LCD_HSYNC */
		{ 0x0148, 0x0338, 0x0, 0x1b0b0 }, /* PAD_B0_03_LCD_VSYNC */
		{ 0x014c, 0x033c, 0x0, 0x1b0b0 }, /* PAD_B0_04_LCD_DATA00 */
		{ 0x0150, 0x0340, 0x0, 0x1b0b0 }, /* PAD_B0_05_LCD_DATA01 */
		{ 0x0154, 0x0344, 0x0, 0x1b0b0 }, /* PAD_B0_06_LCD_DATA02 */
		{ 0x0158, 0x0348, 0x0, 0x1b0b0 }, /* PAD_B0_07_LCD_DATA03 */
		{ 0x015c, 0x034c, 0x0, 0x1b0b0 }, /* PAD_B0_08_LCD_DATA04 */
		{ 0x0160, 0x0350, 0x0, 0x1b0b0 }, /* PAD_B0_09_LCD_DATA05 */
		{ 0x0164, 0x0354, 0x0, 0x1b0b0 }, /* PAD_B0_10_LCD_DATA06 */
		{ 0x0168, 0x0358, 0x0, 0x1b0b0 }, /* PAD_B0_11_LCD_DATA07 */
		{ 0x016c, 0x035c, 0x0, 0x1b0b0 }, /* PAD_B0_12_LCD_DATA08 */
		{ 0x0170, 0x0360, 0x0, 0x1b0b0 }, /* PAD_B0_13_LCD_DATA09 */
		{ 0x0174, 0x0364, 0x0, 0x1b0b0 }, /* PAD_B0_14_LCD_DATA10 */
		{ 0x0178, 0x0368, 0x0, 0x1b0b0 }, /* PAD_B0_15_LCD_DATA11 */
		{ 0x017c, 0x036c, 0x0, 0x1b0b0 }, /* PAD_B1_00_LCD_DATA12 */
		{ 0x0180, 0x0370, 0x0, 0x1b0b0 }, /* PAD_B1_01_LCD_DATA13 */
		{ 0x0184, 0x0374, 0x0, 0x1b0b0 }, /* PAD_B1_02_LCD_DATA14 */
		{ 0x0188, 0x0378, 0x0, 0x1b0b0 }, /* PAD_B1_03_LCD_DATA15 */

		{ 0x01b8, 0x03a8, 0x5, 0x0b069 }, /* PAD_B1_15_GPIO2_IO31 */
		{ 0x00c4, 0x02b4, 0x5, 0x0b069 }, /* PAD_AD_B0_02_GPIO1_IO02 */
	};

	int i;

	/* Configure IOMUX */
	for (i = 0; i < ARRAY_SIZE(pin); i++) {
		writel(pin[i].mux_mode, IOMUXC_BASE + pin[i].mux_reg);
		writel(pin[i].conf_val, IOMUXC_BASE + pin[i].conf_reg);
	}
}
#endif /* CONFIG_VIDEO_MXS */

int get_board_rev(void)
{
	return 2;
}

int print_cpuinfo(void)
{
	printf("CPU: i.MX RT105x at %iMHz\n",
		CLOCK_GetFreq(kCLOCK_CpuClk) / 1000 / 1000);
	return 0;
}

int dram_init(void)
{
	arch_cpu_init();
	SDRAM_Init(3, 3);
	gd->ram_top = PHYS_SDRAM;
	gd->ram_size = PHYS_SDRAM_SIZE;
	return 0;
}

int board_init(void)
{
#ifdef CONFIG_VIDEO_MXS
	setup_lcd();
#endif

	return 0;
}

int board_late_init(void)
{
	board_eth_init(NULL);
	return 0;
}

u32 get_lpuart_clk(void)
{
	return 6000000;
}

#ifdef CONFIG_FSL_ESDHC
static struct fsl_esdhc_cfg usdhc_cfg[] = {
	{0x402c0000, 0, 4},
};

int board_mmc_getcd(struct mmc *mmc)
{
	return 1;
}

int board_mmc_init(bd_t *bis)
{
	return fsl_esdhc_initialize(bis, &usdhc_cfg[0]);
}

void init_clk_usdhc(u32 index)
{
	*(volatile unsigned long*)0x400FC080 |= (3 << 2);
	*(volatile unsigned long*)0x400FC024 &= ~(7 << 11);
	*(volatile unsigned long*)0x400FC024 |= (7 << 11);
}
#endif

#if defined(CONFIG_FIT_EMBED)
int board_fit_config_name_match(const char *name)
{
	if (!strcmp(name, "nxp,imxrt1050-evk"))
		return 0;

	return -1;
}
#endif

