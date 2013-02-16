/*
 * Copyright 2012 Freescale Semiconductor, Inc.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/io.h>
#include <div64.h>
#include <asm/arch/timer.h>
#include <asm/arch/vybrid-regs.h>

DECLARE_GLOBAL_DATA_PTR;

#define timestamp (gd->tbl)
#define timerticks (gd->tbu)
#define lastinc	(gd->lastinc)
static unsigned long ltmstamp = 0;

#define CONFIG_TMR_USEPIT
#ifdef CONFIG_TMR_USEPIT

unsigned long long _usec2ticks(unsigned long long usec);

int timer_init(void)
{
	ulong usecs;
	ulong ticks;

	timestamp = 0;

	/*
	 * nsecs conversion = (1/ipg_clk) * 10^9
	 * equivalent to 1000 / (ipg_clk / 10^6)
	 */
	usecs = (gd->ipg_clk / 1000000);
	ticks = 1000 / usecs;

	clrbits_le32(PIT_MCR, 2);	/* enable PIT */

	/* ticks per 10 us = 10000 us / usecs = cycles time */
	timerticks = (10 * 1000) / ticks;

	__raw_writel(0xFFFFFFFF, PIT_LDVAL1);
	__raw_writel(0, PIT_TCTRL1);
	__raw_writel(4, PIT_TCTRL1);
	__raw_writel(5, PIT_TCTRL1);
	__raw_writel(timerticks, PIT_LDVAL0);
	__raw_writel(1, PIT_TCTRL0);

	lastinc = __raw_readl(PIT_LTMR64H);

	return 0;
}

ulong get_timer(ulong base)
{
	unsigned long now, diff;

	now = __raw_readl(PIT_LTMR64H);
	diff = -(now - lastinc);
	ltmstamp += diff;
	while (ltmstamp > 100) {
		timestamp++;
		ltmstamp -= 100;
	}
	lastinc = now;

	return timestamp - base;
}

/* delay x useconds AND preserve advance timstamp value */
void __udelay(unsigned long usec)
{
	ulong nsecs, tmp;

	/*
	 * nsecs conversion = (1/ipg_clk) * 10^9
	 * equivalent to 1000 / (ipg_clk / 10^6)
	 */
	if (usec < 5)
		usec = 10;

	nsecs = gd->ipg_clk / 1000000;
	nsecs = 1000 / nsecs;

	/* 1 us per ticks = 1000 ns / nsecs = cycles time */
	while (usec > 0) {
		if (usec > 65000)
			tmp = 65000;
		else
			tmp = usec;
		usec = usec - tmp;

		tmp =  (tmp * 1000) / nsecs;

		__raw_writel(tmp, PIT_LDVAL2);
		__raw_writel(1, PIT_TCTRL2);

		while ((__raw_readl(PIT_TFLG2) & 1) != 1)
			;
		__raw_writel(0, PIT_TCTRL2);
		__raw_writel(1, PIT_TFLG2);
	}
}
#endif			/* CONFIG_TMR_USEPIT */

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On ARM it returns the number of timer ticks per second.
 */
unsigned long long _usec2ticks(unsigned long long usec)
{
	return usec;
}

unsigned long long get_ticks(void)
{
	return get_timer(0);
}

ulong get_tbclk(void)
{
	return CONFIG_SYS_HZ;
}
