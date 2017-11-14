// * ===============================================================================
// * File         : clock_pll.h
// * Author       : Wang Hairun
// * Created Date : 06/22/2017 | 10:31:32 AM | Thursday,June
// * Description  : 
// * ===============================================================================
#ifndef __CLOCK_PLL_H
#define __CLOCK_PLL_H

#define HW_AUDIO_PLL1_CFG0           ((volatile hw_audio_pll1_cfg0       *) (0x30360000))   
#define HW_AUDIO_PLL1_CFG1           ((volatile hw_audio_pll1_cfg1       *) (0x30360004))   
#define HW_AUDIO_PLL2_CFG0           ((volatile hw_audio_pll2_cfg0       *) (0x30360008))   
#define HW_AUDIO_PLL2_CFG1           ((volatile hw_audio_pll2_cfg1       *) (0x3036000C))   
#define HW_VIDEO_PLL_CFG0            ((volatile hw_video_pll_cfg0        *) (0x30360010))   
#define HW_VIDEO_PLL_CFG1            ((volatile hw_video_pll_cfg1        *) (0x30360014))   
#define HW_GPU_PLL_CFG0              ((volatile hw_gpu_pll_cfg0          *) (0x30360018))   
#define HW_GPU_PLL_CFG1              ((volatile hw_gpu_pll_cfg1          *) (0x3036001C))   
#define HW_VPU_PLL_CFG0              ((volatile hw_vpu_pll_cfg0          *) (0x30360020))   
#define HW_VPU_PLL_CFG1              ((volatile hw_vpu_pll_cfg1          *) (0x30360024))   
#define HW_ARM_PLL_CFG0              ((volatile hw_arm_pll_cfg0          *) (0x30360028))   
#define HW_ARM_PLL_CFG1              ((volatile hw_arm_pll_cfg1          *) (0x3036002C))   
#define HW_SYS_PLL1_CFG0             ((volatile hw_sys_pll1_cfg0         *) (0x30360030))   
#define HW_SYS_PLL1_CFG1             ((volatile hw_sys_pll1_cfg1         *) (0x30360034))   
#define HW_SYS_PLL1_CFG2             ((volatile hw_sys_pll1_cfg2         *) (0x30360038))   
#define HW_SYS_PLL2_CFG0             ((volatile hw_sys_pll2_cfg0         *) (0x3036003C))   
#define HW_SYS_PLL2_CFG1             ((volatile hw_sys_pll2_cfg1         *) (0x30360040))   
#define HW_SYS_PLL2_CFG2             ((volatile hw_sys_pll2_cfg2         *) (0x30360044))   
#define HW_SYS_PLL3_CFG0             ((volatile hw_sys_pll3_cfg0         *) (0x30360048))   
#define HW_SYS_PLL3_CFG1             ((volatile hw_sys_pll3_cfg1         *) (0x3036004C))   
#define HW_SYS_PLL3_CFG2             ((volatile hw_sys_pll3_cfg2         *) (0x30360050))   
#define HW_VIDEO_PLL2_CFG0           ((volatile hw_video_pll2_cfg0       *) (0x30360054))   
#define HW_VIDEO_PLL2_CFG1           ((volatile hw_video_pll2_cfg1       *) (0x30360058))   
#define HW_VEDIO_PLL2_CFG2           ((volatile hw_vedio_pll2_cfg2       *) (0x3036005C))   
#define HW_DRAM_PLL_CFG0             ((volatile hw_dram_pll_cfg0         *) (0x30360060))   
#define HW_DRAM_PLL_CFG1             ((volatile hw_dram_pll_cfg1         *) (0x30360064))   
#define HW_DRAM_PLL_CFG2             ((volatile hw_dram_pll_cfg2         *) (0x30360068))   
#define HW_DIGPROG                   ((volatile hw_digprog               *) (0x3036006C))   
#define HW_ANAMIX_MISC_CFG           ((volatile hw_anamix_misc_cfg       *) (0x30360070))   
#define HW_PLLOUT_MONITOR_CFG        ((volatile hw_pllout_monitor_cfg    *) (0x30360074))   
#define HW_FRAC_PLLOUT_DIV_CFG       ((volatile hw_frac_pllout_div_cfg   *) (0x30360078))   
#define HW_SSCG_PLLOUT_DIV_CFG       ((volatile hw_sscg_pllout_div_cfg   *) (0x3036007C))   
#define HW_DIFFPAD_CTL_CFG           ((volatile hw_diffpad_ctl_cfg       *) (0x30360080))   


typedef enum {
    PLL_BYPASS,
    PLL_ENABLE,
}pll_ops_t;






typedef union
{
    u32 all;
    struct
    {
        unsigned pll_refclk_sel          : 2 ; 
        unsigned pll_countclk_sel        : 1 ; 
        unsigned pll_lock_sel            : 1 ; 
        unsigned pll_bypass2             : 1 ; 
        unsigned pll_bypass1             : 1 ; 
        unsigned pll_pd_override         : 1 ; 
        unsigned pll_pd                  : 1 ; 
        unsigned pll_clke_override       : 1 ; 
        unsigned pll_clke                : 1 ; 
        unsigned pll_bypass_clk_sel      : 1 ; 
        unsigned                         : 20; 
        unsigned pll_lock                : 1 ;    
    }bit;
}hw_dram_pll_cfg0;


typedef union
{
    u32 all;
    struct
    {
        unsigned pll_sse    : 1 ; 
        unsigned pll_ssmf   : 4 ; 
        unsigned pll_ssmd   : 3 ; 
        unsigned pll_ssds   : 1 ; 
        unsigned            : 23; 
    }bit;
}hw_dram_pll_cfg1;


typedef union
{
    u32 all;
    struct
    {
        unsigned pll_filter_range       : 1;
        unsigned pll_output_div_val     : 6;
        unsigned pll_feedback_divf2     : 6;
        unsigned pll_feedback_divf1     : 6;
        unsigned pll_ref_divr2          : 6;
        unsigned pll_ref_divr1          : 3;
        unsigned                        : 4; 
    }bit;
}hw_dram_pll_cfg2;


typedef union
{
    u32 all;
    struct
    {
        unsigned pll_output_div_val   : 5;
        unsigned pll_refclk_div_val   : 6;
        unsigned pll_newdiv_ack       : 1;
        unsigned pll_newdiv_val       : 1;
        unsigned pll_countclk_sel     : 1;
        unsigned pll_bypass           : 1;
        unsigned pll_lock_sel         : 1;
        unsigned pll_refclk_sel       : 2;
        unsigned pll_pd_override      : 1;
        unsigned pll_pd               : 1;
        unsigned pll_clke_override    : 1;
        unsigned pll_bypass_clk_sel   : 1;
        unsigned                      : 8;
        unsigned pll_lock             : 1;    
    }bit;
}hw_arm_pll_cfg0;







































#endif

