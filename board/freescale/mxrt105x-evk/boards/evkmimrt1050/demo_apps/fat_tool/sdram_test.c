/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "memtest.h"
#include "fsl_sdram.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#define SDRAM_MEM_LENGTH        32*1024*1024
#define MEM_START_ADDRESS       0x80000000
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
uint32_t memtest_buff[256];
void sdram_mem_write(uint32_t seed,uint32_t count)
{
  volatile uint32_t *p;
  p = (volatile uint32_t *)MEM_START_ADDRESS;
  uint32_t i;
  for(i=0;i<count;i++)
  {
    p[i] = i+seed;
  }
}
void sdram_mem_read(uint32_t *Rd,uint32_t count)
{
  volatile uint32_t *p;
  p = (volatile uint32_t *)MEM_START_ADDRESS;
  uint32_t i;
  for(i=0;i<256;i++)
  {
    Rd[i] = p[i];
  }
}

uint32_t sdram_mem_check(uint32_t seed,uint32_t count)
{
  uint32_t i;
  uint32_t errcount = 0;
  volatile uint32_t *pSdram;
  pSdram = (uint32_t *)MEM_START_ADDRESS;
  for(i=0;i<count;i++)
  {
    if(pSdram[i] != (i+seed))
    {
      errcount++;
    }
  }
  return errcount;
}

void sdram_mem_test(uint32_t *Rd,uint32_t seed)
{

}





