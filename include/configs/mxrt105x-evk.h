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
#define CONFIG_SYS_LOAD_ADDR		0x80007fc0
#define CONFIG_LOADADDR			0x80007fc0

#define PHYS_SDRAM			0x80000000
#define PHYS_SDRAM_SIZE			(32 * 1024 * 1024)

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

#define CONFIG_SYS_MMC_ENV_DEV          0	/* USDHC1 */
#define CONFIG_SYS_MMC_ENV_PART         0	/* user area */
#define CONFIG_MMCROOT                  "/dev/mmcblk0p2"  /* USDHC1 */
#define CONFIG_SYS_MMC_IMG_LOAD_PART    1
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

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

/*
 * Configuration of the external SDRAM memory
 */
#define CONFIG_NR_DRAM_BANKS		1
#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM

#define CONFIG_SYS_MAX_FLASH_SECT	8
#define CONFIG_SYS_MAX_FLASH_BANKS	1

#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_SIZE			(8 << 10)

#define CONFIG_SYS_CLK_FREQ		200000000 /* 200 MHz */
#define CONFIG_SYS_HZ_CLOCK		1000000	/* Timer is clocked at 1MHz */

#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG
#define CONFIG_REVISION_TAG

#define CONFIG_SYS_CBSIZE		1024
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE \
					+ sizeof(CONFIG_SYS_PROMPT) + 16)

#define CONFIG_SYS_MAXARGS		16
#define CONFIG_SYS_MALLOC_LEN		(32 * 1024)

#define C1ONFIG_BOOTARGS							\
	"console=ttyS0,115200 earlyprintk consoleblank=0 ignore_loglevel"
#define CONFIG_BOOTARGS							\
	"console=ttyLP0,115200 consoleblank=0 ignore_loglevel lpj=1000"// initcall_debug"
#define CONFIG_BOOTCOMMAND						\
	""

#define CONFIG_EXTRA_ENV_SETTINGS \
	"bootargs_romfs=uclinux.physaddr=0x08180000 root=/dev/mtdblock0\0" \
	"bootcmd_romfs=setenv bootargs ${bootargs} ${bootargs_romfs};" \
	"bootm 0x08044000 - 0x08042000\0" \
	"ethaddr=aa:bb:cc:dd:ee:f0\0" \
	"serverip=172.17.0.1\0" \
	"ipaddr=172.17.44.111\0" \
	"image=dk/imxrt1050/uImage\0" \
	"dtbimage=dk/imxrt1050/imxrt1050-evk.dtb\0" \
	"t=tftp ${image}; tftp 81000000 ${dtbimage}; bootm ${loadaddr} - 81000000\0"


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
