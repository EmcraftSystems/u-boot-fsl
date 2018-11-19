#
# (C) Copyright 2017 Emcraft Systems
# Dmitry Konyshev <probables@emcraft.com>
#
# SPDX-License-Identifier:	GPL-2.0+
#

ccflags-y := -I$(CONFIG_BOARDDIR)/CMSIS \
	-I$(CONFIG_BOARDDIR)/drivers \
	-I$(CONFIG_BOARDDIR)/utilities \
	-DCPU_MIMXRT1062DVL6A -DPRINTF_FLOAT_ENABLE=0 -DSCANF_FLOAT_ENABLE=0 \
	-DPRINTF_ADVANCED_ENABLE=0 -DSCANF_ADVANCED_ENABLE=0 -std=gnu99 \
	-DDEBUG_CONSOLE_IO_LPUART=1
#-DEVK_MCIMXRM

obj-y	:= mxrt106x-evk.o	\
	./CMSIS/system_MIMXRT1062.o	\
	./drivers/fsl_gpio.o	\
	./drivers/fsl_lpuart.o	\
	./drivers/fsl_clock.o	\
	./board/pin_mux.o	\
	./board/peripherals.o	\
	./board/src/board.o	\
	./board/src/clock_config.o \
	./board/src/fsl_sdram.o \
	./drivers/fsl_usdhc.o	\
	./drivers/fsl_enet.o
