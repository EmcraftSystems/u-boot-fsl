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

#ifndef _FSL_SDRAM_H_
#define _FSL_SDRAM_H_

#include "fsl_device_registers.h"
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define SDRAM0_BASE_ADDRESS  0x80000000
#define SDRAM1_BASE_ADDRESS  0x80000000+256*1024*1024


// SDRAM Command Code
#define SD_CC_ARD       0x0     // Master Bus (AXI) command - Read
#define SD_CC_AWR       0x1     // Master Bus (AXI) command - Write
#define SD_CC_IRD       0x8     // IP command - Read
#define SD_CC_IWR       0x9     // IP command - Write
#define SD_CC_IMS       0xA     // IP command - Set Mode Register
#define SD_CC_IACT      0xB     // IP command - ACTIVE
#define SD_CC_IAF       0xC     // IP command - Auto Refresh
#define SD_CC_ISF       0xD     // IP Command - Self Refresh
#define SD_CC_IPRE      0xE     // IP command - Precharge
#define SD_CC_IPREA     0xF     // IP command - Precharge ALL


void SDRAM_Init(uint32_t bl,uint32_t cl);


#endif

