/*
 *  (C) Copyright 2018 Emcraft Systems
 *
 * Freescale Flex Serial Peripheral Interface (FlexSPI) driver
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <spi.h>
#include <asm/io.h>
#include <linux/sizes.h>
#include <dm.h>
#include <errno.h>
#include <watchdog.h>
#include "fsl_flexspi.h"
#include <mapmem.h>

#if defined (CONFIG_SPI_FLASH_ISSI)
#define CONFIG_FLASH_READ_CMD		SPINOR_OP_READ_1_4_4
#define CONFIG_FLASH_DUMMY_CYCLES	6
#define CONFIG_FLASH_PP_CMD		0x02
#endif

#define AHB_BUF_SIZE			1024

DECLARE_GLOBAL_DATA_PTR;

#define OFFSET_BITS_MASK	GENMASK(23, 0)

#define FLASH_STATUS_WEL	0x02

/**
 * struct fsl_flexspi_priv - private data for Freescale FlexSPI
 *
 * @cur_seqid: current LUT table sequence id
 * @sf_addr: flash access offset
 * @iobase: bash address of the FlexSPI registers
 * @flashbase: SPI-NOR flash map address
 */
struct fsl_flexspi_priv {
	u32 cur_seqid;
	u32 sf_addr;

	void *iobase;
	void *flashbase;
};

#ifndef CONFIG_DM_SPI
struct fsl_flexspi {
	struct spi_slave slave;
	struct fsl_flexspi_priv priv;
};
#endif

static inline void fsl_flexspi_unlock_lut(struct fsl_flexspi_priv *flex)
{
	writel(FLEXSPI_LUTKEY_VALUE, flex->iobase + FLEXSPI_LUTKEY);
	writel(FLEXSPI_LCKER_UNLOCK, flex->iobase + FLEXSPI_LCKCR);
}

static inline void fsl_flexspi_lock_lut(struct fsl_flexspi_priv *flex)
{
	writel(FLEXSPI_LUTKEY_VALUE, flex->iobase + FLEXSPI_LUTKEY);
	writel(FLEXSPI_LCKER_LOCK, flex->iobase + FLEXSPI_LCKCR);
}


/* SEQID -- we can have 16 seqids at most. */
#define SEQID_QUAD_READ		0
#define SEQID_WREN		1
#define SEQID_RDSR		2
#define SEQID_SE		3
#define SEQID_PP		4
#define SEQID_RDID		5
#define SEQID_WRSR		6

/* SPI NOR command codes */
#define SPINOR_OP_PP		0x02	/* Page program (up to 256 bytes) */
#define SPINOR_OP_RDSR		0x05	/* Read status register */
#define SPINOR_OP_WRSR		0x01	/* Write status register 1 byte */
#define SPINOR_OP_WREN		0x06	/* Write enable */
#define SPINOR_OP_READ_1_4_4	0xeb	/* Read data bytes (Quad I/0 SPI) */
#define SPINOR_OP_READ_1_1_4	0x6b	/* Read data bytes (Quad SPI) */
#define SPINOR_OP_SE		0xd8	/* Sector erase (usually 64KiB) */
#define SPINOR_OP_RDID		0x9f	/* Read JEDEC ID */

static void flexspi_set_lut(struct fsl_flexspi_priv *priv)
{
	void *base = priv->iobase;
	int rxfifo = CONFIG_FSL_FLEXSPI_RXFIFO_SIZE;
	u8 addrlen = ADDR24BIT;
	u32 lut_base;
	int i;

	fsl_flexspi_unlock_lut(priv);

	/* Clear all the LUT table */
	for (i = 0; i < FLEXSPI_LUT_NUM; i++)
		writel(0, base + FLEXSPI_LUT_BASE + i * 4);

	/* Quad Read */
	lut_base = SEQID_QUAD_READ * 4;
	writel(LUT0(CMD, PAD1, CONFIG_FLASH_READ_CMD) | LUT1(ADDR, PAD4, addrlen),
	       base + FLEXSPI_LUT(lut_base));

	writel(LUT0(DUMMY, PAD4, CONFIG_FLASH_DUMMY_CYCLES) |
	       LUT1(FSL_READ, PAD4, rxfifo),
	       base + FLEXSPI_LUT(lut_base + 1));

	/* Page Program */
	lut_base = SEQID_PP * 4;
	writel(LUT0(CMD, PAD1, CONFIG_FLASH_PP_CMD) | LUT1(ADDR, PAD1, addrlen),
			base + FLEXSPI_LUT(lut_base));
	writel(LUT0(FSL_WRITE, PAD1, 0), base + FLEXSPI_LUT(lut_base + 1));

	/* Write enable */
	lut_base = SEQID_WREN * 4;
	writel(LUT0(CMD, PAD1, SPINOR_OP_WREN), base + FLEXSPI_LUT(lut_base));

	/* Erase a sector */
	lut_base = SEQID_SE * 4;
	writel(LUT0(CMD, PAD1, SPINOR_OP_SE) | LUT1(ADDR, PAD1, addrlen),
			base + FLEXSPI_LUT(lut_base));

	/* Read Status */
	lut_base = SEQID_RDSR * 4;
	writel(LUT0(CMD, PAD1, SPINOR_OP_RDSR) | LUT1(FSL_READ, PAD1, 0x1),
			base + FLEXSPI_LUT(lut_base));

	/* READ ID */
	lut_base = SEQID_RDID * 4;
	writel(LUT0(CMD, PAD1, SPINOR_OP_RDID) | LUT1(FSL_READ, PAD1, 0x8),
			base + FLEXSPI_LUT(lut_base));

	/* Write Register */
	lut_base = SEQID_WRSR * 4;
	writel(LUT0(CMD, PAD1, SPINOR_OP_WRSR) | LUT1(FSL_WRITE, PAD1, 0x2),
			base + FLEXSPI_LUT(lut_base));

	fsl_flexspi_lock_lut(priv);
}

/* Get the SEQID for the command */
static int fsl_flexspi_get_seqid(u8 cmd)
{

	switch (cmd) {
	case SPINOR_OP_PP:
		return SEQID_PP;
	case SPINOR_OP_SE:
		return SEQID_SE;
	case SPINOR_OP_WREN:
		return SEQID_WREN;
	case SPINOR_OP_READ_1_1_4:
	case SPINOR_OP_READ_1_4_4:
		return SEQID_QUAD_READ;
	case SPINOR_OP_RDSR:
		return SEQID_RDSR;
	case SPINOR_OP_RDID:
		return SEQID_RDID;
	case SPINOR_OP_WRSR:
		return SEQID_WRSR;
	default:
		printf("FlexSPI: Unsupported cmd 0x%.2x\n", cmd);
		break;
	}
	return -EINVAL;
}

/* Read out the data from the FLEXSPI_RBDR buffer registers. */
static void fsl_flexspi_read_data(struct fsl_flexspi_priv *flex, int len, u8 *rxbuf)
{
	/* u64 tmp; */
	int i = 0;
	int size;

	/* invalid RXFIFO first */
	writel(FLEXSPI_IPRXFCR_CLR_MASK, flex->iobase + FLEXSPI_IPRXFCR);
	while (len > 0) {

		size = len / 8;

		for (i = 0; i < size; ++i) {
			/* Wait for RXFIFO available*/
			while (!(readl(flex->iobase + FLEXSPI_INTR)
				 & FLEXSPI_INTR_IPRXWA_MASK))
				;

			/* read 64 bit data once */
			memcpy(rxbuf, (void *)(flex->iobase + FLEXSPI_RFDR), 8);
			rxbuf += 8;

			/* move the FIFO pointer */
			writel(FLEXSPI_INTR_IPRXWA_MASK,
			       flex->iobase + FLEXSPI_INTR);
			len -= 8;
		}

		size = len % 8;

		if (size) {
			/* Wait for RXFIFO available*/
			while (!(readl(flex->iobase + FLEXSPI_INTR)
				 & FLEXSPI_INTR_IPRXWA_MASK))
				;

			memcpy(rxbuf, (void*)(flex->iobase + FLEXSPI_RFDR), size);
			len -= size;
		}

		writel(FLEXSPI_INTR_IPRXWA_MASK,
		       flex->iobase + FLEXSPI_INTR);

		/* invalid the RXFIFO */
		writel(FLEXSPI_IPRXFCR_CLR_MASK,
		       flex->iobase + FLEXSPI_IPRXFCR);
	}
}

/*
 * If we have changed the content of the flash by writing or erasing,
 * we need to invalidate the AHB buffer. If we do not do so, we may read out
 * the wrong data. The spec tells us reset the AHB domain and Serial Flash
 * domain at the same time.
 */
static inline void fsl_flexspi_invalid(struct fsl_flexspi_priv *flex)
{
	u32 reg;

	reg = readl(flex->iobase + FLEXSPI_MCR0);
	writel(reg | FLEXSPI_MCR0_SWRST_MASK, flex->iobase + FLEXSPI_MCR0);

	/*
	 * The minimum delay : 1 AHB + 2 SFCK clocks.
	 * Delay 1 us is enough.
	 */
	while (readl(flex->iobase + FLEXSPI_MCR0) & FLEXSPI_MCR0_SWRST_MASK)
		;

}

static int
fsl_flexspi_runcmd(struct fsl_flexspi_priv *flex, unsigned int addr, int len)
{
	void *base = flex->iobase;
	int seqnum = 0;
	u32 reg;
	int err;

	if (flex->cur_seqid == -1)
		return -1;


	/* write address */
	writel(addr, base + FLEXSPI_IPCR0);

	writel((seqnum << FLEXSPI_IPCR1_SEQNUM_SHIFT) |
			(flex->cur_seqid << FLEXSPI_IPCR1_SEQID_SHIFT) | len,
			base + FLEXSPI_IPCR1);

	/* wait till controller is idle */
	do {
		reg = readl(base + FLEXSPI_STS0);
		if ((reg & FLEXSPI_STS0_ARB_IDLE_MASK) &&
		    (reg & FLEXSPI_STS0_SEQ_IDLE_MASK))
			break;
		udelay(1);
	} while (1);

	/* trigger the LUT now */
	writel(1, base + FLEXSPI_IPCMD);

	/* Wait for completion. */
	do {
		reg = readl(base + FLEXSPI_STS0);
		if ((reg & FLEXSPI_STS0_ARB_IDLE_MASK) &&
		    (reg & FLEXSPI_STS0_SEQ_IDLE_MASK))
			break;
		udelay(1);
	} while (1);

	return err;
}


static ssize_t fsl_flexspi_write(struct fsl_flexspi_priv *flex,
				     unsigned int to, u32 *txbuf,
				     unsigned int count)
{
	int ret, i;
	int size;

	/* clear the TX FIFO. */
	writel(FLEXSPI_IPTXFCR_CLR_MASK, flex->iobase + FLEXSPI_IPTXFCR);

	size = count / 8;
	for (i = 0; i < size; i++) {
		/* Wait for TXFIFO empty*/
		while (!(readl(flex->iobase + FLEXSPI_INTR)
			 & FLEXSPI_INTR_IPTXWE_MASK))
			;

		memcpy((void *)(flex->iobase + FLEXSPI_TFDR), txbuf, 8);
		txbuf += 2;
		writel(FLEXSPI_INTR_IPTXWE_MASK, flex->iobase + FLEXSPI_INTR);
	}

	size = count % 8;
	if (size) {
		/* Wait for TXFIFO empty*/
		while (!(readl(flex->iobase + FLEXSPI_INTR)
			 & FLEXSPI_INTR_IPTXWE_MASK))
			;

		memcpy((void *)(flex->iobase + FLEXSPI_TFDR), txbuf, size);
		writel(FLEXSPI_INTR_IPTXWE_MASK, flex->iobase + FLEXSPI_INTR);
	}

	/* Trigger it */
	ret = fsl_flexspi_runcmd(flex, to, count);

	if (ret == 0)
		return count;

	fsl_flexspi_invalid(flex);

	return ret;
}

static int flexspi_xfer(struct fsl_flexspi_priv *priv, unsigned int bitlen,
		const void *dout, void *din, unsigned long flags)
{
	u32 bytes = DIV_ROUND_UP(bitlen, 8);
	u32 txbuf;
	u8 cmd = 0;

	if (dout) {
		if (flags & SPI_XFER_BEGIN) {
			cmd = *(u8 *)dout;
			memcpy(&txbuf, dout, 4);
			priv->cur_seqid = fsl_flexspi_get_seqid(cmd);
			priv->sf_addr = swab32(txbuf) & OFFSET_BITS_MASK;
			if (priv->cur_seqid == -1) {
				return -1;
			}
			if (priv->cur_seqid == SEQID_WREN ||
				priv->cur_seqid == SEQID_WRSR) {
				fsl_flexspi_runcmd(priv, 0, 0);
			}
			if (priv->cur_seqid == SEQID_SE) {
				fsl_flexspi_runcmd(priv, priv->sf_addr, 0);
				fsl_flexspi_invalid(priv);
			}
		}
		if (flags == SPI_XFER_END) {
			if (priv->cur_seqid == SEQID_PP ||
				priv->cur_seqid == SEQID_WRSR) {
				fsl_flexspi_write(priv, priv->sf_addr, (u32 *)dout, bytes);
			}
			return 0;
		}
	}

	if (din) {
		if (priv->cur_seqid == SEQID_RDID ||
		    priv->cur_seqid == SEQID_RDSR) {
			fsl_flexspi_runcmd(priv, 0, bytes);
			fsl_flexspi_read_data(priv, bytes, din);
		} else if (priv->cur_seqid == SEQID_QUAD_READ) {
			memcpy(din, priv->flashbase + priv->sf_addr, bytes);
		}
	}

	return 0;
}

#ifndef CONFIG_DM_SPI
static inline struct fsl_flexspi *to_flexspi_spi(struct spi_slave *slave)
{
	return container_of(slave, struct fsl_flexspi, slave);
}

/*
 * There are two different ways to read out the data from the flash:
 *  the "IP Command Read" and the "AHB Command Read".
 *
 * The IC guy suggests we use the "AHB Command Read" which is faster
 * then the "IP Command Read". (What's more is that there is a bug in
 * the "IP Command Read" in the Vybrid.)
 *
 * After we set up the registers for the "AHB Command Read", we can use
 * the memcpy to read the data directly. A "missed" access to the buffer
 * causes the controller to clear the buffer, and use the sequence pointed
 * by the FLEXSPI_BFGENCR[SEQID] to initiate a read from the flash.
 */
static void fsl_flexspi_init_ahb_read(struct fsl_flexspi *flex)
{
	void *base = flex->priv.iobase;
	/* u32 reg, reg2; */
	int seqid;
	int i;

	/* AHB configuration for access buffer 0/1/2 .*/
	for (i = 0; i < 7; i++) {
		writel(0, base + FLEXSPI_AHBRX_BUF0CR0 + 4 * i);
	}

	/*
	 * Set ADATSZ with the maximum AHB buffer size to improve the
	 * read performance.
	 */
	writel((AHB_BUF_SIZE / 8 | FLEXSPI_AHBRXBUF0CR7_PREF_MASK),
	       base + FLEXSPI_AHBRX_BUF7CR0);

	/* prefetch and no start address alignment limitation */
	writel(FLEXSPI_AHBCR_PREF_EN_MASK | FLEXSPI_AHBCR_RDADDROPT_MASK,
	       base + FLEXSPI_AHBCR);

	/* Set the default lut sequence for AHB Read. */
	seqid = fsl_flexspi_get_seqid(SPINOR_OP_READ_1_4_4);
	writel(seqid, base + FLEXSPI_FLSHA1CR2);
}

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int mode)
{
	struct fsl_flexspi *flexspi;
	void *base;
#if !defined(CONFIG_SPI_BOOT)
	u32 reg;
#endif

	flexspi = spi_alloc_slave(struct fsl_flexspi, bus, cs);
	if (!flexspi)
		return NULL;

	flexspi->priv.iobase = map_sysmem(CONFIG_FSL_FLEXSPI_IOBASE, CONFIG_FSL_FLEXSPI_IOSIZE);
	flexspi->priv.flashbase = map_sysmem(CONFIG_FSL_FLEXSPI_FLASHBASE, CONFIG_FSL_FLEXSPI_FLASHSIZE);
	flexspi->slave.max_write_size = CONFIG_FSL_FLEXSPI_TXFIFO_SIZE;
	flexspi->slave.mode |= SPI_RX_QUAD;


	base = flexspi->priv.iobase;

#if !defined(CONFIG_SPI_BOOT)
	/* Reset the module */
	writel(FLEXSPI_MCR0_SWRST_MASK, base + FLEXSPI_MCR0);
	do {
		udelay(1);
	} while (0x1 & readl(base + FLEXSPI_MCR0));

	/* Disable the module */
	writel(FLEXSPI_MCR0_MDIS_MASK, base + FLEXSPI_MCR0);

	/* enable module */
	writel(FLEXSPI_MCR0_AHB_TIMEOUT_MASK | FLEXSPI_MCR0_IP_TIMEOUT_MASK |
	       FLEXSPI_MCR0_OCTCOMB_EN_MASK, base + FLEXSPI_MCR0);

	/* Read the register value */
	reg = readl(base + FLEXSPI_MCR0);
#endif
	flexspi_set_lut(&flexspi->priv);

	fsl_flexspi_init_ahb_read(flexspi);

	return &flexspi->slave;
}

void spi_free_slave(struct spi_slave *slave)
{
	struct fsl_flexspi *flexspi = to_flexspi_spi(slave);
	unmap_sysmem(flexspi->priv.iobase);
	unmap_sysmem(flexspi->priv.flashbase);

	free(flexspi);
}

int spi_claim_bus(struct spi_slave *slave)
{
	return 0;
}

void spi_release_bus(struct spi_slave *slave)
{
	/* Nothing to do */
}

int spi_xfer(struct spi_slave *slave, unsigned int bitlen,
		const void *dout, void *din, unsigned long flags)
{
	struct fsl_flexspi *flexspi = to_flexspi_spi(slave);

	return flexspi_xfer(&flexspi->priv, bitlen, dout, din, flags);
}

void spi_init(void)
{
	/* Nothing to do */
}
#else
/* DM not supported yet */
#endif
