#
# Copyright 2016 Freescale Semiconductor
#
# SPDX-License-Identifier:      GPL-2.0+
#

obj-y += imx8m_som.o

ifdef CONFIG_SPL_BUILD
obj-y += spl.o
#obj-y += ddr/ddr_init.o ddr/ddrphy_train.o ddr/helper.o
obj-y += ddr/ddr3_phyinit_train_1600mts_x16_ret.o
obj-y += ddr/ddr3_pmu_training_1600mts_x16_ret.o
obj-y += ddr/dram_pll_cfg.o
obj-y += ddr/helper.o
endif
