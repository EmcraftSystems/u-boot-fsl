#include <common.h>
#include <errno.h>
#include <asm/io.h>
#include <asm/arch/ddr_memory_map.h>
#include <asm/arch/clock.h>
#include "ddr3.h"

#define RUN_ON_SILICON

#define SAVE_DDRPHY_TRAIN_ADDR 0x00916000
#define SRC_DDRC_RCR_ADDR SRC_IPS_BASE_ADDR +0x1000
#define DDR_CSD1_BASE_ADDR 0x40000000
#define DDR_CSD2_BASE_ADDR 0x80000000
#define DPRINTF_L0 printf 
#define DPRINTF_L2 printf

#define GPC_PU_PWRHSK 0x303A01FC
#define dwc_ddrphy_apb_wr(addr, data)  reg32_write(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*(addr), data)
#define dwc_ddrphy_apb_rd(addr)        (reg32_read(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*(addr)))

//#define DDR_DEBUG

//#include "restore_1d2d_trained_csr_ddr3_p0.c"
//#include "save_1d2d_trained_csr_ddr3_p0.c"

//#include "dram_pll_cfg.c"
//#include "ddr3_phyinit_task.c"
//#include "ddr3_phyinit_train_1600mts_x16_ret.c"

void umctl2_cfg(){

  unsigned int  tmp;

reg32_write(DDRC_DBG1(0), 0x00000001); 
reg32_write(DDRC_PWRCTL(0), 0x00000001); 
do{
  tmp = 0x7 & (reg32_read(DDRC_STAT(0)));
}while(tmp);// wait init state
    //reg32_write(DDRC_MSTR(0),  0x83040001); // [0]ddr3, [x:24]=1--1 rank, [19:16]=4--BL-8, [29]frequency_mode=1
    reg32_write(DDRC_MSTR(0),  0x81041001); //kanzi x16: [0]ddr3, [x:24]=1--1 rank, [19:16]=4--BL-8, [29]frequency_mode=1, [13:12]data_bus_width=1 
 
//reg32_write(DDRC_MSTR(0), 0x83040001); 
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
reg32_write(DDRC_DIMMCTL(0), 0x00000032); //[1] dimm_addr_mirr_en, it will effect the MRS if use umctl2 to initi dram.
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
reg32_write(DDRC_DFIUPD2(0), 0x80000000); 
reg32_write(DDRC_DFIMISC(0), 0x00000011); 
reg32_write(DDRC_DFITMG2(0), 0x00000603); 
reg32_write(DDRC_DFITMG3(0), 0x00000001); 
reg32_write(DDRC_DBICTL(0), 0x00000001); 
reg32_write(DDRC_DFIPHYMSTR(0), 0x00000000); 

    //row15,bank3,clo10
    reg32_write(DDRC_ADDRMAP0(0), 0x00000015); //[4:0] cs-bit0: axi[27]
    reg32_write(DDRC_ADDRMAP1(0), 0x00070707);  
    reg32_write(DDRC_ADDRMAP2(0), 0x00000000); 
    reg32_write(DDRC_ADDRMAP3(0), 0x00000000); 
    reg32_write(DDRC_ADDRMAP4(0), 0x00001f1f);
    reg32_write(DDRC_ADDRMAP5(0), 0x06060606); 
    reg32_write(DDRC_ADDRMAP6(0), 0x0f060606); 
    reg32_write(DDRC_ADDRMAP7(0), 0x00000f0f);
    reg32_write(DDRC_ADDRMAP8(0), 0x00000000); 


reg32_write(DDRC_ODTCFG(0), 0x0600060c); 
reg32_write(DDRC_ODTMAP(0), 0x00000001); 
reg32_write(DDRC_DBG0(0), 0x00000017); 
reg32_write(DDRC_DBG1(0), 0x00000000); 
reg32_write(DDRC_DBGCMD(0), 0x00000000); 
reg32_write(DDRC_SWCTL(0), 0x00000001); 


}



//void TEST_NAME()
void ddr3_pub_train_1600mts_ret_16bit_1rank()
{
    volatile unsigned int tmp, tmp_t, i;
    //verilog_trigger(vt_event1);

    reg32_write(SRC_DDRC_RCR_ADDR + 0x04, 0x8F000003); 
    reg32_write(SRC_DDRC_RCR_ADDR, 0x8F00000F); 
    reg32_write(SRC_DDRC_RCR_ADDR + 0x04, 0x8F000000); 

    //change the clock source of dram_apb_clk_root 
    reg32_write(CCM_IP_CLK_ROOT_GEN_TAGET_CLR(1),(0x7<<24)|(0x7<<16));   
    reg32_write(CCM_IP_CLK_ROOT_GEN_TAGET_SET(1),(0x4<<24)|(0x3<<16)); //to source 4 --800MHz/4


  //disable the phy iso 
  reg32_write(0x303A00EC,0x0000ffff); //PGC_CPU_MAPPING
  reg32setbit(0x303A00F8,5);//PU_PGC_SW_PUP_REQ

    reg32_write(SRC_DDRC_RCR_ADDR + 0x04, 0x8F000000); 

    DDR_PLL_CONFIG_400MHz();

    reg32_write(SRC_DDRC_RCR_ADDR, 0x8F000006); 
// RESET: <core_ddrc_rstn> ASSERTED (ACTIVE LOW)
// RESET: <presetn> ASSERTED (ACTIVE LOW)
// RESET: <aresetn> for Port 0  ASSERTED (ACTIVE LOW)
// RESET: <presetn> DEASSERTED

umctl2_cfg();

reg32setbit(DDRC_RFSHCTL3(0),0);//dis_auto_refresh
//RESET: <ctn> DEASSERTED
//RESET: <a Port 0  DEASSERTED(0)
reg32_write(SRC_DDRC_RCR_ADDR, 0x8F000000); // release all reset 

 reg32_write(DDRC_DBG1(0), 0x00000000); // ('b00000000_00000000_00000000_00000000) ('d0)
 reg32setbit(DDRC_PWRCTL(0),5);//selfref_sw=1, self-refresh
 reg32clrbit(DDRC_SWCTL(0), 0);//sw_done=0, enable quasi-dynamic programming
 reg32_write(DDRC_DFIMISC(0), 0x00000000); 


    printf("vt_event5\n");
ddr3_phyinit_train_1600mts_x16_ret(0);//before retention


do {
  tmp = reg32_read(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0x00020097); 
  printf("C: Wait CalBusy = 0\n");
}while(tmp != 0);

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


#if 0
    for (i=32; i<40; i++){
         mem32_write(DDR_CSD1_BASE_ADDR+i*4, i);
    }

    for (i=32; i<40; i++){
         tmp=mem32_read(DDR_CSD1_BASE_ADDR+i*4);
        if(tmp!=i) {verilog_trigger(vt_fail);
          info_trigger(i,tmp,0);
        }
    }
    verilog_trigger(vt_pass);
    DPRINTF_L0("DDR memory only write test via memcpy\n");

    //while(1)
    {//only write DDR memory
    	for(i=0;i<0x1000;i++){//0x2000:1GB
    		memcpy((DDR_MEM_RANK0_START_ADDR+i*0x20000),0x007e0000,0x00020000);//target/source/len//rank0
    	}
		//DPRINTF_L2(".");
    }

	DPRINTF_L0("DDR memory read&write test via memcpy\n");
    while(1){//read/write DDR memory
    	DPRINTF_L2("*");
    	memcpy(DDR_MEM_RANK0_START_ADDR,(DDR_MEM_RANK0_START_ADDR+0x0),0x10000000);//target/source/len//512MB//rank0
    	DPRINTF_L0("test result=0x%lx\r\n",memcmp(DDR_MEM_RANK0_START_ADDR,(DDR_MEM_RANK0_START_ADDR+0x0),0x10000000));
    }
#endif
//mem_test(0);
}

