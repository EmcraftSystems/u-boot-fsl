#ifndef __ddr_h__ 
#define __ddr_h__

typedef unsigned int WORD;

#define printk printf
#define info_trigger

#define reg32_write(a,v) writel(v,a)
#define reg32_read(v) readl(v)

#define  reg32setbit(addr,bitpos) \
    reg32_write((addr),(reg32_read((addr)) | (1<<(bitpos))))
#define  reg16setbit(addr,bitpos) \
    reg16_write((addr),(reg16_read((addr)) | (1<<(bitpos))))
#define  reg8setbit(addr,bitpos) \
    reg8_write((addr),(reg16_read((addr)) | (1<<(bitpos))))
#define  reg32clrbit(addr,bitpos) \
         reg32_write((addr),(reg32_read((addr)) & (0xFFFFFFFF ^ (1<<(bitpos)))))

#define dwc_ddrphy_apb_wr(addr, data)  reg32_write(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*(addr), data)
#define dwc_ddrphy_apb_rd(addr)  reg32_read(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*(addr))

enum fw_type {
	FW_1D_IMAGE,
	FW_2D_IMAGE,
};

void ddr_load_train_code(enum fw_type type);
void lpddr4_800M_cfg_phy(void);


//#define SRC_DDRC_RCR_ADDR SRC_IPS_BASE_ADDR +0x1000
//#define DDR_CSD1_BASE_ADDR 0x40000000
#define ANAMIX_PLL_BASE_ADDR         0x30360000
#define HW_DRAM_PLL_CFG0_ADDR (ANAMIX_PLL_BASE_ADDR + 0x60)
#define HW_DRAM_PLL_CFG1_ADDR (ANAMIX_PLL_BASE_ADDR + 0x64)
#define HW_DRAM_PLL_CFG2_ADDR (ANAMIX_PLL_BASE_ADDR + 0x68)
#define GPC_PU_PWRHSK 0x303A01FC
#define GPC_TOP_CONFIG_OFFSET        0x0000
#define AIPS1_ARB_BASE_ADDR             0x30000000 
#define AIPS_TZ1_BASE_ADDR              AIPS1_ARB_BASE_ADDR
#define AIPS1_OFF_BASE_ADDR             AIPS_TZ1_BASE_ADDR+0x200000
#define GPC_IPS_BASE_ADDR              AIPS1_OFF_BASE_ADDR+0x1A0000
#define GPC_BASE_ADDR GPC_IPS_BASE_ADDR
#define GPC_PU_PWRHSK                (GPC_BASE_ADDR + GPC_TOP_CONFIG_OFFSET + 0x01FC)  //added by baoxye
#define CCM_IPS_BASE_ADDR              AIPS1_OFF_BASE_ADDR+0x180000
#define CCM_SRC_CTRL_OFFSET     CCM_IPS_BASE_ADDR+0x800
#define CCM_SRC_CTRL(n)             (CCM_SRC_CTRL_OFFSET+0x10*n)


extern void mem_test(u32 type);

void inline sscgpll_bypass_enable(WORD reg_addr) {
  WORD read_data;
  read_data = reg32_read(reg_addr);
  reg32_write(reg_addr, read_data | 0x00000010);
  read_data = reg32_read(reg_addr);
  reg32_write(reg_addr, read_data | 0x00000020);
}



void inline sscgpll_bypass_disable(WORD reg_addr) {
  WORD read_data;
  read_data = reg32_read(reg_addr);
  reg32_write(reg_addr, read_data & 0xffffffdf);
  read_data = reg32_read(reg_addr);
  reg32_write(reg_addr, read_data & 0xffffffef);
}
unsigned int inline wait_pll_lock(WORD reg_addr) {
  WORD pll_lock;
  pll_lock = reg32_read(reg_addr) >> 31;
  return pll_lock;
}
#endif
