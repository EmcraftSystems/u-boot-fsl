/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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
#include "fsl_common.h"
#include "memtest.h"
#include "fsl_sdram.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* UART instance and clock */

#define SDRAM_START_ADDRESS SDRAM0_BASE_ADDRESS

#define LED_TOGGLE()  printf("@")     
#define SDRAM_EXAMPLE_DATALEN (32*1024*1024)
#define SDRAM_EXAMPLE_WRITETIMES (1000U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#define RETERR  1
/*******************************************************************************
 * Variables
 ******************************************************************************/

static unsigned long _random = 1;
int silent_test;
meminfo memparam;
void get_error(int index);
void ddr_test_all_55(void);
void ddr_test_all_0(void)
{
    unsigned int *p;
    unsigned int data;

    for(p = (unsigned int*)SDRAM_START_ADDRESS; (unsigned int)p < (SDRAM_START_ADDRESS+SDRAM_EXAMPLE_DATALEN); p++)
    {
        *p = 0;
    }
    
    for(p = (unsigned int*)SDRAM_START_ADDRESS; (unsigned int)p < (SDRAM_START_ADDRESS+SDRAM_EXAMPLE_DATALEN); p++)
    {
        data = *p;
        
        if(data != 0)
        {
            //toggle_gpio();
            get_error(1);
        }
    }
}

void ddr_test_all_1(void)
{
    volatile unsigned int *p;

    for(p = (volatile unsigned int*)SDRAM_START_ADDRESS; (volatile unsigned int)p < (SDRAM_START_ADDRESS+SDRAM_EXAMPLE_DATALEN); p++)
    {
        *p = 0xffffffff;
    }
    
    for(p = (volatile unsigned int*)SDRAM_START_ADDRESS; (volatile unsigned int)p < (SDRAM_START_ADDRESS+SDRAM_EXAMPLE_DATALEN); p++)
    {
        if(*p != 0xffffffff)
            get_error(1);
    }
}
void ddr_test_all_55(void)
{
    volatile unsigned int *p;

    for(p = ( unsigned int*)SDRAM_START_ADDRESS; (unsigned int)p < (SDRAM_START_ADDRESS+SDRAM_EXAMPLE_DATALEN); p++)
    {
        *p = 0xaa55aa55;
    }
    
    for(p = (unsigned int*)SDRAM_START_ADDRESS; (unsigned int)p < (SDRAM_START_ADDRESS+SDRAM_EXAMPLE_DATALEN); p++)
    {
        if(*p != 0xaa55aa55)
            get_error(1);
    }
}
void cpu_fillmem(pmeminfo meminfo, uint32 pattern)
{
	uint8 *ram8 = 0;
	uint16 *ram16 = 0;
	uint32 *ram32 = 0;
	void *curroff = 0;
	uint32 i, remain = 0, access;
	uint32 size = 0;

	// printf("Running FillMem test ...\n");

	access = meminfo->xfertype;
	curroff = (void *)meminfo->dstoff;
	size = meminfo->dstsz;
	
	while (access) {
		switch (access) {
		case 32:	/* 32 bit access */
			ram32 = (uint32 *) curroff;
			remain = size % 4;
			for (i = 0; i < (size >> 2); i++) {
				*ram32 = pattern;
				ram32++;
			}
			curroff = (void *)ram32;
			break;
		case 16:	/* 16 bit access */
			ram16 = (uint16 *) curroff;
			remain = size % 2;
			for (i = 0; i < (size >> 1); i++) {
				*ram16 = (uint16) pattern;
				ram16++;
			}
			curroff = (void *)ram16;
			break;
		case 8:	/* 8 bit access */
			ram8 = (uint8 *)curroff;
			remain = size % 1;
			for (i = 0; i < size; i++) {
				*ram8 = (uint8) pattern;
				ram8++;
			}
			break;
		}		/* switch (access) */

		switch (remain) {
		case 0:	/* no byte left, quit */
			access = 0;
			break;
		case 1:	/* 1 byte left */
			access = 8;
			break;
		case 2:
		case 3:	/* 2 or 3 bytes left */
			access = 16;
			break;
		default:
			access = 32;
			break;
		}

		size = remain;
	}			/* while ( access ) */
}

uint32 cpu_checkmem(pmeminfo meminfo, uint32 pattern)
{
	uint32 *ram32 = 0;
	uint16 *ram16 = 0;
	uint8 *ram8 = 0;
	void *curroff = 0;
	uint32 i, remain = 0, access;
	uint32 size = 0;
        uint32_t errorcount = 0;
	// printf("Running CheckMem test ...\n");

	access = meminfo->xfertype;
	curroff = (void *)meminfo->dstoff;
	size = meminfo->dstsz;

	while (access) {
		switch (access) {
		case 32:
			{
				uint32 rdback32;
				ram32 = (uint32 *) curroff;
				remain = size % 4;
				for (i = 0; i < (size >> 2); i++) {
					rdback32 = *ram32;
					if (rdback32 != pattern) {
                                          errorcount++;
						// printf
						//    ("CheckMem pattern 0x%08x @ 0x%08x - 0x%08x\n",
						//     pattern, (uint32) ram32,
						//     rdback32);
					}	/* if (rdback32 != pattern) */
					ram32++;
				}	/* for ( i = 0; i < size; i++ ) */
				curroff = (void *)ram32;
				break;
			}

		case 16:
			{
				uint16 rdback16;
				ram16 = (uint16 *) curroff;
				remain = size % 2;
				for (i = 0; i < (size >> 1); i++) {
					rdback16 = *ram16;
					if (rdback16 != (uint16) pattern) {
                                          errorcount++;
						// printf
						//    ("CheckMem pattern 0x%04x @ 0x%08x - 0x%04x\n",
						//     (uint16) pattern,
						//     (uint32) ram16, rdback16);
					}	/* if ( readback16 != (uint16)pattern ) */
					ram16++;
				}	/* for ( i = 0; i < size; i++ ) */
				curroff = (void *)ram16;
				break;
			}

		case 8:
			{
				uint8 rdback8;
				ram8 = (uint8 *) curroff;
				remain = size % 1;
				for (i = 0; i < size; i++) {
					rdback8 = *ram8;
					if (rdback8 != (uint8) pattern) {
                                          errorcount++;
						//     printf
						//    ("CheckMem pattern 0x%02x @ 0x%08x - 0x%02x\n",
						//     (uint8) pattern,
						//     (uint32) ram8, rdback8);
					}
					ram8++;
				}
				break;
			}
		}		/* switch (access) */

		switch (remain) {
		case 0:
			access = 0;
			break;
		case 1:
			access = 8;
			break;
		case 2:
		case 3:
			access = 16;
			break;
		default:
			access = 32;
			break;
		}
		size = remain;
	}			/* while (access) */

	// printf("End CheckMem test\n\n");

	return (errorcount);
}

int rand(void)
{
	unsigned u;
	// make a copy so threads don't clobber in the middle of the calculation
	unsigned long xrandom = _random;

	u = xrandom >> 3;
	/* both # are prime, garenteeing a maximum cycle */
	xrandom = xrandom * 100073969 + 297607;

	/* still had problems with the last digit having cycles, this fixes it */
	xrandom ^= u;
	xrandom &= RAND_MAX;
	return (_random = xrandom);
}

void print_test_name(char *testname, uint32 * test)
{
	// printf("  Test %2lu:  %20.20s:  Setting...", *test, testname);
	(*test)++;
}

/* Function definitions. */
int test_verify_success(uint32 * bp1, uint32 * bp2, uint32 count)
{
	uint32 *p1 = (uint32 *)bp1;
	uint32 *p2 = (uint32 *)bp2;
	uint32 i;

	if (!silent_test)
		// printf("Testing...");

	for (i = 0; i < count; i++, p1++, p2++) {
		if (*p1 != *p2) {
			// printf("\nFAILURE: 0x%08lx != 0x%08lx at offset 0x%08lx.\n", (uint32)*p1, (uint32)*p2, i);
#ifdef RETERR
			// printf("Skipping to next test...\n");
			return (ERROR);
#endif
		}
	}
	return (OK);
}

int test_random_value(uint32 * bp1, uint32 * bp2, uint32 count)
{
	volatile uint32 *p1 = (volatile uint32 *)bp1;
	volatile uint32 *p2 = (volatile uint32 *)bp2;
	uint32 i;

	silent_test = 0;
	for (i = 0; i < count; i++) {
		*p1++ = *p2++ = RAND32;
	}
	return (test_verify_success(bp1, bp2, count));
}

int test_xor_comparison(uint32 * bp1, uint32 * bp2, uint32 count)
{
	volatile uint32 *p1 = (volatile uint32 *)bp1;
	volatile uint32 *p2 = (volatile uint32 *)bp2;
	uint32 i;
	uint32 q = RAND32;

	silent_test = 0;
	for (i = 0; i < count; i++) {
		*p1++ ^= q;
		*p2++ ^= q;
	}
	return (test_verify_success(bp1, bp2, count));
}

int test_sub_comparison(uint32 * bp1, uint32 * bp2, uint32 count)
{
	volatile uint32 *p1 = (volatile uint32 *)bp1;
	volatile uint32 *p2 = (volatile uint32 *)bp2;
	uint32 i;
	uint32 q = RAND32;

	silent_test = 0;
	for (i = 0; i < count; i++) {
		*p1++ -= q;
		*p2++ -= q;
	}
	return (test_verify_success(bp1, bp2, count));
}

int test_mul_comparison(uint32 * bp1, uint32 * bp2, uint32 count)
{
	volatile uint32 *p1 = (volatile uint32 *)bp1;
	volatile uint32 *p2 = (volatile uint32 *)bp2;
	uint32 i;
	uint32 q = RAND32;

	silent_test = 0;
	for (i = 0; i < count; i++) {
		*p1++ *= q;
		*p2++ *= q;
	}
	return (test_verify_success(bp1, bp2, count));
}

int test_div_comparison(uint32 * bp1, uint32 * bp2, uint32 count)
{
	volatile uint32 *p1 = (volatile uint32 *)bp1;
	volatile uint32 *p2 = (volatile uint32 *)bp2;
	uint32 i;
	uint32 q = RAND32;

	silent_test = 0;
	for (i = 0; i < count; i++) {
		if (!q)
			q++;
		*p1++ /= q;
		*p2++ /= q;
	}
	return (test_verify_success(bp1, bp2, count));
}

int test_or_comparison(uint32 * bp1, uint32 * bp2, uint32 count)
{
	volatile uint32 *p1 = (volatile uint32 *)bp1;
	volatile uint32 *p2 = (volatile uint32 *)bp2;
	uint32 i;
	uint32 q = RAND32;

	silent_test = 0;
	for (i = 0; i < count; i++) {
		*p1++ |= q;
		*p2++ |= q;
	}
	return (test_verify_success(bp1, bp2, count));
}

int test_and_comparison(uint32 * bp1, uint32 * bp2, uint32 count)
{
	volatile uint32 *p1 = (volatile uint32 *)bp1;
	volatile uint32 *p2 = (volatile uint32 *)bp2;
	uint32 i;
	uint32 q = RAND32;

	silent_test = 0;
	for (i = 0; i < count; i++) {
		*p1++ &= q;
		*p2++ &= q;
	}
	return (test_verify_success(bp1, bp2, count));
}

int test_seqinc_comparison(uint32 * bp1, uint32 * bp2, uint32 count)
{
	volatile uint32 *p1 = (volatile uint32 *)bp1;
	volatile uint32 *p2 = (volatile uint32 *)bp2;
	uint32 i;
	uint32 q = RAND32;

	silent_test = 0;
	for (i = 0; i < count; i++)
		*p1++ = *p2++ = (i + q);
	return (test_verify_success(bp1, bp2, count));
}

int test_solidbits_comparison(uint32 * bp1, uint32 * bp2, uint32 count)
{
	volatile uint32 *p1 = (volatile uint32 *)bp1;
	volatile uint32 *p2 = (volatile uint32 *)bp2;
	uint32 q, i, j;

	silent_test = 1;
	// printf("    ");
	for (j = 0; j < 64; j++) {
		q = (j % 2) == 0 ? 0xFFFFFFFF : 0x00000000;
		// printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\bSetting... %3lu", j);
		p1 = (volatile uint32 *)bp1;
		p2 = (volatile uint32 *)bp2;
		for (i = 0; i < count; i++) {
			*p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
		}

		// printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\bTesting... %3lu", j);

		if (test_verify_success(bp1, bp2, count) == ERROR)
			return (ERROR);
	}
	// printf("\b\b\b\b");
	return (OK);
}

int test_checkerboard_comparison(uint32 * bp1, uint32 * bp2, uint32 count)
{
	volatile uint32 *p1 = (volatile uint32 *)bp1;
	volatile uint32 *p2 = (volatile uint32 *)bp2;
	uint32 q, i, j;

	silent_test = 1;
	// printf("    ");
	for (j = 0; j < 64; j++) {
		q = (j % 2) == 0 ? 0x55555555 : 0xAAAAAAAA;
		// printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\bSetting... %3lu", j);
		p1 = (volatile uint32 *)bp1;
		p2 = (volatile uint32 *)bp2;
		for (i = 0; i < count; i++) {
			*p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
		}

		// printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\bTesting... %3lu", j);

		if (test_verify_success(bp1, bp2, count) == ERROR)
			return (ERROR);
	}
	// printf("\b\b\b\b");
	return (OK);
}

int test_blockseq_comparison(uint32 * bp1, uint32 * bp2, uint32 count)
{
	volatile uint32 *p1 = (volatile uint32 *)bp1;
	volatile uint32 *p2 = (volatile uint32 *)bp2;
	uint32 i, j;

	silent_test = 1;
	// printf("    ");
	for (j = 0; j < 256; j++) {
		// printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\bSetting... %3lu", j);
		p1 = (volatile uint32 *)bp1;
		p2 = (volatile uint32 *)bp2;
		for (i = 0; i < count; i++) {
			*p1++ = *p2++ = MAKE32FROM8(j);
		}

		// printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\bTesting... %3lu", j);

		if (test_verify_success(bp1, bp2, count) == ERROR)
			return (ERROR);
	}
	// printf("\b\b\b\b");
	return (OK);
}

int test_walkbits_comparison(uint32 * bp1, uint32 * bp2, uint32 count, int m)
{
	volatile uint32 *p1 = (volatile uint32 *)bp1;
	volatile uint32 *p2 = (volatile uint32 *)bp2;
	uint32 i, j;

	silent_test = 1;
	// printf("    ");
	for (j = 0; j < 64; j++) {
		// printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\bSetting... %3lu", j);
		p1 = (volatile uint32 *)bp1;
		p2 = (volatile uint32 *)bp2;
		for (i = 0; i < count; i++) {
			if (j < 32) {	/* Walk it up. */
				*p1++ = *p2++ = (m == 0) ? 0x00000001 << j :
				    0xFFFFFFFF ^ (0x00000001 << j);
			} else {	/* Walk it back down. */

				*p1++ = *p2++ = (m == 0)
				    ? 0x00000001 << (64 - j - 1)
				    : 0xFFFFFFFF ^ (0x00000001 << (64 - j - 1));
			}
		}

		// printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\bTesting... %3lu", j);
		if (test_verify_success(bp1, bp2, count) == ERROR)
			return (ERROR);
	}
	// printf("\b\b\b\b");
	return (OK);
}

int test_bitspread_comparison(uint32 * bp1, uint32 * bp2, uint32 count)
{
	volatile uint32 *p1 = (volatile uint32 *)bp1;
	volatile uint32 *p2 = (volatile uint32 *)bp2;
	uint32 i, j;

	silent_test = 1;
	// printf("    ");
	for (j = 0; j < 64; j++) {
		// printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\bSetting... %3lu", j);
		p1 = (volatile uint32 *)bp1;
		p2 = (volatile uint32 *)bp2;

		for (i = 0; i < count; i++) {
			if (j < 32) {	/* Walk it up. */
				*p1++ = *p2++ = (i % 2 == 0)
				    ? (0x00000001 << j) | (0x00000001 <<
							   (j + 2))
				    : 0xFFFFFFFF ^ ((0x00000001 << j)
						    | (0x00000001 << (j + 2)));
			} else {	/* Walk it back down. */

				*p1++ = *p2++ = (i % 2 == 0)
				    ? (0x00000001 << (63 - j)) | (0x00000001 <<
								  (65 - j))
				    : 0xFFFFFFFF ^ (0x00000001 << (63 - j)
						    | (0x00000001 << (65 - j)));
			}
		}

		// printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\bTesting... %3lu", j);
		if (test_verify_success(bp1, bp2, count) == ERROR)
			return (ERROR);
	}
	// printf("\b\b\b\b");
	return (OK);
}

int test_bitflip_comparison(uint32 * bp1, uint32 * bp2, uint32 count)
{
	volatile uint32 *p1 = (volatile uint32 *)bp1;
	volatile uint32 *p2 = (volatile uint32 *)bp2;
	uint32 i, j, k;
	uint32 q;

	silent_test = 1;
	// printf("    ");
	for (k = 0; k < 32; k++) {
		q = 0x00000001 << k;

		for (j = 0; j < 8; j++) {
			q = ~q;
			// printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\bSetting... %3lu",
			//     k * 8 + j);
			p1 = (volatile uint32 *)bp1;
			p2 = (volatile uint32 *)bp2;

			for (i = 0; i < count; i++)
				*p1++ = *p2++ = (i % 2) == 0 ? q : ~q;

			// printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\bTesting... %3lu",
			//     k * 8 + j);

			if (test_verify_success(bp1, bp2, count) == ERROR)
				return (ERROR);
		}

	}
	// printf("\b\b\b\b");
	return (OK);
}

int test_stuck_address(uint32 * bp1, uint32 * unused, uint32 count)
{
	uint32 *p1;
	/* second argument is not used; just gives it a compatible signature. */
	uint32 i, j;

	count <<= 1;
	// printf("    ");
	for (j = 0; j < 16; j++) {
		p1 = (uint32 *)bp1;
		// printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\bSetting... %3lu", j);
		for (i = 0; i < count; i++) {
			*p1 =
			    ((j + i) % 2) == 0 ? (uint32) p1 : (~((uint32) p1));
			p1++;
		}
		// printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\bTesting... %3lu", j);
		p1 = (uint32 *)bp1;
		for (i = 0; i < count; i++, p1++) {
			if (*p1 !=
			    (((j + i) % 2) ==
			     0 ? (uint32) p1 : ~((uint32) p1))) {
				// printf("\nFAILURE: bad address line at offset"
				//     " 0x%08lx.\n", i);

#ifdef RETERR
				// printf("Skipping to next test...\n");
				return (ERROR);
#endif
			}
		}
	}

	// printf("\b\b\b\b");
	return (OK);
}

void get_error(int index)
{
    static int i;
    while(1)
    {
        i = index;
        while(1)
        {
                LED_TOGGLE();
                for(i=0;i<0x8ffff;i++);
                printf("sdram test fail!\r\n");
        }
    }
}

void memTest(pmeminfo meminfo)
{
	uint32 *start, *end;
	uint32 run, maxruns, runerrors = 0, totalerrors = 0, i;
	uint32 test, count, memsplit;
	int rc, loop = 1;
	uint32 base, size;

	teststruct_0 tests_noarg[] = {
		{"Stuck Address", test_stuck_address},
		{"Random value", test_random_value},
		{"XOR comparison", test_xor_comparison},
		{"SUB comparison", test_sub_comparison},
		{"MUL comparison", test_mul_comparison},
		{"DIV comparison", test_div_comparison},
		{"OR comparison", test_or_comparison},
		{"AND comparison", test_and_comparison},
		{"Sequential Increment", test_seqinc_comparison},
		{"Solid Bits", test_solidbits_comparison},
		{"Block Sequential", test_blockseq_comparison},
		{"Checkerboard", test_checkerboard_comparison},
		{"Bit Spread", test_bitspread_comparison},
		{"Bit Flip", test_bitflip_comparison},
		{NULL, 0}
	};

	teststruct_1 tests_1arg[] = {
		{"Walking Ones", test_walkbits_comparison, 0},
		{"Walking Zeroes", test_walkbits_comparison, 1},
		{NULL, 0, 0}
	};

	base = meminfo->dstoff;
	size = meminfo->dstsz;

	start = (uint32 *) base;
	memsplit = size >> 1;
	end = (uint32 *) (base + memsplit);
	count = memsplit >> 2;

	while (loop) {
		// printf("memTest2 Test Select:\n");
		// printf("=====================\n");
		// printf("1. Stuck Address\n");
		// printf("2. Random value\n");
		// printf("3. XOR comparison\n");
		// printf("4. SUB comparison\n");
		// printf("5. MUL comparison\n");
		// printf("6. DIV comparison\n");
		// printf("7. OR comparison\n");
		// printf("8. AND comparison\n");
		// printf("9. Sequential Increment\n");
		// printf("a. Solid Bits\n");
		// printf("b. Block Sequentia\n");
		// printf("c. Checkerboard\n");
		// printf("d. Bit Spread\n");
		// printf("e. Bit Flip\n");
		// printf("f. Walking One\n");
		// printf("g. Walking Zeroes\n");
		// printf("h. All Tests\n");
		// printf("i. Exit\n");
		// printf("Select: ");
		// select = uart_getchar(TERM_PORT);
		// printf("\n");

		//if (select == 'i') {
		//	/* Exit */
		//	loop = 0;
		//	// printf("\n\n");
		//	return;
		//} else if ((select < '1') || (select > 'i'))
		//	continue;

		// printf("\nInput run tests times <CR>: ");
		//processNo(&maxruns, 10);
       maxruns = 1;
		// printf("\n\n");
        LED_TOGGLE();
		if (1) {
			/* Run all Tests */
			for (run = 0; run < maxruns; run++) {
				test = 1;
				// printf("Run %4lu:\n", run + 1);

				/* Do basic tests (taking two pointers plus count) here. */
				for (i = 0;; i++) {
					if (tests_noarg[i].name == NULL)
						break;

					// print_test_name(tests_noarg[i].name,
					//	    	&test);
					rc = tests_noarg[i].fp(start, end,
							       count);

					if (!rc)
						;// printf("Passed.\n");  ///wp
					else
                    {
						runerrors++;
                        get_error(i);
                    }
                    LED_TOGGLE();
				}
                                        LED_TOGGLE();
				/* Do other tests (taking two pointers, count, and arg) here. */
				for (i = 0;; i++) {
					if (tests_1arg[i].name == NULL)
						break;

					// print_test_name(tests_1arg[i].name,
					// 		&test);
					rc = tests_1arg[i].fp(start, end, count,
							      tests_1arg[i].
							      arg);
					if (!rc)
						; // printf("Passed.\n");  //wp
					else
                    {
						runerrors++;
                        get_error(i);
                    }
                    LED_TOGGLE();
				}
			}
		}  

		// printf("\nTotal Runs %4lu completed (%lu tests showed errors).\n",
		//     run, runerrors);
		totalerrors += runerrors;
		runerrors = 0;
        loop --;
        if(totalerrors > 0)
            get_error(i);

		// printf("%Total %4lu runs completed.  %lu errors detected.\n\n",
		//     run, totalerrors);
		// printf("Exiting...\n");
	}
}
void memory_test(void)
{
    uint32_t k = 0;
#if(1)
   // while(1)
    {
        ddr_test_all_0();
        LED_TOGGLE();
        ddr_test_all_1();
        LED_TOGGLE();
        //PRINTF("SDRAM test start continue...!\r\n");
        memparam.dstoff = SDRAM_START_ADDRESS;	/* start of test offset */
        memparam.dstsz  = SDRAM_EXAMPLE_DATALEN;
        memparam.xfertype = 8;
        cpu_fillmem(&memparam, 0xa5a5a5a5);
        cpu_checkmem(&memparam, 0xa5a5a5a5);
        LED_TOGGLE();
        cpu_fillmem(&memparam, 0);
        cpu_checkmem(&memparam, 0);
        LED_TOGGLE();
        memparam.xfertype = 16;
        cpu_fillmem(&memparam, 0xaa55aa55);
        cpu_checkmem(&memparam, 0xaa55aa55);
        LED_TOGGLE();
        cpu_fillmem(&memparam, 0);
        cpu_checkmem(&memparam, 0);
        LED_TOGGLE();
        memparam.xfertype = 32;
        cpu_fillmem(&memparam, 0xaaaa5555);
        cpu_checkmem(&memparam, 0xaaaa5555);
        LED_TOGGLE();
        LED_TOGGLE();
        cpu_fillmem(&memparam, 0);
        cpu_checkmem(&memparam, 0);
        LED_TOGGLE();
        memTest(&memparam);
        k++;
        LED_TOGGLE();
    }
#else
        while(1)
        {
            ddr_test_all_0();
            LED_TOGGLE();
            ddr_test_all_1();
            LED_TOGGLE();
            ddr_test_all_55();
        }
#endif
}



