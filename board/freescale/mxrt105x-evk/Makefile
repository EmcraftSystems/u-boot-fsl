#
# (C) Copyright 2017 Emcraft Systems
# Dmitry Konyshev <probables@emcraft.com>
#
# SPDX-License-Identifier:	GPL-2.0+
#

ccflags-y := -I$(CONFIG_BOARDDIR)/CMSIS/Include -I$(CONFIG_BOARDDIR)/CMSIS/Driver/Include \
	-I$(CONFIG_BOARDDIR)/devices/MIMXRT1052 -I$(CONFIG_BOARDDIR)/devices/MIMXRT1052/drivers \
	-I$(CONFIG_BOARDDIR)/devices/MIMXRT1052/utilities \
	-DCPU_MIMXRT1052DVL6A -DPRINTF_FLOAT_ENABLE=0 -DSCANF_FLOAT_ENABLE=0 \
	-DPRINTF_ADVANCED_ENABLE=0 -DSCANF_ADVANCED_ENABLE=0 -DEVK_MCIMXRM

obj-y	:= mxrt105x-evk.o	\
	./CMSIS/Driver/DriverTemplates/Driver_USART.o	\
	./CMSIS/Driver/DriverTemplates/Driver_CAN.o	\
	./CMSIS/Driver/DriverTemplates/Driver_MCI.o	\
	./CMSIS/Driver/DriverTemplates/Driver_SAI.o	\
	./CMSIS/Driver/DriverTemplates/Driver_SPI.o	\
	./CMSIS/Driver/DriverTemplates/Driver_USBD.o	\
	./CMSIS/Driver/DriverTemplates/Driver_USBH.o	\
	./CMSIS/Driver/DriverTemplates/Driver_ETH_MAC.o	\
	./CMSIS/Driver/DriverTemplates/Driver_Storage.o	\
	./CMSIS/Driver/DriverTemplates/Driver_Flash.o	\
	./CMSIS/Driver/DriverTemplates/Driver_ETH_PHY.o	\
	./CMSIS/Driver/DriverTemplates/Driver_I2C.o	\
	./devices/MIMXRT1052/system_MIMXRT1052.o	\
	./devices/MIMXRT1052/drivers/fsl_gpio.o	\
	./devices/MIMXRT1052/drivers/fsl_cache.o	\
	./devices/MIMXRT1052/drivers/fsl_lpuart.o	\
	./devices/MIMXRT1052/drivers/fsl_usdhc.o	\
	./devices/MIMXRT1052/drivers/fsl_clock.o	\
	./devices/MIMXRT1052/drivers/fsl_pit.o	\
	./devices/MIMXRT1052/drivers/fsl_enet.o	\
	./boards/evkmimrt1050/demo_apps/fat_tool/pin_mux.o	\
	./boards/evkmimrt1050/demo_apps/fat_tool/board.o	\
	./boards/evkmimrt1050/demo_apps/fat_tool/clock_config.o \
	./boards/evkmimrt1050/demo_apps/fat_tool/fsl_sdram.o	\
	./boards/evkmimrt1050/demo_apps/fat_tool/fsl_phy.o

#	./devices/MIMXRT1052/drivers/fsl_common.o	\
	./devices/MIMXRT1052/drivers/fsl_lpi2c.o	\
	./devices/MIMXRT1052/drivers/fsl_elcdif.o	\
	./devices/MIMXRT1052/drivers/fsl_sai.o	\
	./devices/MIMXRT1052/drivers/fsl_wdog.o	\
	./devices/MIMXRT1052/drivers/fsl_flexspi.o	\
	./devices/MIMXRT1052/utilities/fsl_str.o	\
	./devices/MIMXRT1052/utilities/fsl_log.o	\
	./devices/MIMXRT1052/utilities/fsl_notifier.o	\
	./devices/MIMXRT1052/utilities/fsl_io.o	\
	./devices/MIMXRT1052/utilities/fsl_debug_console.o	\
#	./devices/MIMXRT1052/drivers/fsl_edma.o	\
