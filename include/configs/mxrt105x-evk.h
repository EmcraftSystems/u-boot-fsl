/*
 * (C) Copyright 2017 Emcraft Systems
 * Dmitry Konyshev, <probables@emcraft.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#if 0
#define DEBUG
#endif

#if 0
#define CONFIG_SYS_ICACHE_OFF
#define CONFIG_SYS_DCACHE_OFF
#endif

#define CONFIG_SYS_ARCH_TIMER

#define CONFIG_SYS_TEXT_BASE		0x00001000

/*
 * To get Image data right at the 'Load Address' (0x80008000), and thus avoid
 * additional uImage relocation:
 * - 0x80007fc0 reserve place for uImage header; two separate image/dtb files
 * - 0x80007fb4 reserve place for 2-files multi-image header; one image+dtb file
 *
 * Note, that unaligned address can't be used when load from FAT as this results
 * to the slow copy path (and 'FAT: Misaligned buffer address') in fs/fat/fat.c.
 */
#define CONFIG_SYS_LOAD_ADDR		0x80007fc0
#define CONFIG_LOADADDR			0x80007fc0

#define PHYS_SDRAM			0x80000000
#define PHYS_SDRAM_SIZE			(32 * 1024 * 1024)

#define DMAMEM_SZ_ALL			(1 * 1024 * 1024)
#define DMAMEM_BASE			(PHYS_SDRAM + PHYS_SDRAM_SIZE - \
					 DMAMEM_SZ_ALL)

#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_MEMTEST_START	PHYS_SDRAM
#define CONFIG_SYS_MEMTEST_END      	(CONFIG_SYS_MEMTEST_START + PHYS_SDRAM_SIZE - (1024 * 1024))

#define CONFIG_SYS_INIT_SP_ADDR		(0x00000000 + 384 * 1024) /* points to end of OCRAM */

#define CONFIG_BOUNCE_BUFFER
#define CONFIG_FSL_ESDHC
#define CONFIG_FSL_USDHC
#define CONFIG_SUPPORT_EMMC_BOOT /* eMMC specific */

#define CONFIG_CMD_MMC

#define CONFIG_SYS_FSL_ESDHC_ADDR	0x402c0000
#define CONFIG_SYS_FSL_USDHC_NUM        1

#define CONFIG_CMD_FAT			1
#define CONFIG_SYS_FSL_ERRATUM_ESDHC135 1
#define ESDHCI_QUIRK_BROKEN_TIMEOUT_VALUE

/* UART */
#define LPUART_BASE			LPUART1_RBASE

/* Network */

#define CONFIG_FEC_MXC
#define CONFIG_MII

#define IMX_FEC_BASE			0x402D8000
#define CONFIG_FEC_MXC_PHYADDR          0x2

#define CONFIG_FEC_XCV_TYPE             RMII
#define CONFIG_ETHPRIME                 "FEC"
#define FEC_QUIRK_ENET_MAC

#define CONFIG_CMD_MII

/* LCD */
#ifdef CONFIG_VIDEO
#define CONFIG_FB_ADDR			DMAMEM_BASE
#define MXS_LCDIF_BASE			0x402B8000
#define CONFIG_VIDEO_MXS
#define CONFIG_SYS_CONSOLE_IS_IN_ENV
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_CMD_BMP
#define CONFIG_BMP_16BPP
#endif

/*
 * Configuration of the external SDRAM memory
 */
#define CONFIG_NR_DRAM_BANKS		1
#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM

#define CONFIG_SYS_MAX_FLASH_SECT	8
#define CONFIG_SYS_MAX_FLASH_BANKS	1

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

#define CONFIG_ENV_SIZE			(8 << 10)

#define CONFIG_SYS_CLK_FREQ		600000000 /* 600 MHz */

#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG
#define CONFIG_REVISION_TAG

#define CONFIG_SYS_CBSIZE		1024
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE \
					+ sizeof(CONFIG_SYS_PROMPT) + 16)

#define CONFIG_SYS_MAXARGS		16
#define CONFIG_SYS_MALLOC_LEN		(1 * 1024 * 1024)

#define CONFIG_SYS_MALLOC_F
#define CONFIG_SYS_MALLOC_F_LEN		(32 * 1024)

#define CONFIG_BOOTARGS							\
	"console=ttyLP0,115200 consoleblank=0 ignore_loglevel "

#define CONFIG_BOOTCOMMAND						\
	"run mmcboot"

#define CONFIG_PREBOOT \
	"fatload mmc 0 ${loadaddr} ${splash} && bmp display ${loadaddr};" \
	"fatexec mmc 0 ${ini}"

#define CONFIG_EXTRA_ENV_SETTINGS \
	"videomode=video=ctfb:x:480,y:272,depth:24,pclk:9300000,le:4,"	\
		"ri:8,up:4,lo:8,hs:41,vs:10,sync:0,vmode:0\0"		\
	"addip=setenv bootargs ${bootargs} ip=${ipaddr}:${serverip}:"	\
		"${gatewayip}:${netmask}:${hostname}:eth0:off\0"	\
	"ethaddr=aa:bb:cc:dd:ee:f0\0" \
	"serverip=172.17.0.1\0" \
	"ipaddr=172.17.44.111\0" \
	"netmask=255.255.0.0\0" \
	"ini=mxrt105x-evk.ini\0" \
	"image=rootfs.uImage\0" \
	"splash=splash-rt1050-series_24.bmp\0" \
	"uboot=u-boot-dtb.imx\0" \
	"tftpdir=imxrt105x/\0" \
	"gui=/crankdemo/gui.sh\0" \
	"ssh=yes\0" \
	"mmcboot=fatload mmc 0 ${loadaddr} ${image} && run addip &&"	\
		" bootm ${loadaddr}\0" \
	"netboot=tftp ${tftpdir}${image} && run addip; bootm ${loadaddr}\0" \
	"mmc_update_uboot=tftp ${tftpdir}${uboot} &&" \
		" setexpr tmp ${filesize} / 0x200 && setexpr tmp ${tmp} + 1 &&"\
		" mmc write ${loadaddr} 2 ${tmp}\0" \
	"mmc_update_kernel=tftp ${tftpdir}${image} &&" \
		" fatwrite mmc 0 ${loadaddr} ${image} ${filesize}\0" \
	"mmc_update_splash=tftp ${tftpdir}${splash} &&" \
		" fatwrite mmc 0 ${loadaddr} ${splash} ${filesize}\0"

/*
 * Command line configuration.
 */
#define CONFIG_SYS_LONGHELP
#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING
#define CONFIG_CMD_CACHE
#define CONFIG_BOARD_LATE_INIT

/*#define CONFIG_CMD_FUSE
  #define CONFIG_MXC_OCOTP*/

/* For SPL */
#ifdef CONFIG_SUPPORT_SPL
#define CONFIG_SPL_STACK		CONFIG_SYS_INIT_SP_ADDR
#define CONFIG_SPL_FRAMEWORK
#define CONFIG_SPL_BOARD_INIT
#define CONFIG_SPL_TEXT_BASE		CONFIG_SYS_FLASH_BASE
#define CONFIG_SYS_MONITOR_LEN		(512 * 1024)
#define CONFIG_SYS_SPL_LEN		0x00008000
#define CONFIG_SYS_UBOOT_START		0x080083FD
#define CONFIG_SYS_UBOOT_BASE		(CONFIG_SYS_FLASH_BASE + \
					 CONFIG_SYS_SPL_LEN)

/* DT blob (fdt) address */
#define CONFIG_SYS_FDT_BASE		(CONFIG_SYS_FLASH_BASE + \
					0x1C0000)
#endif
/* For SPL ends */

#undef CONFIG_CMD_IMLS

#endif /* __CONFIG_H */
