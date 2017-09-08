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

int board_early_init_f(void)
{
	lpuart_config_t lpuart_config;

	/* Init board hardware. */
	BOARD_InitPins();
	BOARD_BootClockRUN();

	LPUART_GetDefaultConfig(&lpuart_config);
	lpuart_config.baudRate_Bps = 115200;
	lpuart_config.enableTx = lpuart_config.enableRx = 1;
	/* Enable clock and initial UART module follow user configure structure. */
	CLOCK_SetMux(kCLOCK_UartMux,1);
	LPUART_Init(LPUART1, &lpuart_config, 6000000);

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

int arch_cpu_init(void)
{
	struct mpu_region_config imxrt1050_region_config[] = {
		{ 0x00000000, REGION_0, XN_DIS, PRIV_RW_USR_RW,
		  STRONG_ORDER, REGION_4GB },
		{ PHYS_SDRAM, REGION_1, XN_DIS, PRIV_RW_USR_RW,
		  O_I_WB_RD_WR_ALLOC, (ffs(PHYS_SDRAM_SIZE) - 2) },
		{ CONFIG_DMAMEM_BASE,
		  REGION_2, XN_DIS, PRIV_RW_USR_RW,
		  STRONG_ORDER, (ffs(CONFIG_DMAMEM_SZ_ALL) - 2) },
	};
	int i;

	/*
	 * Configure the memory protection unit (MPU) to allow full access to
	 * the whole 4GB address space.
	 */
	disable_mpu();
	for (i = 0; i < ARRAY_SIZE(imxrt1050_region_config); i++)
		mpu_config(&imxrt1050_region_config[i]);
	enable_mpu();

	return 0;
}
 

int dram_init(void)
{
	arch_cpu_init();
//	disable_mpu();
	SDRAM_Init(3, 3);
	gd->ram_top = PHYS_SDRAM;
	gd->ram_size = PHYS_SDRAM_SIZE;
	return 0;
}

int board_init(void)
{
	return 0;
}

int board_late_init(void)
{
	board_eth_init(NULL);
	return 0;
}

static int lpuart_serial_init(void)
{
	return 0;
}

static void lpuart_serial_putc(const char c)
{
	if (c == 0xa) {
		const unsigned char d = 0xd;
		LPUART_WriteBlocking(LPUART1, &d, 1);
	}
	LPUART_WriteBlocking(LPUART1, &c, 1);
}

static int lpuart_serial_getc(void)
{
	unsigned char c;
	LPUART_ReadBlocking(LPUART1, &c, 1);
	return c;
}

static void lpuart_serial_setbrg(void)
{
}

static int lpuart_serial_tstc(void)
{
	return 0;
}

static struct serial_device lpuart_serial_drv = {
	.name	= "lpuart_serial",
	.start	= lpuart_serial_init,
	.stop	= NULL,
	.setbrg	= lpuart_serial_setbrg,
	.putc	= lpuart_serial_putc,
	.puts	= default_serial_puts,
	.getc	= lpuart_serial_getc,
	.tstc	= lpuart_serial_tstc,
};

void lpuart_serial_initialize(void)
{
	serial_register(&lpuart_serial_drv);
}

__weak struct serial_device *default_serial_console(void)
{
	return &lpuart_serial_drv;
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

