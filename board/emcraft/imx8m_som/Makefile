#
# Copyright 2016 Freescale Semiconductor
#
# SPDX-License-Identifier:      GPL-2.0+
#

obj-y += imx8m_som.o

ifdef CONFIG_SPL_BUILD
obj-y += spl.o
#obj-y += ddr/ddr_init.o ddr/ddrphy_train.o ddr/helper.o
ifdef CONFIG_TARGET_EMCRAFT_IMX8M_SOM
obj-y += ddr/ddr3l/ddr3_phyinit_train_1600mts_x16_ret.o \
	 ddr/ddr3l/ddr3_pmu_training_1600mts_x16_ret.o \
	 ddr/ddr3l/dram_pll_cfg.o ddr/ddr3l/helper.o
else
obj-y += ddr/lpddr4/ddr_init.o ddr/lpddr4/ddrphy_train.o \
	 ddr/lpddr4/helper.o
endif
endif
