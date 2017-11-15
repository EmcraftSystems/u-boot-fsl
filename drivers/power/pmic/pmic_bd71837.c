/*
 * Copyright (C) 2017 EmCraft Systems
 * Yuri Tikhonov <yur@emcraft.com>
 *
 * SPDX-License-Identifier:      GPL-2.0+
 */

#include <common.h>
#include <errno.h>
#include <i2c.h>
#include <power/pmic.h>
#include <power/bd71837_pmic.h>

/*
 * PMIC device name
 */
static const char bd71837_name[] = BD71837_NAME;

/*
 * Allocate PMIC device
 */
int power_bd71837_init(unsigned char bus)
{
	struct pmic *p;
	int rv;

	p = pmic_alloc();
	if (!p) {
		printf("%s: pmic_alloc() error!\n", __func__);
		rv = -ENOMEM;
		goto out;
	}

	p->name = bd71837_name;
	p->interface = PMIC_I2C;
	p->number_of_regs = BD71837_NUM_OF_REGS;
	p->hw.i2c.addr = CONFIG_POWER_BD71837_I2C_ADDR;
	p->hw.i2c.tx_num = 1;
	p->bus = bus;

	rv = 0;
out:
	return rv;
}
