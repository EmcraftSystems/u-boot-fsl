/*
 * (C) Copyright 2017 Emcraft Systems
 * Yuri Tikhonov <yur@emcraft.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _SYS_PROTO_H_
#define _SYS_PROTO_H_

#include <asm/mach-imx/sys_proto.h>

void mxs_set_lcdclk(uint32_t __maybe_unused lcd_base, uint32_t freq);
int mxs_reset_block(struct mxs_register_32 *reg);

#endif /* _SYS_PROTO_H_ */
