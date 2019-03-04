#
# Copyright 2018 Emcraft Systems
#
# SPDX-License-Identifier:      GPL-2.0+
#

obj-y += imx8mm_som.o

ifdef CONFIG_SPL_BUILD
obj-y += spl.o
ifdef CONFIG_TARGET_EMCRAFT_IMX8MM_SOM_1GB
obj-y += lpddr4_timing-8Gb.o
else
ifdef CONFIG_TARGET_EMCRAFT_IMX8MM_SOM_3GB
obj-y += lpddr4_timing-24Gb.o
else
obj-$(CONFIG_IMX8M_LPDDR4) += lpddr4_timing.o
obj-$(CONFIG_IMX8M_DDR4) += ddr4_timing.o
endif
endif
endif
