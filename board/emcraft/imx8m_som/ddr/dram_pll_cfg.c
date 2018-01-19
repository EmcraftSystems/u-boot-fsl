#include "config.h"
#ifdef DDR3_1600MHZ_RET_16BIT_1RANK_FW09

#include "soc_api.h"
#include "ddr_memory_map.h"
#define printf DPRINTF_L0

void DDR_PLL_CONFIG_266MHz(void) {
WORD ddr_pll_lock;
sscgpll_bypass_enable(HW_DRAM_PLL_CFG0_ADDR);

//266.2MHz
//reg32_write(HW_DRAM_PLL_CFG2_ADDR,0x01154604);
//265.7MHz
reg32_write(HW_DRAM_PLL_CFG2_ADDR,0x011d0684);

sscgpll_bypass_disable(HW_DRAM_PLL_CFG0_ADDR);
while ( ddr_pll_lock != 0x1 ) {
   ddr_pll_lock = wait_pll_lock(HW_DRAM_PLL_CFG0_ADDR);
  }
printk("Congratulations, DDR PLL1 266MHz locked success!\n");
}

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

void DDR_PLL_CONFIG_400MHz(void) {
WORD ddr_pll_lock;
sscgpll_bypass_enable(HW_DRAM_PLL_CFG0_ADDR);
//000 011111 100111 010000 000001 0
//reg32_write(HW_DRAM_PLL_CFG2_ADDR, 0x00fce802);
//000 011101 100011 001001 000000 0
reg32_write(HW_DRAM_PLL_CFG2_ADDR, 0x00ec6984);
sscgpll_bypass_disable(HW_DRAM_PLL_CFG0_ADDR);
while ( ddr_pll_lock != 0x1 ) {
   ddr_pll_lock = wait_pll_lock(HW_DRAM_PLL_CFG0_ADDR);
  }
printk("Congratulations, DDR PLL1 400MHz locked success!\n");
}
void DDR_PLL_CONFIG_395MHz(void) {
WORD ddr_pll_lock;
sscgpll_bypass_enable(HW_DRAM_PLL_CFG0_ADDR);
reg32_write(HW_DRAM_PLL_CFG2_ADDR, 0x00d3e984);
sscgpll_bypass_disable(HW_DRAM_PLL_CFG0_ADDR);
while ( ddr_pll_lock != 0x1 ) {
   ddr_pll_lock = wait_pll_lock(HW_DRAM_PLL_CFG0_ADDR);
  }
printk("Congratulations, DDR PLL1 395MHz locked success!\n");
}
void DDR_PLL_CONFIG_200MHz(void) {
WORD ddr_pll_lock;
sscgpll_bypass_enable(HW_DRAM_PLL_CFG0_ADDR);
//000 011111 100111 010000 000001 0
//reg32_write(HW_DRAM_PLL_CFG2_ADDR, 0x00fce802);
//000 011101 100011 001001 000000 0
reg32_write(HW_DRAM_PLL_CFG2_ADDR, 0x00ece586);
sscgpll_bypass_disable(HW_DRAM_PLL_CFG0_ADDR);
while ( ddr_pll_lock != 0x1 ) {
   ddr_pll_lock = wait_pll_lock(HW_DRAM_PLL_CFG0_ADDR);
  }
printk("Congratulations, DDR PLL1 200MHz locked success!\n");
}

void DDR_PLL_CONFIG_50MHz(void) {
WORD ddr_pll_lock;
sscgpll_bypass_enable(HW_DRAM_PLL_CFG0_ADDR);
//000 011111 100111 010000 000001 0
//reg32_write(HW_DRAM_PLL_CFG2_ADDR, 0x00fce802);
//000 011101 100011 001001 000000 0
reg32_write(HW_DRAM_PLL_CFG2_ADDR, 0x00ec09aa);
sscgpll_bypass_disable(HW_DRAM_PLL_CFG0_ADDR);
while ( ddr_pll_lock != 0x1 ) {
   ddr_pll_lock = wait_pll_lock(HW_DRAM_PLL_CFG0_ADDR);
  }
printk("Congratulations, DDR PLL1 50MHz locked success!\n");
}


void DDR_PLL_CONFIG_667MHz(void) {
WORD ddr_pll_lock;
sscgpll_bypass_enable(HW_DRAM_PLL_CFG0_ADDR);
//000 011111 100111 010000 000001 0
//reg32_write(HW_DRAM_PLL_CFG2_ADDR, 0x00fce802);
//000 011101 100011 001001 000000 0
reg32_write(HW_DRAM_PLL_CFG2_ADDR, 0x00ece480);
sscgpll_bypass_disable(HW_DRAM_PLL_CFG0_ADDR);
while ( ddr_pll_lock != 0x1 ) {
   ddr_pll_lock = wait_pll_lock(HW_DRAM_PLL_CFG0_ADDR);
  }
printk("Congratulations, DDR PLL1 400MHz locked success!\n");
}


void DDR_PLL_CONFIG_800MHz(void) {
WORD ddr_pll_lock;
sscgpll_bypass_enable(HW_DRAM_PLL_CFG0_ADDR);
//000 011111 100111 010000 000001 0
//reg32_write(HW_DRAM_PLL_CFG2_ADDR, 0x00fce802);
//000 011101 100011 001001 000000 0
reg32_write(HW_DRAM_PLL_CFG2_ADDR, 0x00ece580);
sscgpll_bypass_disable(HW_DRAM_PLL_CFG0_ADDR);
while ( ddr_pll_lock != 0x1 ) {
   ddr_pll_lock = wait_pll_lock(HW_DRAM_PLL_CFG0_ADDR);
  }
printk("Congratulations, DDR PLL1 800MHz locked success!\n");
}
#endif//end DDR3_1600MHZ_RET_16BIT_1RANK_FW09
