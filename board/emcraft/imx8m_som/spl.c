/*
 * Copyright 2017 NXP
 * Copyright 2017 EmCraft Systems
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <spl.h>
#include <asm/io.h>
#include <errno.h>
#include <asm/io.h>
#include <asm/imx-common/iomux-v3.h>
#include <asm/arch/ddr_memory_map.h>
#include <asm/arch/imx8mq_pins.h>
#include <asm/arch/sys_proto.h>
#include <power/pmic.h>
#include <power/bd71837_pmic.h>
#include <asm/arch/clock.h>
#include <asm/imx-common/gpio.h>
#include <asm/imx-common/mxc_i2c.h>
#include <fsl_esdhc.h>
#include <mmc.h>
#include <nand.h>

#ifdef CONFIG_TARGET_EMCRAFT_IMX8M_SOM
#include "ddr/ddr3l/ddr.h"
#else
#include "ddr/lpddr4/ddr.h"
#endif

DECLARE_GLOBAL_DATA_PTR;

void spl_dram_init(void)
{
	/* ddr init */
#ifdef CONFIG_TARGET_EMCRAFT_IMX8M_SOM
	ddr3_pub_train_1600mts_ret_16bit_1rank();
#else
	ddr_init();
#endif
        udelay(100000);
        if(0)
        {
            volatile uint32_t  *addrA = 0x40000000;

            volatile uint32_t  *addrB = 0x50000000;

            volatile uint32_t  *addr;

            for(addr = addrA; addr < addrB; addr++){
                //*addr = addr;
                writel((uint32_t)addr, (uint32_t)addr);
            }


            //writel((uint32_t)0, (uint32_t)0x40100000);

            for(addr = addrA; addr < addrB; addr++){
                //readl((uint32_t)addr):
                //if(*addr != (uint32_t)addr)
                if(readl((uint32_t)addr) != (uint32_t)addr)
                {
                    printf(".................. addr check fail at 0x%x \n", addr);
                    while(1);
                }
            }

            printf(".................. 256M addr check pass \n");
        }


}

#define I2C_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_HYS | PAD_CTL_PUE)
#define PC MUX_PAD_CTRL(I2C_PAD_CTRL)
struct i2c_pads_info i2c_pad_info1 = {
	.scl = {
		.i2c_mode = IMX8MQ_PAD_I2C1_SCL__I2C1_SCL | PC,
		.gpio_mode = IMX8MQ_PAD_I2C1_SCL__GPIO5_IO14 | PC,
		.gp = IMX_GPIO_NR(5, 14),
	},
	.sda = {
		.i2c_mode = IMX8MQ_PAD_I2C1_SDA__I2C1_SDA | PC,
		.gpio_mode = IMX8MQ_PAD_I2C1_SDA__GPIO5_IO15 | PC,
		.gp = IMX_GPIO_NR(5, 15),
	},
};

#define USDHC2_CD_GPIO	IMX_GPIO_NR(2, 12)
#define USDHC1_PWR_GPIO IMX_GPIO_NR(2, 10)
#define USDHC2_PWR_GPIO IMX_GPIO_NR(2, 19)

int board_mmc_getcd(struct mmc *mmc)
{
	struct fsl_esdhc_cfg *cfg = (struct fsl_esdhc_cfg *)mmc->priv;
	int ret = 0;

	switch (cfg->esdhc_base) {
	case USDHC1_BASE_ADDR:
		ret = 1;
		break;
	case USDHC2_BASE_ADDR:
		ret = !gpio_get_value(USDHC2_CD_GPIO);
		return ret;
	}

	return 1;
}

#define USDHC_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_HYS | PAD_CTL_PUE | \
			 PAD_CTL_FSEL2)
#define USDHC_GPIO_PAD_CTRL (PAD_CTL_PUE | PAD_CTL_DSE1)

static iomux_v3_cfg_t const usdhc1_pads[] = {
	IMX8MQ_PAD_SD1_CLK__USDHC1_CLK | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	IMX8MQ_PAD_SD1_CMD__USDHC1_CMD | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	IMX8MQ_PAD_SD1_DATA0__USDHC1_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	IMX8MQ_PAD_SD1_DATA1__USDHC1_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	IMX8MQ_PAD_SD1_DATA2__USDHC1_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	IMX8MQ_PAD_SD1_DATA3__USDHC1_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	IMX8MQ_PAD_SD1_DATA4__USDHC1_DATA4 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	IMX8MQ_PAD_SD1_DATA5__USDHC1_DATA5 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	IMX8MQ_PAD_SD1_DATA6__USDHC1_DATA6 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	IMX8MQ_PAD_SD1_DATA7__USDHC1_DATA7 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	IMX8MQ_PAD_SD1_RESET_B__GPIO2_IO10 | MUX_PAD_CTRL(USDHC_GPIO_PAD_CTRL),
};

static iomux_v3_cfg_t const usdhc2_pads[] = {
	IMX8MQ_PAD_SD2_CLK__USDHC2_CLK | MUX_PAD_CTRL(USDHC_PAD_CTRL), /* 0xd6 */
	IMX8MQ_PAD_SD2_CMD__USDHC2_CMD | MUX_PAD_CTRL(USDHC_PAD_CTRL), /* 0xd6 */
	IMX8MQ_PAD_SD2_DATA0__USDHC2_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL), /* 0xd6 */
	IMX8MQ_PAD_SD2_DATA1__USDHC2_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL), /* 0xd6 */
	IMX8MQ_PAD_SD2_DATA2__USDHC2_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL), /* 0x16 */
	IMX8MQ_PAD_SD2_DATA3__USDHC2_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL), /* 0xd6 */
	IMX8MQ_PAD_GPIO1_IO04__USDHC2_VSELECT | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	IMX8MQ_PAD_SD2_CD_B__GPIO2_IO12 | MUX_PAD_CTRL(USDHC_GPIO_PAD_CTRL),
	IMX8MQ_PAD_SD2_RESET_B__GPIO2_IO19 | MUX_PAD_CTRL(USDHC_GPIO_PAD_CTRL),
};

static struct fsl_esdhc_cfg usdhc_cfg[2] = {
	{USDHC1_BASE_ADDR, 0, 8},
	{USDHC2_BASE_ADDR, 0, 4},
};

int board_mmc_init(bd_t *bis)
{
	int i, ret;
	/*
	 * According to the board_mmc_init() the following map is done:
	 * (U-Boot device node)    (Physical Port)
	 * mmc0                    USDHC1
	 * mmc1                    USDHC2
	 */
	for (i = 0; i < CONFIG_SYS_FSL_USDHC_NUM; i++) {
		switch (i) {
		case 0:
			usdhc_cfg[0].sdhc_clk = mxc_get_clock(USDHC1_CLK_ROOT);
			imx_iomux_v3_setup_multiple_pads(
				usdhc1_pads, ARRAY_SIZE(usdhc1_pads));
			gpio_request(USDHC1_PWR_GPIO, "usdhc1_reset");
			gpio_direction_output(USDHC1_PWR_GPIO, 0);
			udelay(500);
			gpio_direction_output(USDHC1_PWR_GPIO, 1);
			break;
		case 1:
			usdhc_cfg[1].sdhc_clk = mxc_get_clock(USDHC2_CLK_ROOT);
			imx_iomux_v3_setup_multiple_pads(
				usdhc2_pads, ARRAY_SIZE(usdhc2_pads));
			gpio_request(USDHC2_PWR_GPIO, "usdhc2_reset");
			gpio_direction_output(USDHC2_PWR_GPIO, 0);
			udelay(500);
			gpio_direction_output(USDHC2_PWR_GPIO, 1);
			break;
		default:
			printf("Warning: you configured more USDHC controllers"
				"(%d) than supported by the board\n", i + 1);
			return -EINVAL;
		}

		ret = fsl_esdhc_initialize(bis, &usdhc_cfg[i]);
		if (ret)
			return ret;
	}

	return 0;
}

#ifdef CONFIG_POWER
#define PWR_EN IMX_GPIO_NR(1, 8)
static iomux_v3_cfg_t const pwr_en_pads[] = {
	IMX8MQ_PAD_GPIO1_IO08__GPIO1_IO8 | MUX_PAD_CTRL(PAD_CTL_DSE6 | PAD_CTL_FSEL1),
};

#define I2C_PMIC	0
int power_init_board(void)
{
	struct pmic *p;
	int ldo[] = {BD71837_REG_LDO5_VOLT, BD71837_REG_LDO6_VOLT,
		     BD71837_REG_LDO7_VOLT};
	u32 val;
	int i, rv;

	/*
	 * Init PMIC
	 */
	rv = power_bd71837_init(CONFIG_POWER_BD71837_I2C_BUS);
	if (rv) {
		printf("%s: power_bd71837_init(%d) error %d\n", __func__,
			CONFIG_POWER_BD71837_I2C_BUS, rv);
		goto out;
	}

	p = pmic_get(BD71837_NAME);
	if (!p) {
		printf("%s: pmic_get(%s) failed\n", __func__, BD71837_NAME);
		rv = -ENODEV;
		goto out;
	}

	rv = pmic_probe(p);
	if (rv) {
		printf("%s: pmic_probe() error %d\n", __func__, rv);
		goto out;
	}

	/*
	 * Unlock all regs
	 */
	pmic_reg_write(p, BD71837_REG_REGLOCK, 0);

	/*
	 * Reconfigure default voltages:
	 * - BUCK3: VDD_GPU_0V9 (1.00 -> 0.90)
	 * - BUCK4: VDD_VPU_0V9 (1.00 -> 0.90)
	 */
	pmic_reg_write(p, BD71837_REG_BUCK3_VOLT_RUN, 0x14);
	pmic_reg_write(p, BD71837_REG_BUCK4_VOLT_RUN, 0x14);

#if 0 /* defined(CONFIG_TARGET_EMCRAFT_IMX8M_SOM) */
	/*
	 * - BUCK8: VDD_DRAM_1V35 (1.10 -> 1.35)
	 */
	pmic_reg_write(p, BD71837_REG_BUCK8_VOLT, 0x37);
#endif

	/*
	 * Enable PHYs voltages: LDO5-7
	 */
	for (i = 0; i < ARRAY_SIZE(ldo); i++) {
		rv = pmic_reg_read(p, ldo[i], &val);
		if (rv) {
			printf("%s: pmic_read(%x) error %d\n", __func__,
				ldo[i], rv);
			continue;
		}

		pmic_reg_write(p, ldo[i], val | LDO_VOLT_EN);
	}

	imx_iomux_v3_setup_multiple_pads(pwr_en_pads, ARRAY_SIZE(pwr_en_pads));

	gpio_request(PWR_EN, "pwr_en");
	gpio_direction_output(PWR_EN, 1);
	udelay(500);

	rv = 0;
out:
	return rv;
}
#endif

void spl_board_init(void)
{
	/* Serial download mode */
	if (is_usb_boot()) {
		puts("Back to ROM, SDP\n");
		restore_boot_params();
	}
	puts("Normal Boot\n");
}

#ifdef CONFIG_SPL_LOAD_FIT
int board_fit_config_name_match(const char *name)
{
	/* Just empty function now - can't decide what to choose */
	debug("%s: %s\n", __func__, name);

	return 0;
}
#endif

void board_init_f(ulong dummy)
{
	int ret;

	/* Clear global data */
	memset((void *)gd, 0, sizeof(gd_t));

	arch_cpu_init();

	board_early_init_f();

	timer_init();

	preloader_console_init();

	/* Clear the BSS. */
	memset(__bss_start, 0, __bss_end - __bss_start);

	ret = spl_init();
	if (ret) {
		debug("spl_init() failed: %d\n", ret);
		hang();
	}

	enable_tzc380();

	/* Adjust pmic voltage VDD_DRAM to 1.0V for DRAM RUN >= 2400MHZ */
	setup_i2c(0, CONFIG_SYS_I2C_SPEED, 0x7f, &i2c_pad_info1);

	power_init_board();

	/* DDR initialization */
	spl_dram_init();

	board_init_r(NULL, 0);
}
