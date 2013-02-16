/*
 * Copyright 2012 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the vybrid Board
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/* High Level Configuration Options */

#define CONFIG_VYBRID

#define CONFIG_SYS_VYBRID_HCLK		24000000
#define CONFIG_SYS_VYBRID_CLK32		32768
#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

#define CONFIG_SYS_ICACHE_OFF
#define CONFIG_SYS_CACHELINE_SIZE	64

/*#define CONFIG_SYS_UBOOT_IN_GPURAM*/

#include <asm/arch/vybrid-regs.h>
/*
 * Disabled for now due to build problems under Debian and a significant
 * increase in the final file size: 144260 vs. 109536 Bytes.
 */

#define CONFIG_CMDLINE_TAG			/* enable passing of ATAGs */
#undef CONFIG_SETUP_MEMORY_TAGS
#undef CONFIG_INITRD_TAG

#undef CONFIG_OF_LIBFDT

#define CONFIG_MACH_TYPE		MACH_TYPE_VYBRID_VF6XX

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		((1024 * 2) << 10)

#define CONFIG_BOARD_LATE_INIT

/* Hardware drivers */
#define CONFIG_VYBRID_UART
#define CONFIG_VYBRID_UART_BASE		UART0_BASE
#define CONFIG_VYBRID_GPIO

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_SYS_UART_PORT		(1)
#define CONFIG_BAUDRATE			115200
#define CONFIG_SYS_BAUDRATE_TABLE	{9600, 19200, 38400, 57600, 115200}

/* Command definition */
#include <config_cmd_default.h>

#define CONFIG_CMD_BDI		/* bdinfo */
#define CONFIG_CMD_BOOTD
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_ELF
#define CONFIG_CMD_CONSOLE	/* coninfo */
#define CONFIG_CMD_MEMORY	/* md mm nm mw cp cmp crc base loop mtest */
#define CONFIG_CMD_MISC
#define CONFIG_CMD_MII
#define CONFIG_CMD_NET
#undef CONFIG_CMD_NFS		/* NFS support			*/
#define CONFIG_CMD_PING
#define	CONFIG_CMD_NAND
#undef CONFIG_CMD_DATE
#undef CONFIG_CMD_IMI		/* iminfo */
#undef CONFIG_CMD_IMLS
#undef CONFIG_CMD_LOADB		/* loadb */
#undef CONFIG_CMD_LOADS		/* loads */

#define CONFIG_MMC
#ifdef CONFIG_MMC
#define CONFIG_SYS_ESDHC1_BASE		ESDHC2_BASE_ADDR
#define CONFIG_SYS_ESDHC2_BASE		ESDHC2_BASE_ADDR
#define CONFIG_FSL_ESDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR	0
#define CONFIG_SYS_FSL_ESDHC_NUM	1
#define CONFIG_ESDHC_NO_SNOOP		1
/*#define CONFIG_MMC_TRACE*/

/*#define CONFIG_ESDHC_DETECT_USE_EXTERN_IRQ1*/
#define CONFIG_SYS_FSL_ERRATUM_ESDHC135
#define CONFIG_SYS_FSL_ERRATUM_ESDHC111
#define CONFIG_SYS_FSL_ERRATUM_ESDHC_A001

#define CONFIG_CMD_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_CMD_FAT
#define CONFIG_DOS_PARTITION
#endif

/*
 * NAND FLASH
 */
#ifdef CONFIG_CMD_NAND
#define CONFIG_JFFS2_NAND
#define CONFIG_NAND_FSL_NFC
#define CONFIG_SYS_NAND_BASE		0x400E0000
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define NAND_MAX_CHIPS			CONFIG_SYS_MAX_NAND_DEVICE
#define CONFIG_SYS_NAND_SELECT_DEVICE
#define	CONFIG_SYS_64BIT_VSPRINTF	/* needed for nand_util.c */
#endif

#define CONFIG_QUAD_SPI

/* Network configuration */
#define CONFIG_MCFFEC
#ifdef CONFIG_MCFFEC
#	define CONFIG_MII		1
#	define CONFIG_MII_INIT		1
#	define CONFIG_SYS_DISCOVER_PHY
#	define CONFIG_SYS_RX_ETH_BUFFER	8
#	define CONFIG_SYS_FAULT_ECHO_LINK_DOWN

#	define CONFIG_SYS_FEC0_PINMUX	0
#	define CONFIG_SYS_FEC1_PINMUX	0
#	define CONFIG_SYS_FEC0_IOBASE	MACNET0_BASE_ADDR
#	define CONFIG_SYS_FEC1_IOBASE	MACNET1_BASE_ADDR
#	define CONFIG_SYS_FEC0_MIIBASE	MACNET0_BASE_ADDR
#	define CONFIG_SYS_FEC1_MIIBASE	MACNET0_BASE_ADDR
#	define MCFFEC_TOUT_LOOP 50000
#	undef CONFIG_HAS_ETH1

#	define CONFIG_ETHADDR		00:e0:0c:bc:e5:60
#	define CONFIG_ETH1ADDR		00:e0:0c:bc:e5:61
#	define CONFIG_ETHPRIME		"FEC0"
#	define CONFIG_IPADDR		10.81.67.175
#	define CONFIG_NETMASK		255.255.252.0
#	define CONFIG_SERVERIP		10.81.64.153
#	define CONFIG_GATEWAYIP		10.81.67.254
/*
#	define CONFIG_IPADDR		192.162.1.2
#	define CONFIG_NETMASK		255.255.255.0
#	define CONFIG_SERVERIP		192.162.1.1
#	define CONFIG_GATEWAYIP		192.162.1.1
*/
#	define CONFIG_OVERWRITE_ETHADDR_ONCE

/* If CONFIG_SYS_DISCOVER_PHY is not defined - hardcoded */
#	ifndef CONFIG_SYS_DISCOVER_PHY
#		define FECDUPLEX	FULL
#		define FECSPEED		_100BASET
#	else
#		ifndef CONFIG_SYS_FAULT_ECHO_LINK_DOWN
#			define CONFIG_SYS_FAULT_ECHO_LINK_DOWN
#		endif
#	endif			/* CONFIG_SYS_DISCOVER_PHY */
#endif

#define CONFIG_BOOTDELAY		3
#define CONFIG_ETHPRIME			"FEC0"
#define CONFIG_LOADADDR			0x3f400000	/* loadaddr env var */
#define CONFIG_ARP_TIMEOUT		200UL

/* Miscellaneous configurable options */
#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_SYS_HUSH_PARSER		/* use "hush" command parser */
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#define CONFIG_SYS_PROMPT		"Vybrid U-Boot > "
#undef CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_CBSIZE		1024	/* Console I/O Buffer Size */
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE \
			(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS		16	/* max number of command args */
/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE

#define CONFIG_SYS_MEMTEST_START	0x3f400000
#define CONFIG_SYS_MEMTEST_END		0x10000

#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR

#define CONFIG_SYS_HZ			1000
#undef CONFIG_CMDLINE_EDITING

/*
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE		(128 * 1024)	/* regular stack */

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM_1_SIZE		(128 * 1024 * 1024)

/*#define CONFIG_SYS_SDRAM_BASE		(PHYS_SDRAM_1)*/
/*#define CONFIG_SYS_SDRAM_BASE		(CONFIG_SYS_INIT_RAM_ADDR)*/
#define CONFIG_SYS_SDRAM_BASE		(0x80000000)
#define CONFIG_SYS_INIT_RAM_ADDR	(IRAM_BASE_ADDR)
#define CONFIG_SYS_INIT_RAM_SIZE	(IRAM_SIZE)

#define CONFIG_BOARD_EARLY_INIT_F

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* timer */
#define FTM_BASE_ADDR			FTM0_BASE_ADDR
#define CONFIG_TMR_USEPIT

/* clock/PLL configuration */
#define CONFIG_SYS_CLKCTL_CCGR0		0xFFFFFFFF
#define CONFIG_SYS_CLKCTL_CCGR1		0xFFFFFFFF
#define CONFIG_SYS_CLKCTL_CCGR2		0xFFFFFFFF
#define CONFIG_SYS_CLKCTL_CCGR3		0xFFFFFFFF
#define CONFIG_SYS_CLKCTL_CCGR4		0xFFFFFFFF
#define CONFIG_SYS_CLKCTL_CCGR5		0xFFFFFFFF
#define CONFIG_SYS_CLKCTL_CCGR6		0xFFFFFFFF
#define CONFIG_SYS_CLKCTL_CCGR7		0xFFFFFFFF
#define CONFIG_SYS_CLKCTL_CCGR8		0x3FFFFFFF
#define CONFIG_SYS_CLKCTL_CCGR9		0xFFFFFFFF
#define CONFIG_SYS_CLKCTL_CCGR10	0xFFFFFFFF
#define CONFIG_SYS_CLKCTL_CCGR11	0xFFFFFFFF

#define CONFIG_SYS_CLKCTRL_CCR		0x00010005
#define CONFIG_SYS_CLKCTRL_CCSR		0x0003FF24
#define CONFIG_SYS_CLKCTRL_CACRR	0x00000810
#define CONFIG_SYS_CLKCTRL_CSCMR1	0x03CA0000
#define CONFIG_SYS_CLKCTRL_CSCDR1	0x01000000
#define CONFIG_SYS_CLKCTRL_CSCDR2	0x30114240
#define CONFIG_SYS_CLKCTRL_CSCDR3	0x00003F1F
#define CONFIG_SYS_CLKCTRL_CSCMR2	0x00000000
#define CONFIG_SYS_CLKCTRL_CSCDR4	0x00000000
#define CONFIG_SYS_CLKCTRL_CLPCR	0x00000078

#define CONFIG_SYS_ANADIG_USB1_CTRL	0x00012000
#define CONFIG_SYS_ANADIG_USB2_CTRL	0x00012000
#define CONFIG_SYS_ANADIG_528_CTRL	0x00002001
#define CONFIG_SYS_ANADIG_528_SS	0x00000000
#define CONFIG_SYS_ANADIG_528_NUM	0x00000000
#define CONFIG_SYS_ANADIG_528_DENOM	0x00000012
#define CONFIG_SYS_ANADIG_VID_CTRL	0x00011028
#define CONFIG_SYS_ANADIG_VID_NUM	0x00000000
#define CONFIG_SYS_ANADIG_VID_DENOM	0x00000012
#define CONFIG_SYS_ANADIG_ENET_CTRL	0x00011001
#define CONFIG_SYS_ANADIG_PFD_USB1	0x1B1D1A1C
#define CONFIG_SYS_ANADIG_PFD_528	0x171C1813
#define CONFIG_SYS_ANADIG_USB1_MISC	0x00000002
#define CONFIG_SYS_ANADIG_USB2_VBUS	0x00100004
#define CONFIG_SYS_ANADIG_USB2_CHRG	0x00000000
#define CONFIG_SYS_ANADIG_USB2_MISC	0x00000002
#define CONFIG_SYS_ANADIG_SYS_CTRL	0x00002001
#define CONFIG_SYS_ANADIG_SYS_SS	0x00000000
#define CONFIG_SYS_ANADIG_SYS_NUM	0x00000000
#define CONFIG_SYS_ANADIG_SYS_DENOM	0x00000012
#define CONFIG_SYS_ANADIG_SYS_PFD_528	0x00000000
#define CONFIG_SYS_ANADIG_SYS_PLL_LOCK	0x00000000

/* FLASH and environment organization */
#define CONFIG_SYS_NO_FLASH

#define CONFIG_ENV_OFFSET		(6 * 64 * 1024)
#define CONFIG_ENV_SIZE			(8 * 1024)
#undef CONFIG_ENV_IS_IN_MMC
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_SYS_MMC_ENV_DEV		0

#endif
