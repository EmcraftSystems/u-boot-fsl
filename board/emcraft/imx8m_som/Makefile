#
# Copyright 2016 Freescale Semiconductor
#
# SPDX-License-Identifier:      GPL-2.0+
#

obj-y += imx8m_som.o

ifdef CONFIG_SPL_BUILD
obj-y += spl.o
ifdef CONFIG_TARGET_EMCRAFT_IMX8M_LPDDR4_SOM
obj-y += lpddr4_timing-mx8m_micron_lpddr4_1gb_1d_1600m_334m_50m_32bit_RPAv21.o
else ifdef CONFIG_TARGET_EMCRAFT_IMX8M_LPDDR4_2GB_SOM
obj-y += lpddr4_timing-mx8m_micron_lpddr4_2gb_2d_1600m_334m_50m_32bit_RPAv21.o
else ifdef CONFIG_TARGET_EMCRAFT_IMX8M_LPDDR4_3GB_SOM
obj-y += lpddr4_timing-mx8m_micron_lpddr4_3gb_2d_1600m_334m_50m_32bit_RPAv21.o
else ifdef CONFIG_TARGET_EMCRAFT_IMX8M_LPDDR4_4GB_SOM
obj-y += lpddr4_timing-mx8m_micron_lpddr4_4gb_2d_1600m_334m_50m_32bit_RPAv21.o
else
#error Unknown Emcraft i.MX8M SOM variant
endif
endif
