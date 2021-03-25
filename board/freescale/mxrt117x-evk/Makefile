#
# (C) Copyright 2017-2021 Emcraft Systems
# Dmitry Konyshev <probables@emcraft.com>
# Vladimir Skvortsov <vskvortsov@emcraft.com>
# 
# SPDX-License-Identifier:	GPL-2.0+
#

ccflags-y := -I$(CONFIG_BOARDDIR)/CMSIS \
	-I$(CONFIG_BOARDDIR)/drivers \
	-I$(CONFIG_BOARDDIR)/device \
	-DCPU_MIMXRT1176DVMAA_cm7 \
	-std=gnu99

obj-y	:= mxrt117x-evk.o	\
	./device/system_MIMXRT1176_cm7.o	\
	./drivers/fsl_gpio.o	\
	./drivers/fsl_clock.o	\
	./drivers/fsl_common.o \
	./drivers/fsl_anatop_ai.o \
	./drivers/fsl_pmu.o \
	./drivers/fsl_semc.o \
	./board/pin_mux.o	\
	./board/board.o	\
	./board/clock_config.o \

flexspi_cb.bin: board/$(BOARDDIR)/flexspi_cb.o
	$(OBJCOPY) -O binary board/$(BOARDDIR)/flexspi_cb.o $@
