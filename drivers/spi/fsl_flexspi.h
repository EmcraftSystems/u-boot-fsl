#ifndef _FSL_FLEXSPI_H_
#define _FSL_FLEXSPI_H_


/* The registers */
#define FLEXSPI_MCR0			0x00
#define FLEXSPI_MCR0_AHB_TIMEOUT_SHIFT	24
#define FLEXSPI_MCR0_AHB_TIMEOUT_MASK	(0xFF << FLEXSPI_MCR0_AHB_TIMEOUT_SHIFT)
#define FLEXSPI_MCR0_IP_TIMEOUT_SHIFT	16
#define FLEXSPI_MCR0_IP_TIMEOUT_MASK	(0xFF << FLEXSPI_MCR0_IP_TIMEOUT_SHIFT)
#define FLEXSPI_MCR0_LEARN_EN_SHIFT	15
#define FLEXSPI_MCR0_LEARN_EN_MASK	(1 << FLEXSPI_MCR0_LEARN_EN_SHIFT)
#define FLEXSPI_MCR0_SCRFRUN_EN_SHIFT	14
#define FLEXSPI_MCR0_SCRFRUN_EN_MASK	(1 << FLEXSPI_MCR0_SCRFRUN_EN_SHIFT)
#define FLEXSPI_MCR0_OCTCOMB_EN_SHIFT	13
#define FLEXSPI_MCR0_OCTCOMB_EN_MASK	(1 << FLEXSPI_MCR0_OCTCOMB_EN_SHIFT)
#define FLEXSPI_MCR0_DOZE_EN_SHIFT	12
#define FLEXSPI_MCR0_DOZE_EN_MASK	(1 << FLEXSPI_MCR0_DOZE_EN_SHIFT)
#define FLEXSPI_MCR0_HSEN_SHIFT		11
#define FLEXSPI_MCR0_HSEN_MASK		(1 << FLEXSPI_MCR0_HSEN_SHIFT)
#define FLEXSPI_MCR0_SERCLKDIV_SHIFT	8
#define FLEXSPI_MCR0_SERCLKDIV_MASK	(7 << FLEXSPI_MCR0_SERCLKDIV_SHIFT)
#define FLEXSPI_MCR0_ATDF_EN_SHIFT	7
#define FLEXSPI_MCR0_ATDF_EN_MASK	(1 << FLEXSPI_MCR0_ATDF_EN_SHIFT)
#define FLEXSPI_MCR0_ARDF_EN_SHIFT	6
#define FLEXSPI_MCR0_ARDF_EN_MASK	(1 << FLEXSPI_MCR0_ARDF_EN_SHIFT)
#define FLEXSPI_MCR0_RXCLKSRC_SHIFT	4
#define FLEXSPI_MCR0_RXCLKSRC_MASK	(3 << FLEXSPI_MCR0_RXCLKSRC_SHIFT)
#define FLEXSPI_MCR0_END_CFG_SHIFT	2
#define FLEXSPI_MCR0_END_CFG_MASK	(3 << FLEXSPI_MCR0_END_CFG_SHIFT)
#define FLEXSPI_MCR0_MDIS_SHIFT		1
#define FLEXSPI_MCR0_MDIS_MASK		(1 << FLEXSPI_MCR0_MDIS_SHIFT)
#define FLEXSPI_MCR0_SWRST_SHIFT	0
#define FLEXSPI_MCR0_SWRST_MASK		(1 << FLEXSPI_MCR0_SWRST_SHIFT)

#define FLEXSPI_MCR1			0x04
#define FLEXSPI_MCR1_SEQ_TIMEOUT_SHIFT	16
#define FLEXSPI_MCR1_SEQ_TIMEOUT_MASK	\
	(0xFFFF << FLEXSPI_MCR1_SEQ_TIMEOUT_SHIFT)
#define FLEXSPI_MCR1_AHB_TIMEOUT_SHIFT	0
#define FLEXSPI_MCR1_AHB_TIMEOUT_MASK	\
	(0xFFFF << FLEXSPI_MCR1_AHB_TIMEOUT_SHIFT)

#define FLEXSPI_MCR2			0x08
#define FLEXSPI_MCR2_IDLE_WAIT_SHIFT	24
#define FLEXSPI_MCR2_IDLE_WAIT_MASK	(0xFF << FLEXSPI_MCR2_IDLE_WAIT_SHIFT)
#define FLEXSPI_MCR2_SAMEFLASH_SHIFT	15
#define FLEXSPI_MCR2_SAMEFLASH_MASK	(1 << FLEXSPI_MCR2_SAMEFLASH_SHIFT)
#define FLEXSPI_MCR2_CLRLRPHS_SHIFT	14
#define FLEXSPI_MCR2_CLRLRPHS_MASK	(1 << FLEXSPI_MCR2_CLRLRPHS_SHIFT)
#define FLEXSPI_MCR2_ABRDATSZ_SHIFT	8
#define FLEXSPI_MCR2_ABRDATSZ_MASK	(1 << FLEXSPI_MCR2_ABRDATSZ_SHIFT)
#define FLEXSPI_MCR2_ABRLEARN_SHIFT	7
#define FLEXSPI_MCR2_ABRLEARN_MASK	(1 << FLEXSPI_MCR2_ABRLEARN_SHIFT)
#define FLEXSPI_MCR2_ABR_READ_SHIFT	6
#define FLEXSPI_MCR2_ABR_READ_MASK	(1 << FLEXSPI_MCR2_ABR_READ_SHIFT)
#define FLEXSPI_MCR2_ABRWRITE_SHIFT	5
#define FLEXSPI_MCR2_ABRWRITE_MASK	(1 << FLEXSPI_MCR2_ABRWRITE_SHIFT)
#define FLEXSPI_MCR2_ABRDUMMY_SHIFT	4
#define FLEXSPI_MCR2_ABRDUMMY_MASK	(1 << FLEXSPI_MCR2_ABRDUMMY_SHIFT)
#define FLEXSPI_MCR2_ABR_MODE_SHIFT	3
#define FLEXSPI_MCR2_ABR_MODE_MASK	(1 << FLEXSPI_MCR2_ABR_MODE_SHIFT)
#define FLEXSPI_MCR2_ABRCADDR_SHIFT	2
#define FLEXSPI_MCR2_ABRCADDR_MASK	(1 << FLEXSPI_MCR2_ABRCADDR_SHIFT)
#define FLEXSPI_MCR2_ABRRADDR_SHIFT	1
#define FLEXSPI_MCR2_ABRRADDR_MASK	(1 << FLEXSPI_MCR2_ABRRADDR_SHIFT)
#define FLEXSPI_MCR2_ABR_CMD_SHIFT	0
#define FLEXSPI_MCR2_ABR_CMD_MASK	(1 << FLEXSPI_MCR2_ABR_CMD_SHIFT)

#define FLEXSPI_AHBCR			0x0c
#define FLEXSPI_AHBCR_RDADDROPT_SHIFT	6
#define FLEXSPI_AHBCR_RDADDROPT_MASK	(1 << FLEXSPI_AHBCR_RDADDROPT_SHIFT)
#define FLEXSPI_AHBCR_PREF_EN_SHIFT	5
#define FLEXSPI_AHBCR_PREF_EN_MASK	(1 << FLEXSPI_AHBCR_PREF_EN_SHIFT)
#define FLEXSPI_AHBCR_BUFF_EN_SHIFT	4
#define FLEXSPI_AHBCR_BUFF_EN_MASK	(1 << FLEXSPI_AHBCR_BUFF_EN_SHIFT)
#define FLEXSPI_AHBCR_CACH_EN_SHIFT	3
#define FLEXSPI_AHBCR_CACH_EN_MASK	(1 << FLEXSPI_AHBCR_CACH_EN_SHIFT)
#define FLEXSPI_AHBCR_CLRTXBUF_SHIFT	2
#define FLEXSPI_AHBCR_CLRTXBUF_MASK	(1 << FLEXSPI_AHBCR_CLRTXBUF_SHIFT)
#define FLEXSPI_AHBCR_CLRRXBUF_SHIFT	1
#define FLEXSPI_AHBCR_CLRRXBUF_MASK	(1 << FLEXSPI_AHBCR_CLRRXBUF_SHIFT)
#define FLEXSPI_AHBCR_PAR_EN_SHIFT	0
#define FLEXSPI_AHBCR_PAR_EN_MASK	(1 << FLEXSPI_AHBCR_PAR_EN_SHIFT)

#define FLEXSPI_INTEN			0x10
#define FLEXSPI_INTEN_SCLKSBWR_SHIFT	9
#define FLEXSPI_INTEN_SCLKSBWR_MASK	(1 << FLEXSPI_INTEN_SCLKSBWR_SHIFT)
#define FLEXSPI_INTEN_SCLKSBRD_SHIFT	8
#define FLEXSPI_INTEN_SCLKSBRD_MASK	(1 << FLEXSPI_INTEN_SCLKSBRD_SHIFT)
#define FLEXSPI_INTEN_DATALRNFL_SHIFT	7
#define FLEXSPI_INTEN_DATALRNFL_MASK	(1 << FLEXSPI_INTEN_DATALRNFL_SHIFT)
#define FLEXSPI_INTEN_IPTXWE_SHIFT	6
#define FLEXSPI_INTEN_IPTXWE_MASK	(1 << FLEXSPI_INTEN_IPTXWE_SHIFT)
#define FLEXSPI_INTEN_IPRXWA_SHIFT	5
#define FLEXSPI_INTEN_IPRXWA_MASK	(1 << FLEXSPI_INTEN_IPRXWA_SHIFT)
#define FLEXSPI_INTEN_AHBCMDERR_SHIFT	4
#define FLEXSPI_INTEN_AHBCMDERR_MASK	(1 << FLEXSPI_INTEN_AHBCMDERR_SHIFT)
#define FLEXSPI_INTEN_IPCMDERR_SHIFT	3
#define FLEXSPI_INTEN_IPCMDERR_MASK	(1 << FLEXSPI_INTEN_IPCMDERR_SHIFT)
#define FLEXSPI_INTEN_AHBCMDGE_SHIFT	2
#define FLEXSPI_INTEN_AHBCMDGE_MASK	(1 << FLEXSPI_INTEN_AHBCMDGE_SHIFT)
#define FLEXSPI_INTEN_IPCMDGE_SHIFT	1
#define FLEXSPI_INTEN_IPCMDGE_MASK	(1 << FLEXSPI_INTEN_IPCMDGE_SHIFT)
#define FLEXSPI_INTEN_IPCMDDONE_SHIFT	0
#define FLEXSPI_INTEN_IPCMDDONE_MASK	(1 << FLEXSPI_INTEN_IPCMDDONE_SHIFT)

#define FLEXSPI_INTR			0x14
#define FLEXSPI_INTR_SCLKSBWR_SHIFT	9
#define FLEXSPI_INTR_SCLKSBWR_MASK	(1 << FLEXSPI_INTR_SCLKSBWR_SHIFT)
#define FLEXSPI_INTR_SCLKSBRD_SHIFT	8
#define FLEXSPI_INTR_SCLKSBRD_MASK	(1 << FLEXSPI_INTR_SCLKSBRD_SHIFT)
#define FLEXSPI_INTR_DATALRNFL_SHIFT	7
#define FLEXSPI_INTR_DATALRNFL_MASK	(1 << FLEXSPI_INTR_DATALRNFL_SHIFT)
#define FLEXSPI_INTR_IPTXWE_SHIFT	6
#define FLEXSPI_INTR_IPTXWE_MASK	(1 << FLEXSPI_INTR_IPTXWE_SHIFT)
#define FLEXSPI_INTR_IPRXWA_SHIFT	5
#define FLEXSPI_INTR_IPRXWA_MASK	(1 << FLEXSPI_INTR_IPRXWA_SHIFT)
#define FLEXSPI_INTR_AHBCMDERR_SHIFT	4
#define FLEXSPI_INTR_AHBCMDERR_MASK	(1 << FLEXSPI_INTR_AHBCMDERR_SHIFT)
#define FLEXSPI_INTR_IPCMDERR_SHIFT	3
#define FLEXSPI_INTR_IPCMDERR_MASK	(1 << FLEXSPI_INTR_IPCMDERR_SHIFT)
#define FLEXSPI_INTR_AHBCMDGE_SHIFT	2
#define FLEXSPI_INTR_AHBCMDGE_MASK	(1 << FLEXSPI_INTR_AHBCMDGE_SHIFT)
#define FLEXSPI_INTR_IPCMDGE_SHIFT	1
#define FLEXSPI_INTR_IPCMDGE_MASK	(1 << FLEXSPI_INTR_IPCMDGE_SHIFT)
#define FLEXSPI_INTR_IPCMDDONE_SHIFT	0
#define FLEXSPI_INTR_IPCMDDONE_MASK	(1 << FLEXSPI_INTR_IPCMDDONE_SHIFT)

#define FLEXSPI_LUTKEY			0x18
#define FLEXSPI_LUTKEY_VALUE		0x5AF05AF0

#define FLEXSPI_LCKCR			0x1C
#define FLEXSPI_LCKER_LOCK		0x1
#define FLEXSPI_LCKER_UNLOCK		0x2

#define FLEXSPI_BUFXCR_INVALID_MSTRID	0xe
#define FLEXSPI_AHBRX_BUF0CR0		0x20
#define FLEXSPI_AHBRX_BUF1CR0		0x24
#define FLEXSPI_AHBRX_BUF2CR0		0x28
#define FLEXSPI_AHBRX_BUF3CR0		0x2C
#define FLEXSPI_AHBRX_BUF4CR0		0x30
#define FLEXSPI_AHBRX_BUF5CR0		0x34
#define FLEXSPI_AHBRX_BUF6CR0		0x38
#define FLEXSPI_AHBRX_BUF7CR0		0x3C
#define FLEXSPI_AHBRXBUF0CR7_PREF_SHIFT	31
#define FLEXSPI_AHBRXBUF0CR7_PREF_MASK	(1 << FLEXSPI_AHBRXBUF0CR7_PREF_SHIFT)

#define FLEXSPI_AHBRX_BUF0CR1		0x40
#define FLEXSPI_AHBRX_BUF1CR1		0x44
#define FLEXSPI_AHBRX_BUF2CR1		0x48
#define FLEXSPI_AHBRX_BUF3CR1		0x4C
#define FLEXSPI_AHBRX_BUF4CR1		0x50
#define FLEXSPI_AHBRX_BUF5CR1		0x54
#define FLEXSPI_AHBRX_BUF6CR1		0x58
#define FLEXSPI_AHBRX_BUF7CR1		0x5C
#define FLEXSPI_BUFXCR1_MSID_SHIFT	0
#define FLEXSPI_BUFXCR1_MSID_MASK	(0xF << FLEXSPI_BUFXCR1_MSID_SHIFT)
#define FLEXSPI_BUFXCR1_PRIO_SHIFT	8
#define FLEXSPI_BUFXCR1_PRIO_MASK	(0x7 << FLEXSPI_BUFXCR1_PRIO_SHIFT)

#define FLEXSPI_FLSHA1CR0		0x60
#define FLEXSPI_FLSHA2CR0		0x64
#define FLEXSPI_FLSHB1CR0		0x68
#define FLEXSPI_FLSHB2CR0		0x6C
#define FLEXSPI_FLSHXCR0_SZ_SHIFT	10
#define FLEXSPI_FLSHXCR0_SZ_MASK	(0x3FFFFF << FLEXSPI_FLSHXCR0_SZ_SHIFT)

#define FLEXSPI_FLSHA1CR1		0x70
#define FLEXSPI_FLSHA2CR1		0x74
#define FLEXSPI_FLSHB1CR1		0x78
#define FLEXSPI_FLSHB2CR1		0x7C
#define FLEXSPI_FLSHXCR1_CSINTR_SHIFT	16
#define FLEXSPI_FLSHXCR1_CSINTR_MASK	\
	(0xFFFF << FLEXSPI_FLSHXCR1_CSINTR_SHIFT)
#define FLEXSPI_FLSHXCR1_CAS_SHIFT	11
#define FLEXSPI_FLSHXCR1_CAS_MASK	(0xF << FLEXSPI_FLSHXCR1_CAS_SHIFT)
#define FLEXSPI_FLSHXCR1_WA_SHIFT	10
#define FLEXSPI_FLSHXCR1_WA_MASK	(1 << FLEXSPI_FLSHXCR1_WA_SHIFT)
#define FLEXSPI_FLSHXCR1_TCSH_SHIFT	5
#define FLEXSPI_FLSHXCR1_TCSH_MASK	(0x1F << FLEXSPI_FLSHXCR1_TCSH_SHIFT)
#define FLEXSPI_FLSHXCR1_TCSS_SHIFT	0
#define FLEXSPI_FLSHXCR1_TCSS_MASK	(0x1F << FLEXSPI_FLSHXCR1_TCSS_SHIFT)

#define FLEXSPI_FLSHA1CR2		0x80
#define FLEXSPI_FLSHA2CR2		0x84
#define FLEXSPI_FLSHB1CR2		0x88
#define FLEXSPI_FLSHB2CR2		0x8C
#define FLEXSPI_FLSHXCR2_CLRINSP_SHIFT	24
#define FLEXSPI_FLSHXCR2_CLRINSP_MASK	(1 << FLEXSPI_FLSHXCR2_CLRINSP_SHIFT)
#define FLEXSPI_FLSHXCR2_AWRWAIT_SHIFT	16
#define FLEXSPI_FLSHXCR2_AWRWAIT_MASK	(0xFF << FLEXSPI_FLSHXCR2_AWRWAIT_SHIFT)
#define FLEXSPI_FLSHXCR2_AWRSEQN_SHIFT	13
#define FLEXSPI_FLSHXCR2_AWRSEQN_MASK	(0x7 << FLEXSPI_FLSHXCR2_AWRSEQN_SHIFT)
#define FLEXSPI_FLSHXCR2_AWRSEQI_SHIFT	8
#define FLEXSPI_FLSHXCR2_AWRSEQI_MASK	(0xF << FLEXSPI_FLSHXCR2_AWRSEQI_SHIFT)
#define FLEXSPI_FLSHXCR2_ARDSEQN_SHIFT	5
#define FLEXSPI_FLSHXCR2_ARDSEQN_MASK	(0x7 << FLEXSPI_FLSHXCR2_ARDSEQN_SHIFT)
#define FLEXSPI_FLSHXCR2_ARDSEQI_SHIFT	0
#define FLEXSPI_FLSHXCR2_ARDSEQI_MASK	(0xF << FLEXSPI_FLSHXCR2_ARDSEQI_SHIFT)

#define FLEXSPI_IPCR0			0xA0

#define FLEXSPI_IPCR1			0xA4
#define FLEXSPI_IPCR1_IPAREN_SHIFT	31
#define FLEXSPI_IPCR1_IPAREN_MASK	(1 << FLEXSPI_IPCR1_IPAREN_SHIFT)
#define FLEXSPI_IPCR1_SEQNUM_SHIFT	24
#define FLEXSPI_IPCR1_SEQNUM_MASK	(0xF << FLEXSPI_IPCR1_SEQNUM_SHIFT)
#define FLEXSPI_IPCR1_SEQID_SHIFT	16
#define FLEXSPI_IPCR1_SEQID_MASK	(0xF << FLEXSPI_IPCR1_SEQID_SHIFT)
#define FLEXSPI_IPCR1_IDATSZ_SHIFT	0
#define FLEXSPI_IPCR1_IDATSZ_MASK	(0xFFFF << FLEXSPI_IPCR1_IDATSZ_SHIFT)

#define FLEXSPI_IPCMD			0xB0
#define FLEXSPI_IPCMD_TRG_SHIFT		0
#define FLEXSPI_IPCMD_TRG_MASK		(1 << FLEXSPI_IPCMD_TRG_SHIFT)

#define FLEXSPI_DLPR			0xB4

#define FLEXSPI_IPRXFCR			0xB8
#define FLEXSPI_IPRXFCR_CLR_SHIFT	0
#define FLEXSPI_IPRXFCR_CLR_MASK	(1 << FLEXSPI_IPRXFCR_CLR_SHIFT)
#define FLEXSPI_IPRXFCR_DMA_EN_SHIFT	1
#define FLEXSPI_IPRXFCR_DMA_EN_MASK	(1 << FLEXSPI_IPRXFCR_DMA_EN_SHIFT)
#define FLEXSPI_IPRXFCR_WMRK_SHIFT	2
#define FLEXSPI_IPRXFCR_WMRK_MASK	(0x1F << FLEXSPI_IPRXFCR_WMRK_SHIFT)

#define FLEXSPI_IPTXFCR			0xBC
#define FLEXSPI_IPTXFCR_CLR_SHIFT	0
#define FLEXSPI_IPTXFCR_CLR_MASK	(1 << FLEXSPI_IPTXFCR_CLR_SHIFT)
#define FLEXSPI_IPTXFCR_DMA_EN_SHIFT	1
#define FLEXSPI_IPTXFCR_DMA_EN_MASK	(1 << FLEXSPI_IPTXFCR_DMA_EN_SHIFT)
#define FLEXSPI_IPTXFCR_WMRK_SHIFT	2
#define FLEXSPI_IPTXFCR_WMRK_MASK	(0x1F << FLEXSPI_IPTXFCR_WMRK_SHIFT)

#define FLEXSPI_STS0			0xE0
#define FLEXSPI_STS0_DLPHA_SHIFT	9
#define FLEXSPI_STS0_DLPHA_MASK		(0x1F << FLEXSPI_STS0_DLPHA_SHIFT)
#define FLEXSPI_STS0_DLPHB_SHIFT	4
#define FLEXSPI_STS0_DLPHB_MASK		(0x1F << FLEXSPI_STS0_DLPHB_SHIFT)
#define FLEXSPI_STS0_CMD_SRC_SHIFT	2
#define FLEXSPI_STS0_CMD_SRC_MASK	(3 << FLEXSPI_STS0_CMD_SRC_SHIFT)
#define FLEXSPI_STS0_ARB_IDLE_SHIFT	1
#define FLEXSPI_STS0_ARB_IDLE_MASK	(1 << FLEXSPI_STS0_ARB_IDLE_SHIFT)
#define FLEXSPI_STS0_SEQ_IDLE_SHIFT	0
#define FLEXSPI_STS0_SEQ_IDLE_MASK	(1 << FLEXSPI_STS0_SEQ_IDLE_SHIFT)

#define FLEXSPI_STS1			0xE4
#define FLEXSPI_STS1_IP_ERRCD_SHIFT	24
#define FLEXSPI_STS1_IP_ERRCD_MASK	(0xF << FLEXSPI_STS1_IP_ERRCD_SHIFT)
#define FLEXSPI_STS1_IP_ERRID_SHIFT	16
#define FLEXSPI_STS1_IP_ERRID_MASK	(0xF << FLEXSPI_STS1_IP_ERRID_SHIFT)
#define FLEXSPI_STS1_AHB_ERRCD_SHIFT	8
#define FLEXSPI_STS1_AHB_ERRCD_MASK	(0xF << FLEXSPI_STS1_AHB_ERRCD_SHIFT)
#define FLEXSPI_STS1_AHB_ERRID_SHIFT	0
#define FLEXSPI_STS1_AHB_ERRID_MASK	(0xF << FLEXSPI_STS1_AHB_ERRID_SHIFT)

#define FLEXSPI_AHBSPNST		0xEC
#define FLEXSPI_AHBSPNST_DATLFT_SHIFT	16
#define FLEXSPI_AHBSPNST_DATLFT_MASK	\
	(0xFFFF << FLEXSPI_AHBSPNST_DATLFT_SHIFT)
#define FLEXSPI_AHBSPNST_BUFID_SHIFT	1
#define FLEXSPI_AHBSPNST_BUFID_MASK	(7 << FLEXSPI_AHBSPNST_BUFID_SHIFT)
#define FLEXSPI_AHBSPNST_ACTIVE_SHIFT	0
#define FLEXSPI_AHBSPNST_ACTIVE_MASK	(1 << FLEXSPI_AHBSPNST_ACTIVE_SHIFT)

#define FLEXSPI_IPRXFSTS		0xF0
#define FLEXSPI_IPRXFSTS_RDCNTR_SHIFT	16
#define FLEXSPI_IPRXFSTS_RDCNTR_MASK	\
	(0xFFFF << FLEXSPI_IPRXFSTS_RDCNTR_SHIFT)
#define FLEXSPI_IPRXFSTS_FILL_SHIFT	0
#define FLEXSPI_IPRXFSTS_FILL_MASK	(0xFF << FLEXSPI_IPRXFSTS_FILL_SHIFT)

#define FLEXSPI_IPTXFSTS		0xF4
#define FLEXSPI_IPTXFSTS_WRCNTR_SHIFT	16
#define FLEXSPI_IPTXFSTS_WRCNTR_MASK	\
	(0xFFFF << FLEXSPI_IPTXFSTS_WRCNTR_SHIFT)
#define FLEXSPI_IPTXFSTS_FILL_SHIFT	0
#define FLEXSPI_IPTXFSTS_FILL_MASK	(0xFF << FLEXSPI_IPTXFSTS_FILL_SHIFT)

#define FLEXSPI_RFDR			0x100
#define FLEXSPI_TFDR			0x180

#define FLEXSPI_LUT_BASE		0x200

/* register map end */

/*
 * The definition of the LUT register shows below:
 *
 *  ---------------------------------------------------
 *  | INSTR1 | PAD1 | OPRND1 | INSTR0 | PAD0 | OPRND0 |
 *  ---------------------------------------------------
 */
#define OPRND0_SHIFT		0
#define PAD0_SHIFT		8
#define INSTR0_SHIFT		10
#define OPRND1_SHIFT		16

/* Instruction set for the LUT register. */

#define LUT_STOP		0x00
#define LUT_CMD		0x01
#define LUT_ADDR		0x02
#define LUT_CADDR_SDR		0x03
#define LUT_MODE		0x04
#define LUT_MODE2		0x05
#define LUT_MODE4		0x06
#define LUT_MODE8		0x07
#define LUT_FSL_WRITE		0x08
#define LUT_FSL_READ		0x09
#define LUT_LEARN_SDR		0x0A
#define LUT_DATSZ_SDR		0x0B
#define LUT_DUMMY		0x0C
#define LUT_DUMMY_RWDS_SDR	0x0D
#define LUT_JMP_ON_CS		0x1F
#define LUT_CMD_DDR		0x21
#define LUT_ADDR_DDR		0x22
#define LUT_CADDR_DDR		0x23
#define LUT_MODE_DDR		0x24
#define LUT_MODE2_DDR		0x25
#define LUT_MODE4_DDR		0x26
#define LUT_MODE8_DDR		0x27
#define LUT_WRITE_DDR		0x28
#define LUT_READ_DDR		0x29
#define LUT_LEARN_DDR		0x2A
#define LUT_DATSZ_DDR		0x2B
#define LUT_DUMMY_DDR		0x2C
#define LUT_DUMMY_RWDS_DDR	0x2D

/*
 * The PAD definitions for LUT register.
 *
 * The pad stands for the lines number of IO[0:3].
 * For example, the Quad read need four IO lines, so you should
 * set LUT_PAD4 which means we use four IO lines.
 */
#define LUT_PAD1		0
#define LUT_PAD2		1
#define LUT_PAD4		2
#define LUT_PAD8		3

/* Oprands for the LUT register. */
#define ADDR24BIT		0x18
#define ADDR32BIT		0x20

/* Macros for constructing the LUT register. */
#define LUT0(ins, pad, opr)						\
		(((opr) << OPRND0_SHIFT) | ((LUT_##pad) << PAD0_SHIFT) | \
		((LUT_##ins) << INSTR0_SHIFT))

#define LUT1(ins, pad, opr)	(LUT0(ins, pad, opr) << OPRND1_SHIFT)

/* other macros for LUT register. */
#define FLEXSPI_LUT(x)          (FLEXSPI_LUT_BASE + (x) * 4)
#define FLEXSPI_LUT_NUM		64


#endif /* _FSL_FLEXSPI_H_ */