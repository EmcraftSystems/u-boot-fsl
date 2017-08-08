/*
 * Copyright (C) 2017 Emcraft Systems
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _ASM_ARCH_CLOCK_H
#define _ASM_ARCH_CLOCK_H

#include <common.h>

u32 imx_get_uartclk(void);
u32 imx_get_fecclk(void);
void clock_init(void);

#endif
