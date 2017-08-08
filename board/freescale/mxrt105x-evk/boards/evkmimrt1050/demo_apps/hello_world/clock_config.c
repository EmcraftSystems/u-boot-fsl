/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_common.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* ARM PLL configuration for RUN mode */
const clock_arm_pll_config_t armPllConfig = {
  .loopDivider = 100U};

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* System clock frequency. */
extern uint32_t SystemCoreClock;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void BOARD_BootClockGate(void)
{
    /* Disable all unused peripheral clock */
    CCM->CCGR0 = 0x00C0000FU;
    CCM->CCGR1 = 0x30000000U;
    CCM->CCGR2 = 0x003F0030U;
    CCM->CCGR3 = 0xC0000300U;
    CCM->CCGR4 = 0x0000FF3CU;
    CCM->CCGR5 = 0xF000330FU;
    CCM->CCGR6 = 0x00FC0300U;
}

void BOARD_BootClockRUN(void)
{
    uint32_t val;

    /* Boot ROM did initialize the XTAL, here we only sets external XTAL OSC freq */
//    CLOCK_SetXtalFreq(24000000U);
    //  CLOCK_SetRtcXtalFreq(32768U);

    // setup the CLO to CLKO1, CLKO1_DIV=6, sel to AHB_CLK
    CCM->CCOSR = 0xDB;

    // change the DCDC_LP to 1.25V
    val = DCDC->REG3;
    val &= ~0x1F;
    val |= 0x12; // set to 1.25V for 25mV each steps
    DCDC->REG3 = val;

    while (!(DCDC->REG0 & 0x80000000)); // wait for DCDC ok

    // Move PERIPH_CLK OSC24M
    CLOCK_SetMux(kCLOCK_PeriphMux, 0x1);
    //CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 0x1);
    // AHB Podf DIV1, IPG PODF DIV4
    CCM->CBCDR &= ~0x1C00;
    CCM->CBCDR |= 0x300; // prevent IPG clock > 144Mhz
    // ARM PODF DIV2
    CCM->CACRR = 0x1;

    CLOCK_InitArmPll(&armPllConfig); /* Configure ARM PLL to 1200M */
    CLOCK_SetMux(kCLOCK_PrePeriphMux, 0x3);    /* Now CPU runs again on ARM PLL at 600M (with divider 2) */
    // if we switch to OSC24M, need to back to PRE_PREIPH
    CLOCK_SetMux(kCLOCK_PeriphMux, 0x0);

    /* Disable unused clock */
    BOARD_BootClockGate();

    /* Power down all unused PLL */    
    CLOCK_DeinitAudioPll();
    CLOCK_DeinitVideoPll();
    CLOCK_DeinitEnetPll();
    CLOCK_DeinitUsb2Pll();

    /* Update core clock */
    SystemCoreClockUpdate();
    const clock_usb_pll_config_t g_ccmConfigUsbPll = {.loopDivider = 0U};
    CLOCK_InitUsb1Pll(&g_ccmConfigUsbPll);
    CLOCK_InitUsb1Pfd(kCLOCK_Pfd1, 17);
    

    CCM_ANALOG->PLL_SYS = CCM_ANALOG_PLL_SYS_ENABLE_MASK|CCM_ANALOG_PLL_SYS_DIV_SELECT_MASK;
    while(!(CCM_ANALOG->PLL_SYS & CCM_ANALOG_PLL_SYS_LOCK_MASK));
    
    //SEMC_ClockConfig();

    /* route clock to anaclk1/1b */
    /*  clock source selection
            00000 ARM_PLL ！ Arm PLL
            00001 SYS_PLL ！ System PLL
            00010 PFD4 ！ ref_pfd4_clk == pll2_pfd0_clk
            00011 PFD5 ！ ref_pfd5_clk == pll2_pfd1_clk
            00100 PFD6 ！ ref_pfd6_clk == pll2_pfd2_clk
            00101 PFD7 ！ ref_pfd7_clk == pll2_pfd3_clk
            00110 AUDIO_PLL ！ Audio PLL
            00111 VIDEO_PLL ！ Video PLL
            01001 ETHERNET_REF ！ ethernet ref clock (ENET_PLL)
            01100 USB1_PLL ！ USB1 PLL clock
            01101 USB2_PLL ！ USB2 PLL clock
            01110 PFD0 ！ ref_pfd0_clk == pll3_pfd0_clk
            01111 PFD1 ！ ref_pfd1_clk == pll3_pfd1_clk
            10000 PFD2 ！ ref_pfd2_clk == pll3_pfd2_clk
            10001 PFD3 ！ ref_pfd3_clk == pll3_pfd3_clk
            10010 XTAL ！ xtal (24M)
            10101 to 11111 ref_pfd7_clk == pll2_pfd3_clk
    */
    CCM_ANALOG->MISC1 &= ~CCM_ANALOG_MISC1_LVDSCLK1_IBEN_MASK;
    CCM_ANALOG->MISC1 &= ~CCM_ANALOG_MISC1_LVDS1_CLK_SEL_MASK;
    CCM_ANALOG->MISC1 |= CCM_ANALOG_MISC1_LVDS1_CLK_SEL(0x1);
    CCM_ANALOG->MISC1 |= CCM_ANALOG_MISC1_LVDSCLK1_OBEN_MASK;
    //enable clock gate
    //CLOCK_EnableClock(kCLOCK_Semc);
    //while(1);
}
