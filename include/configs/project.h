/*
 * Copyright 2016 Emcraft Systems
 *
 * Configuration settings for Ditting BSB board.
 *
 * Sergei Miroshnichenko <sergeimir@emcraft.com>
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

#define tostring(s)			#s
#define stringify(s)			tostring(s)

/*#define DEBUG*/

#define CONFIG_SYS_CLOCK_FREQUENCY	498000000UL

#define CONFIG_SYS_BOARD_NAME		"DITTING-BSB"
#define CONFIG_VF6_SOM_LC
#define CONFIG_SYS_BOARD_REV_STR        "1A"

/* #define CONFIG_SYS_ICACHE_OFF */
/* #define CONFIG_SYS_DCACHE_OFF */
/* #define CONFIG_SYS_ARM_CACHE_WRITETHROUGH */
/* #define CONFIG_SYS_L2CACHE_OFF */

#define CONFIG_MMC
#define CONFIG_MCFFEC

#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_NAND_SIZE			(256 * 1024 * 1024)
#define PHYS_SDRAM_1_SIZE		(512 * 1024 * 1024)
#define PHYS_RAM_VIRT_SIZE		(256 * 1024 * 1024)

#include "vybrid_som_common.h"

#define CONFIG_SYS_NAND_BUSWIDTH_16BIT

#define CONFIG_ENV_IS_IN_NAND
#define CONFIG_ENV_SIZE			(0x10000)
#define CONFIG_ENV_RANGE		(0x40000)
#define CONFIG_ENV_OFFSET		(0x80000)
#define CONFIG_ENV_OFFSET_REDUND	(0xc0000)

#define CONFIG_LCD
#define CONFIG_VIDEO_MVF_DCU
#define CONFIG_PROMATE_ILITEK98
#define LCD_PROMATE7
#define CONFIG_SYS_CONSOLE_IS_IN_ENV
#define CONFIG_SYS_CONSOLE_OVERWRITE_ROUTINE
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_SETUP_VIDEOLFB_TAG
#define CONFIG_SETUP_MTDSPLASHPART_TAG

#undef	CONFIG_DCU_BACKLIGHT_GPIO_NUM
#define CONFIG_DCU_BACKLIGHT_GPIO_NUM		24

#define CONFIG_DCU_POWERDOWN_GPIO_ADDR         (GPIO_BASE_ADDR + 0x40)
#define CONFIG_DCU_POWERDOWN_GPIO_ADDR_SET     (GPIO_BASE_ADDR + 0x40 + 4)
#define CONFIG_DCU_POWERDOWN_GPIO_ADDR_CLEAR   (GPIO_BASE_ADDR + 0x40 + 8)
#define CONFIG_DCU_POWERDOWN_GPIO_NUM		(56 - 32)

#define CONFIG_PHY_ENABLE_GPIO			31
#define CONFIG_PHY_ENABLE_GPIO_ACTIVE_LVL	0

/* PTA21: 2Ch offset: 0x2C / 4 = 11*/
#define CONFIG_RECOVERY_BOOT_GPIO		11
#define CONFIG_RECOVERY_BOOT_GPIO_ACTIVE_LVL	0

#define CONFIG_CODEC_GPIO			88 /* PTD9 */
#define CONFIG_CODEC_GPIO_ACTIVE_LVL		0

#define CONFIG_OF_LIBFDT

#define CONFIG_SOFT_SPI

#define CONFIG_CMD_SET_BOOT_MEDIA
#define CONFIG_BOOT_MEDIA_NAND

#ifdef CONFIG_SOFT_SPI
#define SPI_SCL(val)				spi_bitbang_scl(val)
#define SPI_SDA(val)				spi_bitbang_sda(val)
#define SPI_READ				spi_bitbang_read()
#define SPI_DELAY				udelay(10)
#ifndef __ASSEMBLY__
#define CONFIG_SPI_BITBANG_CS_GPIO		41
#define CONFIG_SPI_BITBANG_SIN_GPIO		42
#define CONFIG_SPI_BITBANG_SOUT_GPIO		43
#define CONFIG_SPI_BITBANG_SCK_GPIO		44
void spi_bitbang_scl(int set);
void spi_bitbang_sda(int set);
unsigned char spi_bitbang_read(void);
#endif /* __ASSEMBLY__ */
#endif /* CONFIG_SOFT_SPI */

#define CONFIG_MTD_SPLASH_PART_START	0x120000
#define CONFIG_MTD_SPLASH_PART_LEN	0x180000

#define CONFIG_BMP
#undef CONFIG_CMD_BMP
#define CONFIG_BMP_24BPP
#define LCD_BPP	LCD_COLOR32

#include "vybrid_lcd_config.h"

# if CONFIG_MTD_SPLASH_PART_LEN != 0x180000
#  error "Current partition config is for splash len = 0x180000"
# endif

# if CONFIG_MTD_SPLASH_PART_START != 0x120000
#  error "Current partition config is for splash start = 0x140000"
# endif

#define DTB_PART_SIZE			0x100000
#define KERNEL_PART_SIZE		0xc00000
#define ROOTFS_PART_SIZE		0xa000000
#define RECOVERY_PART_SIZE		0x1000000

# define SPLASH1_FLASH_BASE		(CONFIG_MTD_SPLASH_PART_START)
# define DTB1_FLASH_BASE		(SPLASH1_FLASH_BASE + CONFIG_MTD_SPLASH_PART_LEN)
# define KERNEL1_FLASH_BASE		(DTB1_FLASH_BASE + DTB_PART_SIZE)
# define ROOTFS1_FLASH_BASE		(KERNEL1_FLASH_BASE + KERNEL_PART_SIZE)

# define RECOVERY_FLASH_BASE		(PHYS_NAND_SIZE - RECOVERY_PART_SIZE)
# define RECOVERY_DTB_FLASH_BASE	(RECOVERY_FLASH_BASE - DTB_PART_SIZE)


#define CONFIG_HOSTNAME		ditting
#define CONFIG_EXTRA_ENV_SETTINGS					\
	"autoload=yes\0"						\
	"addip=setenv bootargs ${bootargs}"				\
	" ip=${ipaddr}:${serverip}:${gatewayip}:"			\
	" ${netmask}:${hostname}:eth0:off "				\
	" fec_mac=${ethaddr}\0"						\
	"ethaddr=C0:B1:3C:77:88:AB\0"					\
	"ipaddr=172.17.80.3\0"						\
	"serverip=172.17.0.1\0"						\
	"image=ditting/ditting.uImage\0"				\
	"bootcmd=run reliableboot\0"					\
	"netboot=tftp ${image} && run args addip && run boot_dtb\0"	\
	"nandboot=nboot ${loadaddr} 0 ${uImage_offset} "		\
	"&& run args addip && run boot_dtb\0"		\
	"args=run args_quiet\0"						\
	"args_common=mem=256M console=ttymxc0,115200"			\
	" ubi.fm_autoconvert=1 lpj=1646592 fbcon=rotate:3\0"		\
	"args_quiet=setenv bootargs ${args_common} ${ubirfs}"		\
	" quiet=quiet\0"						\
	"args_verbose=setenv bootargs ${args_common} ${ubirfs}"		\
	" ignore_loglevel\0"						\
	"args_nfs=setenv bootargs ${args_common} ignore_loglevel"	\
	" root=/dev/nfs"						\
	" nfsroot=172.17.0.19:/mnt/nfs/ditting/rootfs,v3,nolock\0"	\
	"verify=no\0"							\
	"bootdelay=1\0"							\
	"splashimage=0x80007fc0\0"					\
	"splashfile=ditting/boot_screen.bmp\0"				\
	"splashpos='m,m'\0"						\
	"splashupdate=tftp ${splashfile} && nand erase.spread "		\
	"${splash_offset} ${filesize} && nand write ${loadaddr} "	\
	"${splash_offset} ${filesize}\0"				\
	"update=tftp ${image} && nand erase.spread "			\
	"${uImage_offset} ${filesize} && nand write ${loadaddr} "	\
	"${uImage_offset} ${filesize}\0"					\
	"rootfsimage=secondary.ubi\0"					\
	"rootfsupdate=tftp ${rootfsimage} && nand erase.spread "	\
	"${rootfs_offset} " stringify(ROOTFS_PART_SIZE)			\
	" && nand write ${loadaddr} ${rootfs_offset} ${filesize}\0"	\
	"recoverydtbimage=ditting/project-recovery-ditting.dtb\0"	\
	"recoveryimage=ditting/project-recovery-ditting.uImage\0"	\
	"recoveryupdate=tftp ${recoveryimage} && nand erase.spread "	\
	"${recovery_offset} ${filesize} && nand write ${loadaddr} "	\
	"${recovery_offset} ${filesize} && "				\
	"tftp ${recoverydtbimage} && nand erase.spread "		\
	"${recovery_dtb_offset} ${filesize} && nand write ${loadaddr} "	\
	"${recovery_dtb_offset} ${filesize}\0"				\
	"dtbimage=ditting/ditting.dtb\0"				\
	"dtbupdate=tftp ${dtbimage} && nand erase.spread "		\
	"${dtb_offset} ${filesize} && nand write ${loadaddr} "		\
	"${dtb_offset} ${filesize}\0"					\
	"activesetboot=run nandboot\0"					\
	"recoveryboot=run nandboot\0"					\
	"boot_dtb=nand read ${dtb_addr} ${dtb_offset} 0x7000 &&"	\
	" bootm ${loadaddr} - ${dtb_addr}\0"				\
	"dtb_addr=0x80000100\0"						\
	"zrec=setenv active_boot_set -1 && "				\
	" setenv boot_set1_valid 0; saveenv; reset\0"			\
	"uboot_image=ditting/u-boot.nand\0"				\
	"update_uboot=tftpboot ${uboot_image} && "			\
	" nand erase 0 0x60000 && nand write ${loadaddr} 0 ${filesize}\0"

#endif /* __CONFIG_H */
