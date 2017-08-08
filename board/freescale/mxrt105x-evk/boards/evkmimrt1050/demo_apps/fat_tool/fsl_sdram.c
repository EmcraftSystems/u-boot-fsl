/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright (c) 2016 - 2017 , NXP
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
 * o Neither the name of copyright holder nor the names of its
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
#include "fsl_sdram.h"


void Delay (uint32_t i);
uint8_t SDRAM_WaitIpCmdDone (void);
uint32_t SDRAM_IpCmd (uint32_t mem_addr, uint32_t ipcmd, uint32_t wd); 
void SDRAM_InitSequence(uint32_t bl,uint32_t cl);



void Delay (uint32_t i) {
  uint32_t j;
  for(j=0;j<i;j++);
}

uint8_t SDRAM_WaitIpCmdDone (void) 
{
  uint32_t temp_data,i;
  do
  {
    if(i++>10000*1000)
    {
        break;
    }
  }while(!(SEMC->INTR & 0x3));
  
  if(SEMC->INTR &0x01)
  {
    return true;
  }
  else
  {
    return false;
  }
}


uint32_t SDRAM_IpCmd (uint32_t mem_addr, uint32_t ipcmd, uint32_t wd) {
   uint32_t sd_cmd_w_read, sd_cmd_w_write, rd;
   rd = 0x0;
   if((ipcmd==SD_CC_IWR) || ((ipcmd & 0x0000FFFF)==SD_CC_IMS))  
   {
     SEMC->IPTXDAT = wd & 0x0000FFFF;
   }
   SEMC->IPCR0 = mem_addr;
   SEMC->IPCMD = 0xA55A0000 | (ipcmd & 0x0000FFFF);
   SDRAM_WaitIpCmdDone();
   if(ipcmd==SD_CC_IRD)  
   {
     rd = SEMC->IPRXDAT;  // Read from IPRXD
   }
   return rd;
}

void SDRAM_InitSequence(uint32_t bl,uint32_t cl)
{
    
    Delay (0x100);
    SDRAM_IpCmd(SDRAM0_BASE_ADDRESS, SD_CC_IPREA, 0x0000);
    SDRAM_IpCmd(SDRAM0_BASE_ADDRESS, SD_CC_IAF,   0x0000);
    SDRAM_IpCmd(SDRAM0_BASE_ADDRESS, SD_CC_IAF,   0x0000);
    SDRAM_IpCmd(SDRAM0_BASE_ADDRESS, SD_CC_IMS,   bl|(cl<<4));

    SDRAM_IpCmd(SDRAM1_BASE_ADDRESS, SD_CC_IPREA, 0x0000);
    SDRAM_IpCmd(SDRAM1_BASE_ADDRESS, SD_CC_IAF,   0x0000);
    SDRAM_IpCmd(SDRAM1_BASE_ADDRESS, SD_CC_IAF,   0x0000);
    SDRAM_IpCmd(SDRAM1_BASE_ADDRESS, SD_CC_IMS,   bl|(cl<<4));

    Delay (0x100);
  
   // SEMC->SDRAMCR3 = 0; /*RE_EN*/
}

void SDRAM_Init(uint32_t bl,uint32_t cl)
{
    CLOCK_EnableClock(kCLOCK_Semc);    
    //pin mux configuration
    uint32_t i;
    for(i=0;i<42;i++)
    {
        IOMUXC->SW_MUX_CTL_PAD[i]  = 0;
        IOMUXC->SW_PAD_CTL_PAD[i]  = 0x3<<6|0x6<<3|0x01;
    }
    IOMUXC->SW_MUX_CTL_PAD[39]  = 0x1 << 4;

    SEMC->MCR &= ~SEMC_MCR_MDIS_MASK;
    
    SEMC->MCR = 0x10000004;
    
    SEMC->BMCR0 = 0x00030524;
    
    SEMC->BMCR1 = 0x06030524;

    SEMC->BR[0] = (SDRAM0_BASE_ADDRESS&0xfffff000)
                |(SEMC_BR_MS(0xD))
                |(SEMC_BR_VLD_MASK);
    SEMC->IOCR |= SEMC_IOCR_MUX_CSX0(1);     // config SEMC_CCSX0 as SDRAM_CS1

    SEMC->SDRAMCR0 &= (~SEMC_SDRAMCR0_BL_MASK)
                     |(~SEMC_SDRAMCR0_CL_MASK)
                     ;
    SEMC->SDRAMCR0 = SEMC_SDRAMCR0_BL(bl)
                    |SEMC_SDRAMCR0_CL(cl)
                    |0x301
                    ;
    SEMC->SDRAMCR1 = 0x652922;
    SEMC->SDRAMCR2 = 0x10920;
    SEMC->SDRAMCR3 = 0x50210A09;

    SEMC->DBICR0 = 0x00000021;
    SEMC->DBICR1 = 0x00888888;
    SEMC->IPCR1 = 0x2;
    SEMC->IPCR2 = 0;
 //   SEMC->MCR = 0;
    SDRAM_InitSequence(bl,cl);
 
}

