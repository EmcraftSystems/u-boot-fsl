/*
 * (C) Copyright 2017-2021 Emcraft Systems
 * Dmitry Konyshev, <probables@emcraft.com>
 * Vladimir Skvortsov <vskvortsov@emcraft.com>
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

#if defined CONFIG_SPI_BOOT
#define CONFIG_SYS_TEXT_BASE		0x30002000
#define IMX_FLEXSPI_CONFIG_BLOCK
#else
#define CONFIG_SYS_TEXT_BASE		0x20241000
#endif

#define CONFIG_SYS_CLK_FREQ		1000000000 /* 600 MHz */

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
#define PHYS_SDRAM_SIZE			(64 * 1024 * 1024)

#define DMAMEM_SZ_ALL			(1 * 1024 * 1024)
#define DMAMEM_BASE			(PHYS_SDRAM + PHYS_SDRAM_SIZE - \
					 DMAMEM_SZ_ALL)

#if defined(CONFIG_CMD_MEMTEST)
#define CONFIG_SYS_MEMTEST_START	PHYS_SDRAM
#define CONFIG_SYS_MEMTEST_END      	(CONFIG_SYS_MEMTEST_START + PHYS_SDRAM_SIZE - (2 * 1024 * 1024))
#endif

#define CONFIG_SYS_INIT_SP_ADDR		(0x20040000)

#define CONFIG_BOUNCE_BUFFER
#define CONFIG_FSL_ESDHC
#define CONFIG_FSL_USDHC
#define CONFIG_SUPPORT_EMMC_BOOT /* eMMC specific */

#define CONFIG_SYS_FSL_USDHC_NUM        1

#define CONFIG_CMD_FAT			1
#define CONFIG_SYS_FSL_ERRATUM_ESDHC135 1
#define ESDHCI_QUIRK_BROKEN_TIMEOUT_VALUE

/* Network */

#if defined (CONFIG_FEC_MXC)
#define CONFIG_MII

#define CONFIG_FEC_MXC_PHYADDR          0x2

#define CONFIG_FEC_XCV_TYPE             RMII
#define CONFIG_ETHPRIME                 "FEC"
#define FEC_QUIRK_ENET_MAC

#define CONFIG_CMD_MII
#endif


#if defined(CONFIG_FSL_FLEXSPI)
#define CONFIG_FSL_FLEXSPI_IOBASE	0x400cc000
#define CONFIG_FSL_FLEXSPI_IOSIZE	0x00000300
#define CONFIG_FSL_FLEXSPI_FLASHBASE	0x30000000
#define CONFIG_FSL_FLEXSPI_FLASHSIZE	0x01000000
#define CONFIG_FSL_FLEXSPI_RXFIFO_SIZE	128
#define CONFIG_FSL_FLEXSPI_TXFIFO_SIZE	128
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

#if defined(CONFIG_ENV_IS_IN_SPI_FLASH)
/* Environemnt is in SPI flash */
#define CONFIG_ENV_SIZE			(0x10000)
#define CONFIG_SYS_REDUNDAND_ENVIRONMENT
#define CONFIG_ENV_OFFSET		0x50000		/* 256K */
#define CONFIG_ENV_OFFSET_REDUND	(CONFIG_ENV_OFFSET + CONFIG_ENV_SIZE)
#define CONFIG_ENV_SECT_SIZE		0x10000
#else
#define CONFIG_ENV_SIZE			(8 << 10)
#endif

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


#define CONFIG_BOOTCOMMAND						\
	"run sfboot"

#define CONFIG_BOOTARGS							\
	"console=ttyLP0,115200 consoleblank=0 ignore_loglevel "


#define CONFIG_EXTRA_ENV_SETTINGS					\
	"addip=setenv bootargs ${bootargs} ip=${ipaddr}:${serverip}:"	\
		"${gatewayip}:${netmask}:${hostname}:eth0:off\0"	\
	"ethaddr=aa:bb:cc:dd:ee:f0\0"					\
	"serverip=172.17.0.1\0"						\
	"ipaddr=172.17.44.111\0"					\
	"netmask=255.255.0.0\0"						\
	"ini=mxrt117x-evk.ini\0"					\
	"image=rootfs.uImage\0"						\
	"tftpdir=imxrt117x/\0"						\
	"sfboot=run addip && bootm ${image_sf_addr}\0"			\
	"fdt_addr_r=0x81000000\0"					\
	"uboot_sf_offset=0x0\0"						\
	"uboot_sf_size=0x50000\0"					\
	"kernel_sf_offset=0x80000\0"					\
	"kernel_sf_size=0xa00000\0"					\
	"image_sf_addr=0x30080000\0"					\
	"netboot=tftp ${tftpdir}${image} &&"				\
		" run addip; bootm ${loadaddr}\0"			\
	"sf_kernel_update=tftp ${tftpdir}${image} &&"			\
		" sf erase ${kernel_sf_offset} ${kernel_sf_size} &&"	\
		" sf write ${loadaddr} ${kernel_sf_offset} ${filesize}\0"\
	"uboot=u-boot.flexspi\0"					\
	"sf_uboot_update=tftp ${tftpdir}${uboot} &&"			\
		" sf erase ${uboot_sf_offset} ${uboot_sf_size} &&"	\
		" sf write ${loadaddr} ${uboot_sf_offset} ${filesize}\0"

/*
 * Command line configuration.
 */
#define CONFIG_SYS_LONGHELP
#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING
#define CONFIG_CMD_CACHE
#define CONFIG_BOARD_LATE_INIT

#undef CONFIG_CMD_IMLS
#define CONFIG_SYS_BOOTM_LEN		(16*1024*1024)

#endif /* __CONFIG_H */
