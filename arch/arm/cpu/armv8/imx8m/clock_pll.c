// * ===============================================================================
// * File         : clock_pll.c
// * Author       : Wang Hairun
// * Created Date : 06/22/2017 | 10:29:19 AM | Thursday,June
// * Description  : 
// * ===============================================================================
#include <common.h>
#include <asm/arch/clock_pll.h>
#include <asm/io.h>
/*
Formula for SSCG PLLOUT
(When SSE = 0) PLLOUT = REF/DIVR1*2*DIVF1/DIVR2*DIVF2*2/2/DIVQ
(When SSE = 1) PLLOUT = REF/DIVR1*8*DIVF1/DIVR2*DIVF2*2/2/DIVQ
Programming Example:
 SYSTEM PLL1, SSE = 0, Target Frequency 800MHz.
REF = 25MHz (10 MHz ~ 300 MHz) (25MHz ~ 235MHz)
PLL_REF_DIVR1 = 3’b000,
DIVR1 = 1, REF/DIVR1 = 25MHz, (25MHz ~ 54MHz)
PLL_FEEDBACK_DIVF1 = 6’b011111,
DIVF1 =32, REF/DIVR1*2*DIVF1 = 1600MHz, (1600MHz ~ 2400 MHz)
PLL_REF_DIVR2 = 6’b010111,
DIVR2 = 24, REF/DIVR1*2*DIVF1/DIVR2 = 66.66666667MHz, (54MHz ~ 75MHz)
PLL_FEEDBACK_DIVF2 = 6’b001011,
DIVF2 = 12, REF/DIVR1*2*DIVF1/DIVR2*DIVF2*2 = 1600MHz, (1200MHz ~ 2400MHz)
PLL_OUTPUT_DIV_VAL = 6’b0
DIVQ = 1, PLLOUT = REF/DIVR1*2*DIVF1/DIVR2*DIVF2*2/2/DIVQ = 800MHz (20MHz ~ 1200MHz)
*/

void config_dram_pll_400M(void)
{
    hw_dram_pll_cfg0    hw_dram_pll_cfg0_;    
    hw_dram_pll_cfg1    hw_dram_pll_cfg1_;
    hw_dram_pll_cfg2    hw_dram_pll_cfg2_;

    hw_dram_pll_cfg0_.all = readl(HW_DRAM_PLL_CFG0);
    hw_dram_pll_cfg0_.bit.pll_bypass1 = 1;
    hw_dram_pll_cfg0_.bit.pll_bypass2 = 1;
    writel(hw_dram_pll_cfg0_.all,HW_DRAM_PLL_CFG0);

    hw_dram_pll_cfg2_.all = readl(HW_DRAM_PLL_CFG2);
    hw_dram_pll_cfg2_.bit.pll_output_div_val = 1;
    writel(hw_dram_pll_cfg2_.all,HW_DRAM_PLL_CFG2);

    hw_dram_pll_cfg0_.all = readl(HW_DRAM_PLL_CFG0);
    hw_dram_pll_cfg0_.bit.pll_bypass1 = 0;
    hw_dram_pll_cfg0_.bit.pll_bypass2 = 0;
    writel(hw_dram_pll_cfg0_.all,HW_DRAM_PLL_CFG0);


    while(hw_dram_pll_cfg0_.bit.pll_lock != 1)
    {
        hw_dram_pll_cfg0_.all = readl(HW_DRAM_PLL_CFG0);
    }
    // pll_monitor_cfg(14);
}

void config_dram_pll_800M(void)
{
    hw_dram_pll_cfg0    hw_dram_pll_cfg0_;    
    hw_dram_pll_cfg1    hw_dram_pll_cfg1_;
    hw_dram_pll_cfg2    hw_dram_pll_cfg2_;

    hw_dram_pll_cfg0_.all = readl(HW_DRAM_PLL_CFG0);
    hw_dram_pll_cfg0_.bit.pll_bypass1 = 1;
    hw_dram_pll_cfg0_.bit.pll_bypass2 = 1;
    writel(hw_dram_pll_cfg0_.all,HW_DRAM_PLL_CFG0);

    hw_dram_pll_cfg2_.all = readl(HW_DRAM_PLL_CFG2);
    hw_dram_pll_cfg2_.bit.pll_output_div_val = 0;
    writel(hw_dram_pll_cfg2_.all,HW_DRAM_PLL_CFG2);

    hw_dram_pll_cfg0_.all = readl(HW_DRAM_PLL_CFG0);
    hw_dram_pll_cfg0_.bit.pll_bypass1 = 0;
    hw_dram_pll_cfg0_.bit.pll_bypass2 = 0;
    writel(hw_dram_pll_cfg0_.all,HW_DRAM_PLL_CFG0);


    while(hw_dram_pll_cfg0_.bit.pll_lock != 1)
    {
        hw_dram_pll_cfg0_.all = readl(HW_DRAM_PLL_CFG0);
    }
    // pll_monitor_cfg(14);
}

void config_dram_pll_600M(void)
{
    hw_dram_pll_cfg0    hw_dram_pll_cfg0_;    
    hw_dram_pll_cfg1    hw_dram_pll_cfg1_;
    hw_dram_pll_cfg2    hw_dram_pll_cfg2_;

    hw_dram_pll_cfg0_.all = readl(HW_DRAM_PLL_CFG0);
    hw_dram_pll_cfg0_.bit.pll_bypass1 = 1;
    hw_dram_pll_cfg0_.bit.pll_bypass2 = 1;
    writel(hw_dram_pll_cfg0_.all,HW_DRAM_PLL_CFG0);

    hw_dram_pll_cfg2_.all = readl(HW_DRAM_PLL_CFG2);
    hw_dram_pll_cfg2_.bit.pll_output_div_val = 1;
    hw_dram_pll_cfg2_.bit.pll_feedback_divf2 = 17;
    writel(hw_dram_pll_cfg2_.all,HW_DRAM_PLL_CFG2);

    hw_dram_pll_cfg0_.all = readl(HW_DRAM_PLL_CFG0);
    hw_dram_pll_cfg0_.bit.pll_bypass1 = 0;
    hw_dram_pll_cfg0_.bit.pll_bypass2 = 0;
    writel(hw_dram_pll_cfg0_.all,HW_DRAM_PLL_CFG0);


    while(hw_dram_pll_cfg0_.bit.pll_lock != 1)
    {
        hw_dram_pll_cfg0_.all = readl(HW_DRAM_PLL_CFG0);
    }
    printf("\ndram pll 600M locked\n");
    // pll_monitor_cfg(14);
}

void config_arm_pll_1400M(void)
{
    hw_arm_pll_cfg0    hw_arm_pll_cfg0_;    
    hw_arm_pll_cfg0_.all = readl(HW_ARM_PLL_CFG0);
    hw_arm_pll_cfg0_.bit.pll_bypass = 1;
    writel(hw_arm_pll_cfg0_.all,HW_ARM_PLL_CFG0);

    hw_arm_pll_cfg0_.all = readl(HW_ARM_PLL_CFG0);
    hw_arm_pll_cfg0_.bit.pll_bypass = 0;
    writel(hw_arm_pll_cfg0_.all,HW_ARM_PLL_CFG0);

    writel(0x1000000,0x30389880);
}

void turn_off_dram_pll_600M(void)
{
    hw_dram_pll_cfg0    hw_dram_pll_cfg0_;    

    hw_dram_pll_cfg0_.all = readl(HW_DRAM_PLL_CFG0);
    hw_dram_pll_cfg0_.bit.pll_bypass1 = 0;
    hw_dram_pll_cfg0_.bit.pll_bypass2 = 0;
    hw_dram_pll_cfg0_.bit.pll_clke = 0;
    hw_dram_pll_cfg0_.bit.pll_pd = 1;
    writel(hw_dram_pll_cfg0_.all,HW_DRAM_PLL_CFG0);
}

void turn_on_dram_pll_600M(void)
{
    hw_dram_pll_cfg0    hw_dram_pll_cfg0_;    
    hw_dram_pll_cfg1    hw_dram_pll_cfg1_;
    hw_dram_pll_cfg2    hw_dram_pll_cfg2_;

    hw_dram_pll_cfg0_.all = readl(HW_DRAM_PLL_CFG0);
    hw_dram_pll_cfg0_.bit.pll_bypass1 = 1;
    hw_dram_pll_cfg0_.bit.pll_bypass2 = 1;
    writel(hw_dram_pll_cfg0_.all,HW_DRAM_PLL_CFG0);

    hw_dram_pll_cfg2_.all = readl(HW_DRAM_PLL_CFG2);
    hw_dram_pll_cfg2_.bit.pll_output_div_val = 1;
    hw_dram_pll_cfg2_.bit.pll_feedback_divf2 = 17;
    writel(hw_dram_pll_cfg2_.all,HW_DRAM_PLL_CFG2);

    hw_dram_pll_cfg0_.all = readl(HW_DRAM_PLL_CFG0);
    hw_dram_pll_cfg0_.bit.pll_bypass1 = 0;
    hw_dram_pll_cfg0_.bit.pll_bypass2 = 0;
    hw_dram_pll_cfg0_.bit.pll_clke = 1;
    hw_dram_pll_cfg0_.bit.pll_pd = 0;
    writel(hw_dram_pll_cfg0_.all,HW_DRAM_PLL_CFG0);

    while(hw_dram_pll_cfg0_.bit.pll_lock != 1)
    {
        hw_dram_pll_cfg0_.all = readl(HW_DRAM_PLL_CFG0);
    }
}

