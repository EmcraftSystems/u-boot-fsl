/*
 * Copyright 2013 Emcraft Systems
 *
 * Configuration settings for Vybrid-SOM Rev 1A.
 *
 * Pavel Boldin <paboldin@emcraft.com>
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

/*#define DEBUG*/

#define CONFIG_SYS_BOARD_REV            0x1A

/* #define CONFIG_SYS_ICACHE_OFF */
/* #define CONFIG_SYS_DCACHE_OFF */
/* #define CONFIG_SYS_ARM_CACHE_WRITETHROUGH */
/* #define CONFIG_SYS_L2CACHE_OFF */

#define CONFIG_MMC
#define CONFIG_MCFFEC

#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM_1_SIZE		(128 * 1024 * 1024)

#include "vybrid_som_common.h"

#define CONFIG_VYBRID_QSPI_128MBIT_DEVICE

#define CONFIG_ENV_IS_IN_QSPI_FLASH
#define CONFIG_ENV_OFFSET		(0x30000)
#define CONFIG_ENV_SIZE			(0x10000)

#undef CONFIG_LCD
#undef CONFIG_VIDEO_MVF_DCU
#undef LCD_TWR_RGB
#undef CONFIG_SYS_CONSOLE_IS_IN_ENV
#undef CONFIG_SYS_CONSOLE_OVERWRITE_ROUTINE
#undef CONFIG_SPLASH_SCREEN
#undef CONFIG_SPLASH_SCREEN_ALIGN
#undef CONFIG_SETUP_VIDEOLFB_TAG
#undef CONFIG_SETUP_MTDSPLASHPART_TAG

#define CONFIG_MTD_SPLASH_PART_START	0x40000
#define CONFIG_MTD_SPLASH_PART_LEN	0x80000

#define CONFIG_BMP
#undef CONFIG_CMD_BMP
#define CONFIG_BMP_24BPP
#define LCD_BPP	LCD_COLOR32

#include "vybrid_lcd_config.h"

#ifdef CONFIG_SETUP_MTDSPLASHPART_TAG

# if CONFIG_MTD_SPLASH_PART_LEN != 0x80000
#  error "Current partition config is for splash len = 0x80000"
# endif

# if CONFIG_MTD_SPLASH_PART_START != 0x40000
#  error "Current partition config is for splash start = 0x40000"
# endif

# define KERNEL_FLASH_BASE	"c0000"
# define KERNEL_MEM_BASE	"200c0000"

#else /* CONFIG_SPLASH_SCREEN */
# define KERNEL_FLASH_BASE	"40000"
# define KERNEL_MEM_BASE	"20040000"
#endif /* CONFIG_SPLASH_SCREEN */
#define CONFIG_HOSTNAME		vf6-som
#define CONFIG_EXTRA_ENV_SETTINGS					\
        "autoload=yes\0"						\
        "addip=setenv bootargs ${bootargs} "				\
                "ip=${ipaddr}:${serverip}:${gatewayip}:"		\
                        "${netmask}:${hostname}:eth0:off "		\
                "fec_mac=${ethaddr}\0"					\
        "ethaddr=C0:B1:3C:77:88:AB\0"					\
        "ipaddr=172.17.44.46\0"						\
        "serverip=172.17.0.1\0"						\
        "image=uImage\0"						\
	"netboot=tftp ${image};run addip;bootm\0"			\
	"bootcmd=qspi probe 1;cp.b " KERNEL_MEM_BASE " ${loadaddr} "	\
	"${flashsize};run addip;bootm\0"				\
	"bootargs=mem=" KERNEL_MEM_INFO " console=ttymxc0,115200 "	\
	LCD_BOOTARG "\0"						\
	"verify=no\0"							\
	"bootdelay=3\0"							\
	"update=tftp ${image} && qspi probe 1 && qspi erase "		\
	KERNEL_FLASH_BASE " +${filesize} && qspi write ${loadaddr} "	\
	KERNEL_FLASH_BASE " ${filesize} && setenv flashsize ${filesize}"\
	" && saveenv\0"
#endif /* __CONFIG_H */
