/*
 * Copyright (C) 2016 Emcraft Systems
 *
 * Configuration settings for the Emcraft solox-som board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */


#ifndef __CONFIG_H
#define __CONFIG_H

#include <asm/arch/imx-regs.h>
#include <linux/sizes.h>
#include "mx6_common.h"
#include <asm/imx-common/gpio.h>

#define CONFIG_MX6
#define CONFIG_ROM_UNIFIED_SECTIONS
#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO
#undef CONFIG_LDO_BYPASS_CHECK

#define CONFIG_DBG_MONITOR
/* uncomment for PLUGIN mode support */
/* #define CONFIG_USE_PLUGIN */

/* uncomment for SECURE mode support */
/* #define CONFIG_SECURE_BOOT */

#ifdef CONFIG_SECURE_BOOT
#ifndef CONFIG_CSF_SIZE
#define CONFIG_CSF_SIZE 0x4000
#endif
#endif

#ifdef CONFIG_SPL
#define CONFIG_SPL_LIBCOMMON_SUPPORT
#define CONFIG_SPL_MMC_SUPPORT
#include "imx6_spl.h"
#endif

#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG
#define CONFIG_REVISION_TAG
#define CONFIG_SYS_GENERIC_BOARD

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(32 * SZ_1M)

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_LATE_INIT
#define CONFIG_MXC_GPIO

#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE		UART1_BASE

/* MMC Configuration */
#if 1
#define CONFIG_FSL_ESDHC
#define CONFIG_FSL_USDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR	USDHC4_BASE_ADDR
#define CONFIG_SYS_FSL_USDHC_NUM	3

#define CONFIG_MMC
#define CONFIG_CMD_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_BOUNCE_BUFFER
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_FAT
#define CONFIG_DOS_PARTITION
#define CONFIG_SUPPORT_EMMC_BOOT /* eMMC specific */
#endif

#define CONFIG_BAUDRATE			115200

#undef CONFIG_BOOTM_NETBSD
#undef CONFIG_BOOTM_PLAN9
#undef CONFIG_BOOTM_RTEMS

#undef CONFIG_CMD_EXPORTENV
#undef CONFIG_CMD_IMPORTENV

/* Network */
#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_MII
#define CONFIG_CMD_NET
#define CONFIG_FEC_MXC
#define CONFIG_MII
#define CONFIG_FEC_ENET_DEV 0

#if (CONFIG_FEC_ENET_DEV == 0)
#define IMX_FEC_BASE			ENET_BASE_ADDR
#define CONFIG_FEC_MXC_PHYADDR          0x4
#define CONFIG_FEC_XCV_TYPE             RGMII
#elif (CONFIG_FEC_ENET_DEV == 1)
#define IMX_FEC_BASE			ENET2_BASE_ADDR
#define CONFIG_FEC_MXC_PHYADDR          0x1
#define CONFIG_FEC_XCV_TYPE             RMII
#endif

#define CONFIG_ETHPRIME                 "FEC"

#define CONFIG_PHYLIB
#define CONFIG_PHY_ATHEROS
#define CONFIG_FEC_DMA_MINALIGN		64
#define CONFIG_FEC_MXC_25M_REF_CLK

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_CONS_INDEX		1

/* I2C Configs */
#define CONFIG_CMD_I2C
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_SPEED		100000

/* VIDEO */
#define CONFIG_VIDEO
#define CONFIG_VIDEO_GIS

/* Command definition */
#include <config_cmd_default.h>

#undef CONFIG_CMD_IMLS

#define CONFIG_BOOTDELAY		1

#define CONFIG_LOADADDR			0x807fffc0
#define CONFIG_SYS_TEXT_BASE		0x87800000

#define CONFIG_SYS_AUXCORE_BOOTDATA 0x78000000 /* Set to QSPI2 B flash at default */
#ifndef CONFIG_SYS_AUXCORE_FASTUP
#define CONFIG_CMD_BOOTAUX /* Boot M4 by command, disable this when M4 fast up */
#define CONFIG_CMD_SETEXPR
#endif

#ifdef CONFIG_CMD_BOOTAUX
#define UPDATE_M4_ENV \
	"m4image=m4_qspi.bin\0" \
	"loadm4image=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${m4image}\0" \
	"update_m4_from_sd=" \
		"if sf probe 1:0; then " \
			"if run loadm4image; then " \
				"setexpr fw_sz ${filesize} + 0xffff; " \
				"setexpr fw_sz ${fw_sz} / 0x10000; "	\
				"setexpr fw_sz ${fw_sz} * 0x10000; "	\
				"sf erase 0x0 ${fw_sz}; " \
				"sf write ${loadaddr} 0x0 ${filesize}; " \
			"fi; " \
		"fi\0" \
	"m4boot=sf probe 1:0; bootaux "__stringify(CONFIG_SYS_AUXCORE_BOOTDATA)"\0"
#else
#define UPDATE_M4_ENV ""
#endif

#define DTB_PART_SIZE			0x20000
#define KERNEL_PART_SIZE		0xc00000
#define ROOTFS_PART_SIZE		0x3a000000

#if 0
# define SPLASH_FLASH_BASE		0x120000 /* (CONFIG_MTD_SPLASH_PART_START) */
#endif
# define DTB_FLASH_BASE			0x5000000 /* (SPLASH_FLASH_BASE + CONFIG_MTD_SPLASH_PART_LEN) */
# define KERNEL_FLASH_BASE		0x4000000 /* (DTB_FLASH_BASE + DTB_PART_SIZE) */
# define ROOTFS_FLASH_BASE		0x6000000 /* (KERNEL_FLASH_BASE + KERNEL_PART_SIZE) */

#ifdef CONFIG_VIDEO
#define CONFIG_VIDEO_MODE \
	"panel=lcd-vf6-bsb\0"
#else
#define CONFIG_VIDEO_MODE ""
#endif

#define CONFIG_EXTRA_ENV_SETTINGS					\
	CONFIG_VIDEO_MODE						\
	"autoload=yes\0"						\
	"addip=setenv bootargs ${bootargs}"				\
	" ip=${ipaddr}:${serverip}:${gatewayip}:"			\
	"${netmask}:${hostname}:eth0:off "				\
	" fec_mac=${ethaddr}\0"						\
	"ethaddr=3C:FB:96:77:88:AB\0"					\
	"ipaddr=172.17.80.3\0"						\
	"serverip=172.17.0.1\0"						\
	"image=slx-som/rootfs.uImage\0"					\
	"bootcmd=run reliableboot\0"					\
	"reliableboot=run nandboot\0"					\
	"netboot=tftp ${image} && run args addip && run boot_dtb\0"	\
	"nandboot=nand read ${loadaddr} ${uImage_offset}"		\
	" ${flashsize} && run args addip && run boot_dtb\0"		\
	"args=run args_quiet\0"						\
	"args_common=console=ttymxc0,115200\0"		\
	"args_quiet=setenv bootargs ${args_common} ${ubirfs}"		\
	" quiet=quiet\0"						\
	"args_verbose=setenv bootargs ${args_common} ${ubirfs}"		\
	" ignore_loglevel\0"						\
	"ubirfs=mtdparts=gpmi-nand:64m(boot),16m(kernel),16m(dtb),-(rootfs)" \
	" rootwait=1 rw ubi.mtd=3,2048 rootfstype=ubifs"		\
	" root=ubi0:rootfs ubi.fm_autoconvert=1\0"			\
	"args_nfs=setenv bootargs ${args_common} ignore_loglevel"	\
	" root=/dev/nfs"						\
	" nfsroot=${serverip}:/mnt/nfs/slx-som/rootfs,v3,nolock\0"	\
	"verify=no\0"							\
	"bootdelay=1\0"							\
	"update=tftp ${image} && nand erase.spread "			\
	"${uImage_offset} ${filesize} && nand write ${loadaddr} "	\
	"${uImage_offset} ${filesize} "					\
	"&& setenv flashsize ${filesize}  && saveenv\0"			\
	"rootfsimage=rootfs.ubi\0"					\
	"rootfsupdate=tftp ${rootfsimage} && nand erase.spread "	\
	"${rootfs_offset} " __stringify(ROOTFS_PART_SIZE)			\
	" && nand write ${loadaddr} ${rootfs_offset} ${filesize}\0"	\
	"dtbimage=slx-som/rootfs.dtb\0"				\
	"dtbupdate=tftp ${dtbimage} && nand erase.spread "		\
	"${dtb_offset} ${filesize} && nand write ${loadaddr} "		\
	"${dtb_offset} ${filesize}\0"					\
	"boot_dtb=nand read ${dtb_addr} ${dtb_offset} "			\
	__stringify(DTB_PART_SIZE) " && bootm ${loadaddr} - ${dtb_addr}\0"	\
	"dtb_offset=" __stringify(DTB_FLASH_BASE) "\0"			\
	"uImage_offset=" __stringify(KERNEL_FLASH_BASE) "\0"		\
	"rootfs_offset=" __stringify(ROOTFS_FLASH_BASE) "\0"		\
	"dtb_addr=0x83000000\0"

/* Miscellaneous configurable options */
#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT		"SLX-SOM U-Boot > "
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_CBSIZE		1024

#define CONFIG_SYS_MAXARGS		256
#define CONFIG_SYS_BARGSIZE CONFIG_SYS_CBSIZE

#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START + 0x10000)

#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR
#define CONFIG_SYS_HZ			1000

#define CONFIG_CMDLINE_EDITING
#define CONFIG_STACKSIZE		SZ_128K

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM			MMDC0_ARB_BASE_ADDR
#define PHYS_SDRAM_SIZE			SZ_512M

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* FLASH and environment organization */
#define CONFIG_SYS_NO_FLASH

#define CONFIG_ENV_IS_IN_NAND
#define CONFIG_ENV_SIZE			(0x10000)
#define CONFIG_ENV_RANGE		(0x40000)
#define CONFIG_ENV_OFFSET		(0x200000)
#define CONFIG_ENV_OFFSET_REDUND	(0x220000)

#define CONFIG_OF_LIBFDT
#define CONFIG_CMD_BOOTZ

#define CONFIG_CMD_BMODE

#ifndef CONFIG_SYS_DCACHE_OFF
#define CONFIG_CMD_CACHE
#endif

#ifdef CONFIG_VIDEO
#define	CONFIG_CFB_CONSOLE
#define	CONFIG_VIDEO_MXS
#define	CONFIG_VIDEO_LOGO
#define	CONFIG_VIDEO_SW_CURSOR
#define	CONFIG_VGA_AS_SINGLE_DEVICE
#define	CONFIG_SYS_CONSOLE_IS_IN_ENV
#define	CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define	CONFIG_CMD_BMP
#define	CONFIG_BMP_16BPP
#define	CONFIG_VIDEO_BMP_RLE8
#define CONFIG_VIDEO_BMP_LOGO
#ifdef CONFIG_VIDEO_GIS
#define CONFIG_VIDEO_CSI
#define CONFIG_VIDEO_PXP
#define CONFIG_VIDEO_VADC
#endif
#endif

#define CONFIG_CMD_USB
#ifdef CONFIG_CMD_USB
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_MX6
#define CONFIG_USB_STORAGE
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_MXC_USB_PORTSC  (PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS   0
#define CONFIG_USB_MAX_CONTROLLER_COUNT 2
#endif

/*
 * The PCIe support in uboot would bring failures in i.MX6SX PCIe
 * EP/RC validations. Disable PCIe support in uboot here.
 * RootCause: The bit10(ltssm_en) of GPR12 would be set in uboot,
 * thus the i.MX6SX PCIe EP would be cheated that the other i.MX6SX
 * PCIe RC had been configured and trying to setup PCIe link directly,
 * although the i.MX6SX RC is not properly configured at that time.
 * PCIe can be supported in uboot, if the i.MX6SX PCIe EP/RC validation
 * is not running.
 */
/* #define CONFIG_CMD_PCI */
#undef CONFIG_CMD_PCI
#ifdef CONFIG_CMD_PCI
#define CONFIG_PCI
#define CONFIG_PCI_PNP
#define CONFIG_PCI_SCAN_SHOW
#define CONFIG_PCIE_IMX
#define CONFIG_PCIE_IMX_PERST_GPIO	IMX_GPIO_NR(2, 0)
#define CONFIG_PCIE_IMX_POWER_GPIO	IMX_GPIO_NR(2, 1)
#endif

#define CONFIG_CMD_FUSE
#ifdef CONFIG_CMD_FUSE
#define CONFIG_MXC_OCOTP
#endif

#define CONFIG_CMD_TIME

#if 1
#define CONFIG_CMD_NAND
#define CONFIG_CMD_NAND_TRIMFFS

/* NAND stuff */
#define CONFIG_NAND_MXS
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_BASE		0x40000000
#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_NAND_ONFI_DETECTION
#define CONFIG_NAND_MXS_BCH_LEGACY_GEO
#endif

/* DMA stuff, needed for GPMI/MXS NAND support */
#define CONFIG_APBH_DMA
#define CONFIG_APBH_DMA_BURST
#define CONFIG_APBH_DMA_BURST8

#endif				/* __CONFIG_H */
