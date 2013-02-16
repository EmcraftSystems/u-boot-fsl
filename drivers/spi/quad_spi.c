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
#include <spi.h>
#include <malloc.h>
#include <asm/arch/quadspi.h>

struct quadspi_slave {
	struct spi_slave slave;
	uint baudrate;
	int charbit;
};

int quadspi_xfer(struct spi_slave *slave, uint bitlen, const void *dout,
	       void *din, ulong flags);
struct spi_slave *quadspi_setup_slave(struct quadspi_slave *slave, uint mode);
void quadspi_init(void);
void quadspi_tx(u32 ctrl, u16 data);
u16 quadspi_rx(void);

extern void setup_iomux_quadspi(void);
extern int quadspi_claim_bus(uint bus, uint cs);
extern void quadspi_release_bus(uint bus, uint cs);

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_QUAD_SPI
void quadspi_init(void)
{
	setup_iomux_quadspi();	/* port configuration */
}

void quadspi_tx(u32 ctrl, u16 data)
{
}

u16 quadspi_rx(void)
{
	return 0;
}

int quadspi_xfer(struct spi_slave *slave, uint bitlen, const void *dout,
	       void *din, ulong flags)
{
	return 0;
}

struct spi_slave *quadspi_setup_slave(struct quadspi_slave *slave, uint mode)
{
	return 0;
}
#endif				/* CONFIG_QUAD_SPI */

#ifdef CONFIG_CMD_SPI
int spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
	return 0;
}

void spi_init_f(void)
{
}

void spi_init_r(void)
{
}

void spi_init(void)
{
	quadspi_init();
}

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
				  unsigned int max_hz, unsigned int mode)
{
	struct quadspi_slave *slave;

	if (!spi_cs_is_valid(bus, cs))
		return NULL;

	slave = malloc(sizeof(struct quadspi_slave));
	if (!slave)
		return NULL;

	slave->slave.bus = bus;
	slave->slave.cs = cs;
	slave->baudrate = max_hz;

	/* specific setup */
	return quadspi_setup_slave(slave, mode);
}

void spi_free_slave(struct spi_slave *slave)
{
	free(slave);
}

int spi_claim_bus(struct spi_slave *slave)
{
	return quadspi_claim_bus(slave->bus, slave->cs);
}

void spi_release_bus(struct spi_slave *slave)
{
	quadspi_release_bus(slave->bus, slave->cs);
}

int spi_xfer(struct spi_slave *slave, unsigned int bitlen, const void *dout,
	     void *din, unsigned long flags)
{
	return quadspi_xfer(slave, bitlen, dout, din, flags);
}
#endif				/* CONFIG_CMD_SPI */
