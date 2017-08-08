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

#ifndef _MEMTEST_H
#define _MEMTEST_H

#ifndef uint32
#define uint32  unsigned long
#endif
#ifndef uint16
#define uint16  unsigned short
#endif
#ifndef uint8
#define uint8   unsigned char
#endif

//#define RETERR      0
#define OK          0
#define ERROR      -1
#define RAND32          (uint32) ( (uint32) rand () | ( (uint32) rand () << 16))
#define MAKE32FROM8(X)  (uint32) (X | X << 8 | X << 16 | X << 24)
//#define MAX(a,b)        ( (a > b) ? a : b )

#ifndef RAND_MAX
#define RAND_MAX 2147483647
#endif

typedef struct mem_test_region {
	unsigned long srcoff;	/* memory source offset */
	unsigned long srcsz;	/* memory source size */
	unsigned long dstoff;	/* memory destination offset */
	unsigned long dstsz;	/* memory destination size */
	unsigned char xfertype;	/* access method, 1 - byte, 2 - word, 4 - longword
							 * 16 - line, 32 - dma */
} meminfo, *pmeminfo;

/* Datatypes. */
typedef struct teststruct_noarg
{
    char *name;
    int (*fp)(uint32*, uint32*, uint32);
} teststruct_0;

typedef struct teststruct_onearg
{
    char *name;
    int (*fp)(uint32*, uint32*, uint32, int);
    int arg;
} teststruct_1;

/* mem Function declarations. */
void cpu_fillmem(pmeminfo meminfo, uint32 pattern);
uint32 cpu_checkmem(pmeminfo meminfo, uint32 pattern);

/* memTest2 Function declarations. */
void memTest(pmeminfo meminfo);
int test_verify_success (uint32 *bp1, uint32 *bp2, uint32 count);
int test_random_value (uint32 *bp1, uint32 *bp2, uint32 count);
int test_xor_comparison (uint32 *bp1, uint32 *bp2, uint32 count);
int test_sub_comparison (uint32 *bp1, uint32 *bp2, uint32 count);
int test_mul_comparison (uint32 *bp1, uint32 *bp2, uint32 count);
int test_div_comparison (uint32 *bp1, uint32 *bp2, uint32 count);
int test_or_comparison (uint32 *bp1, uint32 *bp2, uint32 count);
int test_and_comparison (uint32 *bp1, uint32 *bp2, uint32 count);
int test_seqinc_comparison (uint32 *bp1, uint32 *bp2, uint32 count);
int test_checkerboard_comparison (uint32 *bp1, uint32 *bp2, uint32 count);
int test_solidbits_comparison (uint32 *bp1, uint32 *bp2, uint32 count);
int test_blockseq_comparison (uint32 *bp1, uint32 *bp2, uint32 count);
int test_bitspread_comparison (uint32 *bp1, uint32 *bp2, uint32 count);
int test_bitflip_comparison (uint32 *bp1, uint32 *bp2, uint32 count);
int test_stuck_address (uint32 *bp1, uint32 *bp2, uint32 count);
int test_walkbits_comparison (uint32 *bp1, uint32 *bp2, uint32 count, int mode);
void memory_test(void);

#endif			/* _MEMTEST_H */


