/*
 * Copyright 2012 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <common.h>
#include <watchdog.h>
#include <asm/io.h>
#include <asm/arch/vybrid-regs.h>
#include <asm/arch/serial-vybrid.h>
#include <asm/arch/clock.h>

#ifndef CONFIG_VYBRID_UART_BASE
#error "define CONFIG_VYBRID_UART_BASE to use the VYBRID UART driver"
#endif

#define UART_CONSOLE	\
		(CONFIG_VYBRID_UART_BASE + (CONFIG_SYS_UART_PORT * 0x1000))

#ifdef CONFIG_SERIAL_MULTI
#warning "Vybrid driver does not support MULTI serials."
#endif

DECLARE_GLOBAL_DATA_PTR;

void serial_setbrg(void)
{
	u32 clk = vybrid_get_uartclk();
	u16 sbr;

	if (!gd->baudrate)
		gd->baudrate = CONFIG_BAUDRATE;

	sbr = (u16)(clk / (16 * gd->baudrate));
	/* place adjustment later - n/32 BRFA */

	out_8((UART_CONSOLE + UBDH), (sbr >> 8));
	out_8((UART_CONSOLE + UBDL), (sbr & 0xFF));
}

int serial_getc(void)
{
	while (!(in_8(UART_CONSOLE + US1) & US1_RDRF))
		WATCHDOG_RESET();

	setbits_8((UART_CONSOLE + US1), US1_RDRF);

	return in_8(UART_CONSOLE + UD);
}

void serial_putc(const char c)
{
	if (c == '\n')
		serial_putc('\r');

	while (!(in_8(UART_CONSOLE + US1) & US1_TDRE))
		WATCHDOG_RESET();

	out_8((UART_CONSOLE + UD), c);
}

/*
 * Test whether a character is in the RX buffer
 */
int serial_tstc(void)
{
	if (in_8(UART_CONSOLE + URCFIFO) == 0)
		return 0;

	return 1;
}

void
serial_puts(const char *s)
{
	while (*s)
		serial_putc(*s++);
}

/*
 * Initialise the serial port with the given baudrate. The settings
 * are always 8 data bits, no parity, 1 stop bit, no start bits.
 */
int serial_init(void)
{
	clrbits_8((UART_CONSOLE + UC2), UC2_RE);
	clrbits_8((UART_CONSOLE + UC2), UC2_TE);

	out_8((UART_CONSOLE + UMODEM), 0);
	out_8((UART_CONSOLE + UC1), 0);

	/* provide data bits, parity, stop bit, etc */

	serial_setbrg();

	out_8((UART_CONSOLE + UC2), (UC2_RE | UC2_TE));

	return 0;
}
