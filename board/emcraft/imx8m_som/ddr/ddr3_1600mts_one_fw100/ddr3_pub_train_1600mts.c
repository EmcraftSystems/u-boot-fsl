#include <common.h>
#include <asm/io.h>
#include <asm/arch/ddr_memory_map.h>
#include <asm/arch/clock.h>

#define DDR_ONE_RANK
#define RUN_ON_SILICON 

#define reg32_write(a,v) writel(v,a)
#define reg32_read(v) readl(v)

#define  reg32setbit(addr,bitpos) \
    reg32_write((addr),(reg32_read((addr)) | (1<<(bitpos))))
#define  reg32clrbit(addr,bitpos) \
    reg32_write((addr),(reg32_read((addr)) & (~(1<<(bitpos)))))

#define  reg16setbit(addr,bitpos) \
    reg16_write((addr),(reg16_read((addr)) | (1<<(bitpos))))
#define  reg8setbit(addr,bitpos) \
    reg8_write((addr),(reg16_read((addr)) | (1<<(bitpos))))
typedef volatile unsigned int WORD;
#if 0
//#include "ddr3_phyinit_train_1600mts.c"

void DDR_PLL_CONFIG_600MHz(void) {
WORD ddr_pll_lock;
sscgpll_bypass_enable(HW_DRAM_PLL_CFG0_ADDR);
//000 011111 100111 010000 000001 0
//reg32_write(HW_DRAM_PLL_CFG2_ADDR, 0x00fce802);
//000 011101 100011 001001 000000 0
reg32_write(HW_DRAM_PLL_CFG2_ADDR, 0x00EC6480);
sscgpll_bypass_disable(HW_DRAM_PLL_CFG0_ADDR);
while ( ddr_pll_lock != 0x1 ) {
   ddr_pll_lock = wait_pll_lock(HW_DRAM_PLL_CFG0_ADDR);
  }
printk("Congratulations, DDR PLL1 600MHz locked success!\n");
}

//PLLOUT = REF/DIVR1*2*DIVF1/DIVR2*DIVF2*2/2/DIVQ
void DDR_PLL_CONFIG_400MHz(void) {
WORD ddr_pll_lock;
sscgpll_bypass_enable(HW_DRAM_PLL_CFG0_ADDR);
//000 011111 100111 010000 000001 0
//reg32_write(HW_DRAM_PLL_CFG2_ADDR, 0x00fce802);
//000 011101 100011 001001 000000 0
//reg32_write(HW_DRAM_PLL_CFG2_ADDR, 0x00EC6480);
reg32setbit(HW_DRAM_PLL_CFG2_ADDR,1);//default is 800MHz
sscgpll_bypass_disable(HW_DRAM_PLL_CFG0_ADDR);
while ( ddr_pll_lock != 0x1 ) {
   ddr_pll_lock = wait_pll_lock(HW_DRAM_PLL_CFG0_ADDR);
  }
printk("Congratulations, DDR PLL1 400MHz locked success!\n");
}

void DDR_PLL_CONFIG_666MHz(void) {
WORD ddr_pll_lock;
sscgpll_bypass_enable(HW_DRAM_PLL_CFG0_ADDR);
//000 011111 100111 010000 000001 0
//reg32_write(HW_DRAM_PLL_CFG2_ADDR, 0x00fce802);
//000 011101 100011 001001 000000 0
reg32_write(HW_DRAM_PLL_CFG2_ADDR, 0x00ECEE84);
sscgpll_bypass_disable(HW_DRAM_PLL_CFG0_ADDR);
while ( ddr_pll_lock != 0x1 ) {
   ddr_pll_lock = wait_pll_lock(HW_DRAM_PLL_CFG0_ADDR);
  }
printk("Congratulations, DDR PLL1 locked success!\n");
}
#endif

void ddr3_pub_train_1600mts()
{
    volatile unsigned int tmp, tmp_t, i;
    volatile unsigned int train_ok;
    volatile unsigned int train_fail;
    volatile unsigned int stream_msg;
    WORD tmp_data;

    //change the clock source of dram_apb_clk_root 
    reg32_write(CCM_IP_CLK_ROOT_GEN_TAGET_CLR(1),(0x7<<24)|(0x7<<16));   
    reg32_write(CCM_IP_CLK_ROOT_GEN_TAGET_SET(1),(0x4<<24)|(0x3<<16)); //to source 4 --800MHz/4


  //disable the phy iso 
  reg32_write(0x303A00EC,0x0000ffff); //PGC_CPU_MAPPING
  reg32setbit(0x303A00F8,5);//PU_PGC_SW_PUP_REQ

    reg32_write(SRC_DDRC_RCR_ADDR + 0x04, 0x8F000000); // release [0]ddr2_preset_n, [1]ddr2_core_reset_n, [2]ddr2_phy_reset, [3]ddr2_phy_pwrokin_n

config_dram_pll_400M();
//    DDR_PLL_CONFIG_400MHz();

    reg32_write(SRC_DDRC_RCR_ADDR, 0x8F000006); // release [0]ddr1_preset_n, [1]ddr1_core_reset_n, [2]ddr1_phy_reset, [3]ddr1_phy_pwrokin_n
// RESET: <core_ddrc_rstn> ASSERTED (ACTIVE LOW)
// RESET: <presetn> ASSERTED (ACTIVE LOW)
// RESET: <aresetn> for Port 0  ASSERTED (ACTIVE LOW)
// RESET: <presetn> DEASSERTED

umctl2_cfg();

reg32setbit(DDRC_RFSHCTL3(0),0);//dis_auto_refresh
//RESET: <ctn> DEASSERTED
//RESET: <a Port 0  DEASSERTED(0)
reg32_write(SRC_DDRC_RCR_ADDR, 0x8F000000); // release all reset 
ddr4_load_train_code();
 reg32_write(DDRC_DBG1(0), 0x00000000); // ('b00000000_00000000_00000000_00000000) ('d0)
 reg32setbit(DDRC_PWRCTL(0),5);//selfref_sw=1, self-refresh
 reg32clrbit(DDRC_SWCTL(0), 0);//sw_done=0, enable quasi-dynamic programming
 reg32_write(DDRC_DFIMISC(0), 0x00000000); 


ddr3_phyinit_train_1600mts(0);//before retention


do {
  tmp_data = reg32_read(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0x00020097); 
  // printf("C: Wait CalBusy = 0\n");
}while(tmp_data != 0);

reg32setbit(DDRC_DFIMISC(0),5);//dfi_init_start=1
do{
  tmp=0x1 & (reg32_read(DDRC_DFISTAT(0)));
}while(!tmp);// wait DFISTAT.dfi_init_complete to 1

reg32clrbit(DDRC_DFIMISC(0),5);//dfi_init_start=0
reg32setbit(DDRC_DFIMISC(0),0);//dfi_init_complete_en=1

reg32clrbit(DDRC_PWRCTL(0),5);//selfref_sw=0, exit self-refresh

reg32setbit(DDRC_SWCTL(0), 0);//sw_done=1, disable quasi-dynamic programming

//wait SWSTAT.sw_done_ack to 1
do{
  tmp=0x1 & (reg32_read(DDRC_SWSTAT(0)));
}while(!tmp); 

//wait STAT to normal state
do{
  tmp=0x7 & (reg32_read(DDRC_STAT(0)));
}while(tmp != 0x1); 

reg32_write(DDRC_PCTRL_0(0), 0x00000001); //enable port 0

reg32clrbit(DDRC_RFSHCTL3(0), 0); //  auto-refresh enable

}


void umctl2_cfg(){

  WORD tmp;

reg32_write(DDRC_DBG1(0), 0x00000001); 
reg32_write(DDRC_PWRCTL(0), 0x00000001); 
do{
  tmp = 0x7 & (reg32_read(DDRC_STAT(0)));
}while(tmp);// wait init state

#ifdef DDR_ONE_RANK
reg32_write(DDRC_MSTR(0), 0x81040001);//one rank
#else
reg32_write(DDRC_MSTR(0), 0x83040001);//two rank
#endif

reg32_write(DDRC_MRCTRL0(0), 0x40004030); 
reg32_write(DDRC_MRCTRL1(0), 0x0001c68e); 
reg32_write(DDRC_MRCTRL2(0), 0x921b7e95); 
reg32_write(DDRC_DERATEEN(0), 0x00000506); 
reg32_write(DDRC_DERATEINT(0), 0x9a4fbdf1); 
reg32_write(DDRC_MSTR2(0), 0x00000001); 
reg32_write(DDRC_PWRCTL(0), 0x000000a8); 
reg32_write(DDRC_PWRTMG(0), 0x00532203); 
reg32_write(DDRC_HWLPCTL(0), 0x0b6d0000); 
reg32_write(DDRC_HWFFCCTL(0), 0x00000030); 
reg32_write(DDRC_RFSHCTL0(0), 0x00203020); 
reg32_write(DDRC_RFSHCTL1(0), 0x0001000d); 
reg32_write(DDRC_RFSHCTL3(0), 0x00000000); 
reg32_write(DDRC_RFSHTMG(0), 0x0061008c); 
reg32_write(DDRC_CRCPARCTL0(0), 0x00000000); 
reg32_write(DDRC_CRCPARCTL1(0), 0x00000000); 
reg32_write(DDRC_INIT0(0), 0xc0030002); 
reg32_write(DDRC_INIT1(0), 0x0001000b); 
reg32_write(DDRC_INIT2(0), 0x00006303); 
reg32_write(DDRC_INIT3(0), 0x0d700044);//MR1, MR0 
reg32_write(DDRC_INIT4(0), 0x00180000);//MR2 
reg32_write(DDRC_INIT5(0), 0x00090071); 
reg32_write(DDRC_INIT6(0), 0x00000000); 
reg32_write(DDRC_INIT7(0), 0x00000000); 
reg32_write(DDRC_DIMMCTL(0), 0x00000030); 
reg32_write(DDRC_RANKCTL(0), 0x00000ee5); 
reg32_write(DDRC_DRAMTMG0(0), 0x0c101a0e); 
reg32_write(DDRC_DRAMTMG1(0), 0x000a0314); 
reg32_write(DDRC_DRAMTMG2(0), 0x04060509); 
reg32_write(DDRC_DRAMTMG3(0), 0x00002006); 
reg32_write(DDRC_DRAMTMG4(0), 0x06020306); 
reg32_write(DDRC_DRAMTMG5(0), 0x0b060202); 
reg32_write(DDRC_DRAMTMG6(0), 0x060a0009); 
reg32_write(DDRC_DRAMTMG7(0), 0x0000060b); 
reg32_write(DDRC_DRAMTMG8(0), 0x01017c0a); 
reg32_write(DDRC_DRAMTMG9(0), 0x4000000e); 
reg32_write(DDRC_DRAMTMG10(0), 0x00070803); 
reg32_write(DDRC_DRAMTMG11(0), 0x0101000b); 
reg32_write(DDRC_DRAMTMG12(0), 0x00000000); 
reg32_write(DDRC_DRAMTMG13(0), 0x5d000000); 
reg32_write(DDRC_DRAMTMG14(0), 0x00000b39); 
reg32_write(DDRC_DRAMTMG15(0), 0x80000000); 
reg32_write(DDRC_DRAMTMG17(0), 0x00f1006a); 
reg32_write(DDRC_ZQCTL0(0), 0x50800020); 
reg32_write(DDRC_ZQCTL1(0), 0x00000070); 
reg32_write(DDRC_ZQCTL2(0), 0x00000000); 
reg32_write(DDRC_DFITMG0(0), 0x03868203); 
reg32_write(DDRC_DFITMG1(0), 0x00020103); 
reg32_write(DDRC_DFILPCFG0(0), 0x07713121); 
reg32_write(DDRC_DFILPCFG1(0), 0x00000010); 
reg32_write(DDRC_DFIUPD0(0), 0xe0400018); 
reg32_write(DDRC_DFIUPD1(0), 0x0005003c); 
reg32_write(DDRC_DFIUPD2(0), 0x00000000); 
reg32_write(DDRC_DFIMISC(0), 0x00000011); 
reg32_write(DDRC_DFITMG2(0), 0x00000603); 
reg32_write(DDRC_DFITMG3(0), 0x00000001); 
reg32_write(DDRC_DBICTL(0), 0x00000001); 
reg32_write(DDRC_DFIPHYMSTR(0), 0x00000000); 

    // My test mapping in this test case, for 8Gb,(two 4Gb, x16 DDR3) (col addr:10 bits  row addr: 15 bits  bank addr: 3bits  2 ranks)
    // MEMC_BURST_LENGTH = 8
    //-----------------------------------------------------------------------------------------------------------------------------------     
    // AXI add: 31  30  29  28  27  26  25  24  23  22  21  20  19  18  17  16  15  14  13   12  11  10  9   8   7   6   5   4   3   2   1   0 (MEM_DATWIDTH=64)
    // AXI add: 30  29  28  27  26  25  24  23  22  21  20  19  18  17  16  15  14  13  12   11  10  9   8   7   6   5   4   3   2   1   0     (MEM_DATWIDTH=32) ***
    // AXI add: 29  28  27  26  25  24  23  22  21  20  19  18  17  16  15  14  13  12  11   10  9   8   7   6   5   4   3   2   1   0         (MEM_DATWIDTH=16)
    //-----------------------------------------------------------------------------------------------------------------------------------
    // HIF add: 28  27  26  25  24  23  22  21  20  19  18  17  16  15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0   -   -   -
    //-----------------------------------------------------------------------------------------------------------------------------------    
    // **** for Full DQ bus width (X32) ****
    //          cs  r14 r13 r12 r11 r10 r9  r8  r7  r6  r5  r4  r3  r2  r1  r0  b2  b1  b0  c9  c8  c7  c6  c5  c4  c3  c2  c1  c0  
    //Int base  6   20  19  18  17  16  15  14  13  12  11  10  9   8   7   6   4   3   2    9   8  7   6   5    4   3   2   -       
    //p Value   22  7   7   7   7   7   7   7   7   7    7   7  7   7   7   7   8   8   8    0   0  0   0   0    0   0   0   -
    //-----------------------------------------------------------------------------------------------------------------------------------

    reg32_write(DDRC_ADDRMAP0(0), 0x00000016); //[4:0] cs-bit0: 6+22=28; [12:8] cs-bit1: 7+0
    reg32_write(DDRC_ADDRMAP1(0), 0x00080808); //[5:0] bank b0: 2+8; [13:8] b1: P3+8 ; [21:16] b2: 4+8 
    reg32_write(DDRC_ADDRMAP2(0), 0x00000000); //[3:0] col-b2: 2;  [11:8] col-b3: 3; [19:16] col-b4: 4 ; [27:24] col-b5: 5
    reg32_write(DDRC_ADDRMAP3(0), 0x00000000); //[3:0] col-b6: 6;  [11:8] col-b7: 7; [19:16] col-b8: 8 ; [27:24] col-b9: 9
    reg32_write(DDRC_ADDRMAP4(0), 0x00001f1f); //col-b10, col-b11 not used
    reg32_write(DDRC_ADDRMAP5(0), 0x07070707); //[3:0] row-b0: 6;  [11:8] row-b1: 7; [19:16] row-b2_b10 ; [27:24] row-b11: 17
    reg32_write(DDRC_ADDRMAP6(0), 0x0f070707); //[3:0] row-b12:18; [11:8] row-b13: 19; [19:16] row-b14:20
    reg32_write(DDRC_ADDRMAP7(0), 0x00000f0f);
    reg32_write(DDRC_ADDRMAP8(0), 0x00000000); //[5:0] bg-b0; [13:8]bg-b1
    reg32_write(DDRC_ADDRMAP9(0), 0x0a020b06); // it's valid only when ADDRMAP5.addrmap_row_b2_10 is set to value 15
    reg32_write(DDRC_ADDRMAP10(0), 0x0a0a0a0a);// it's valid only when ADDRMAP5.addrmap_row_b2_10 is set to value 15
    reg32_write(DDRC_ADDRMAP11(0), 0x00000000);


reg32_write(DDRC_ODTCFG(0), 0x041d0f5c); 
reg32_write(DDRC_ODTMAP(0), 0x00003012); 
reg32_write(DDRC_SCHED(0), 0x7ab50b07); 
reg32_write(DDRC_SCHED1(0), 0x00000022); 
reg32_write(DDRC_PERFHPR1(0), 0x7b00665e); 
reg32_write(DDRC_PERFLPR1(0), 0x2b00c4e1); 
reg32_write(DDRC_PERFWR1(0), 0xb700c9fe); 
reg32_write(DDRC_DBG0(0), 0x00000017); 
reg32_write(DDRC_DBG1(0), 0x00000000); 
reg32_write(DDRC_DBGCMD(0), 0x00000000); 
reg32_write(DDRC_SWCTL(0), 0x00000001); 
reg32_write(DDRC_POISONCFG(0), 0x00010000); 
reg32_write(DDRC_PCCFG(0), 0x00000100); 
reg32_write(DDRC_PCFGR_0(0), 0x00003051); 
reg32_write(DDRC_PCFGW_0(0), 0x000061d2); 
reg32_write(DDRC_PCTRL_0(0), 0x00000001); 
reg32_write(DDRC_PCFGQOS0_0(0), 0x02100b04); 
reg32_write(DDRC_PCFGQOS1_0(0), 0x003f0353); 
reg32_write(DDRC_PCFGWQOS0_0(0), 0x00000002); 
reg32_write(DDRC_PCFGWQOS1_0(0), 0x000005fd);



}



