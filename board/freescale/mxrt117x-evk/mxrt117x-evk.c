/*
 *  (C) Copyright 2017-2021 Emcraft Systems
 *  Dmitry Konyshev <probables@emcraft.com>
 *  Vladimir Skvortsov <vskvortsov@emcraft.com>
 *
 *  SPDX-License-Identifier:	GPL-2.0+
*/

#include <common.h>
#include <serial.h>
#include <phy.h>
#include <linux/compiler.h>
#include <asm/armv7m_mpu.h>
#include <fsl_esdhc.h>
#include <fsl_gpio.h>
#include "board/board.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_semc.h"
#include "board/pin_mux.h"

DECLARE_GLOBAL_DATA_PTR;

void BOARD_BootClockRUN(void);
void BOARD_InitPins(void);
void BOARD_InitPins_Enet(void);
void BOARD_InitPins_SDcard(void);
void SDRAM_Init(uint32_t bl,uint32_t cl);

#if defined(DEBUG)
/* turn on the user led on the IMXRT1174-EVK board*/
void board_early_dbg_led(void)
{
	gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
	CLOCK_EnableClock(kCLOCK_Iomuxc);           /* LPCG on: LPCG is ON. */

	IOMUXC_SetPinMux(
			 IOMUXC_GPIO_AD_04_GPIO9_IO03,           /* GPIO_AD_04 is configured as GPIO9_IO03 */
			 0U);                                    /* Software Input On Field: Input Path is determined by functionality */

	GPIO_PinInit(GPIO9, 3, &gpio_config);

	GPIO_WritePinOutput(GPIO9, 3, 1);
}
#endif

int board_early_init_f(void)
{
	/* Enable I cache and D cache */
	SCB_EnableDCache();
	SCB_EnableICache();

	BOARD_ConfigMPU();
	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitPins_SDcard();

	CLOCK_EnableClock(kCLOCK_Lpuart1);

	CLOCK_EnableClock(kCLOCK_Pit1);
	CLOCK_EnableClock(kCLOCK_Edma);

	return 0;
}

#ifdef CONFIG_FEC_MXC

#define MII_PHY_CTRL2	0x1f
#define MII_PHY_CTRL2_RMII_CLK_50MHZ	(1 << 7)
#define MII_PHY_CTRL2_RMII_LED_MODE	(1 << 4)

int board_phy_config(struct phy_device *phydev)
{
	phy_write(phydev, MDIO_DEVAD_NONE, MII_BMCR,
		  BMCR_RESET);

	phy_write(phydev, MDIO_DEVAD_NONE, MII_PHY_CTRL2,
		  phy_read(phydev, MDIO_DEVAD_NONE, MII_PHY_CTRL2) |
		  MII_PHY_CTRL2_RMII_CLK_50MHZ |
		  MII_PHY_CTRL2_RMII_LED_MODE);

	phy_write(phydev, MDIO_DEVAD_NONE, MII_ADVERTISE,
		  (ADVERTISE_100FULL | ADVERTISE_100HALF |
		   ADVERTISE_10FULL | ADVERTISE_10HALF | ADVERTISE_CSMA));

	phy_write(phydev, MDIO_DEVAD_NONE, MII_BMCR,
		  (BMCR_ANRESTART | BMCR_ANENABLE));

	return 0;
}

void IOMUXC_SelectENETClock(void)
{
	IOMUXC_GPR->GPR4 |= 0x3; /* 50M ENET_REF_CLOCK output to PHY and ENET module. */
}

int board_eth_init(bd_t *bis)
{
	const clock_sys_pll1_config_t sysPll1Config = {
			.pllDiv2En = true,
	};

	BOARD_InitPins_Enet();

	CLOCK_InitSysPll1(&sysPll1Config);
	clock_root_config_t rootCfg = {.mux = 4, .div = 10}; /* Generate 50M root clock. */
	CLOCK_SetRootClock(kCLOCK_Root_Enet1, &rootCfg);

	/* Select syspll2pfd3, 528*18/24 = 396M */
	CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd3, 24);
	rootCfg.mux = 7;
	rootCfg.div = 2;
	CLOCK_SetRootClock(kCLOCK_Root_Bus, &rootCfg); /* Generate 198M bus clock. */

	IOMUXC_SelectENETClock();

	gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

	GPIO_PinInit(GPIO9, 11, &gpio_config);
	GPIO_PinInit(GPIO12, 12, &gpio_config);
	/* Pull up the ENET_INT before RESET. */
	GPIO_WritePinOutput(GPIO9, 11, 1);
	GPIO_WritePinOutput(GPIO12, 12, 0);
	udelay(10000);
	GPIO_WritePinOutput(GPIO12, 12, 1);
	udelay(1000);

	return 0;
}

void imx_get_mac_from_fuse(int dev_id, unsigned char *mac)
{
	memset(mac, 0, 6);
}

u32 imx_get_fecclk(void)
{
	u32 ret = CLOCK_GetRootClockFreq(kCLOCK_Root_Bus);
	return ret;
}
#endif


int get_board_rev(void)
{
	return 0;
}

int print_cpuinfo(void)
{
	printf("CPU: i.MX RT117x at %iMHz\n",
		CLOCK_GetFreq(kCLOCK_CpuClk) / 1000 / 1000);
	return 0;
}

status_t BOARD_InitSEMC(void)
{
	semc_config_t config;
	semc_sdram_config_t sdramconfig;
	uint32_t clockFrq = CLOCK_GetRootClockFreq(kCLOCK_Root_Semc);

	/* Initializes the MAC configure structure to zero. */
	memset(&config, 0, sizeof(semc_config_t));
	memset(&sdramconfig, 0, sizeof(semc_sdram_config_t));

	/* Initialize SEMC. */
	SEMC_GetDefaultConfig(&config);
	config.dqsMode = kSEMC_Loopbackdqspad; /* For more accurate timing. */
	SEMC_Init(SEMC, &config);

	/* Configure SDRAM. */
	sdramconfig.csxPinMux           = kSEMC_MUXCSX0;
	sdramconfig.address             = 0x80000000;
	sdramconfig.memsize_kbytes      = 2 * 32 * 1024;       /* 64MB = 2*32*1024*1KBytes*/
	sdramconfig.portSize            = kSEMC_PortSize32Bit; /*two 16-bit SDRAMs make up 32-bit portsize*/
	sdramconfig.burstLen            = kSEMC_Sdram_BurstLen8;
	sdramconfig.columnAddrBitNum    = kSEMC_SdramColunm_9bit;
	sdramconfig.casLatency          = kSEMC_LatencyThree;
	sdramconfig.tPrecharge2Act_Ns   = 15; /* tRP 15ns */
	sdramconfig.tAct2ReadWrite_Ns   = 15; /* tRCD 15ns */
	sdramconfig.tRefreshRecovery_Ns = 70; /* Use the maximum of the (Trfc , Txsr). */
	sdramconfig.tWriteRecovery_Ns   = 2;  /* tWR 2ns */
	sdramconfig.tCkeOff_Ns =
		42; /* The minimum cycle of SDRAM CLK off state. CKE is off in self refresh at a minimum period tRAS.*/
	sdramconfig.tAct2Prechage_Ns       = 40; /* tRAS 40ns */
	sdramconfig.tSelfRefRecovery_Ns    = 70;
	sdramconfig.tRefresh2Refresh_Ns    = 60;
	sdramconfig.tAct2Act_Ns            = 2; /* tRC/tRDD 2ns */
	sdramconfig.tPrescalePeriod_Ns     = 160 * (1000000000 / clockFrq);
	sdramconfig.refreshPeriod_nsPerRow = 64 * 1000000 / 8192; /* 64ms/8192 */
	sdramconfig.refreshUrgThreshold    = sdramconfig.refreshPeriod_nsPerRow;
	sdramconfig.refreshBurstLen        = 1;
	sdramconfig.delayChain             = 2;

	return SEMC_ConfigureSDRAM(SEMC, kSEMC_SDRAM_CS0, &sdramconfig, clockFrq);
}

int dram_init(void)
{
	if (BOARD_InitSEMC() != 0) {
		return -1;
	}
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
	return 24000000;
}

#ifdef CONFIG_FSL_ESDHC
static struct fsl_esdhc_cfg usdhc_cfg[] = {
	{0x40418000, 0, 4},
};

int board_mmc_getcd(struct mmc *mmc)
{
	return 1;
}

int board_mmc_init(bd_t *bis)
{
	return fsl_esdhc_initialize(bis, &usdhc_cfg[0]);
}

uint32_t BOARD_USDHC1ClockConfiguration(void)
{
    clock_root_config_t rootCfg = {0};
    /* SYS PLL2 528MHz. */
    const clock_sys_pll2_config_t sysPll2Config = {
	.ssEnable = false,
    };

    CLOCK_InitSysPll2(&sysPll2Config);
    CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd2, 24);

    rootCfg.mux = 4;
    rootCfg.div = 2;
    CLOCK_SetRootClock(kCLOCK_Root_Usdhc1, &rootCfg);

    return CLOCK_GetRootClockFreq(kCLOCK_Root_Usdhc1);
}

void init_clk_usdhc(u32 index)
{
	BOARD_USDHC1ClockConfiguration();
	CLOCK_EnableClock(kCLOCK_Usdhc1);
}
#endif

#if defined(CONFIG_FIT_EMBED)
int board_fit_config_name_match(const char *name)
{
	if (!strcmp(name, "nxp,imxrt1060-evk"))
		return 0;

	return -1;
}
#endif
