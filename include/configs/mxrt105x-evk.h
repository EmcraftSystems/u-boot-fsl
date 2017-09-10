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

# define CONFIG_DMAMEM_SZ_ALL		(1 * 1024 * 1024)
# define CONFIG_DMAMEM_BASE		(PHYS_SDRAM + \
					 (PHYS_SDRAM_SIZE) - \
					 CONFIG_DMAMEM_SZ_ALL)

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

#define CONFIG_BOOTARGS_SD							\
	"console=root=/dev/mmcblk0p2 rw rootwait"
#define CONFIG_BOOTARGS							\
	"console=ttyLP0,115200 consoleblank=0 ignore_loglevel " CONFIG_BOOTARGS_SD
#define CONFIG_BOOTCOMMAND						\
	""

#define CONFIG_EXTRA_ENV_SETTINGS \
	"addip=setenv bootargs ${bootargs} ip=${ipaddr}:${serverip}:"	\
	"${gatewayip}:${netmask}:${hostname}:${netdev}::off\0"		\
	"ethaddr=aa:bb:cc:dd:ee:f0\0" \
	"serverip=172.17.0.1\0" \
	"ipaddr=172.17.44.111\0" \
	"netmask=255.255.0.0\0" \
	"image=dk/imxrt1050/uImage\0" \
	"dtbimage=dk/imxrt1050/imxrt1050-evk.dtb\0" \
	"dtb_addr=81000000\0" \
	"mmcboot=fatload mmc 0 ${loadaddr} ${image};" \
	" fatload mmc 0 ${dtb_addr} ${dtbimage}; run addip; bootm ${loadaddr} - ${dtb_addr}\0" \
	"netboot=tftp ${image}; tftp ${dtb_addr} ${dtbimage}; run addip; bootm ${loadaddr} - ${dtb_addr}\0"


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
