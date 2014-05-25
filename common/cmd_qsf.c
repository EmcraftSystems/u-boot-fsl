/*
 * Command for accessing SPI flash.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <malloc.h>

#include <spi_flash.h>
#include <asm/io.h>
#include <asm/arch/iomux.h>

#define RX_BUFFER_SIZE (32 << 2)
#define QSPI_IPCR_SEQID_SHIFT	(24)
#define QSPI_MCR_CLR_TXF_SHIFT	(11)
#define QSPI_MCR_CLR_TXF_MASK	((1) << (QSPI_MCR_CLR_TXF_SHIFT))

#define QSPI_MCR_CLR_RXF_SHIFT	(10)
#define QSPI_MCR_CLR_RXF_MASK	((1) << (QSPI_MCR_CLR_RXF_SHIFT))

static struct spi_flash *flash = NULL;
static int swap_data = 0;

struct QSPI_tag { 
    union { 
        u32 R; 
        struct { 
           u32 SWRSTSD:1; 
           u32 SWRSTHD:1; 
           u32:4; 
           u32 DQS_EN:1; 
           u32 DDR_EN:1; 
           u32:2; 
           u32 CLR_RXF:1; 
           u32 CLR_TXF:1; 
           u32:2; 
           u32 MDIS:1; 
           u32 DOZE:1; 
           u32:16; 
        } B; 
    } MCR;				/*00000000*/ 
 
    uint8_t QSPI_reserved0[4]; 
 
    union { 
        u32 R; 
        struct { 
           u32 IDATSZ:16; 
           u32 PAR_EN:1; 
           u32:7; 
           u32 SEQID:4; 
           u32:4; 
        } B; 
    } IPCR;				/*00000008*/ 
 
    union { 
        u32 R; 
        struct { 
           u32 TCSS:4; 
           u32:4; 
           u32 TCSH:4; 
           u32:20; 
        } B; 
    } FLSHCR;				/*0000000C*/ 
 
    union { 
        u32 R; 
        struct { 
           u32 MSTRID:4; 
           u32:4; 
           u32 ADATSZ:8; 
           u32:15;  
           u32 HP_EN:1; 
        } B; 
    } BUF0CR;				/*00000010*/ 
 
    union { 
        u32 R; 
        struct { 
           u32 MSTRID:4; 
           u32:4; 
           u32 ADATSZ:8; 
           u32:16; 
        } B; 
    } BUF1CR;				/*00000014*/ 
 
    union { 
        u32 R; 
        struct { 
           u32 MSTRID:4; 
           u32:4; 
           u32 ADATSZ:8; 
           u32:16; 
        } B; 
    } BUF2CR;				/*00000018*/ 
 
    union { 
        u32 R; 
        struct { 
           u32 MSTRID:4; 
           u32:4; 
           u32 ADATSZ:8; 
           u32:15; 
           u32 ALLMST:1; 
        } B; 
    } BUF3CR;				/*0000001C*/ 
 
    union { 
        u32 R; 
        struct { 
           u32:12; 
           u32 SEQID:4; 
           u32 PAR_EN:1; 
           u32:15; 
        } B; 
    } BFGENCR;				/*00000020*/ 
 
    union { 
        u32 R; 
        struct { 
           u32:12; 
           u32 SOCCFG:4; 
           u32:16; 
        } B; 
    } SOCCR;				/*00000024*/ 
 
    uint8_t QSPI_reserved1[8]; 
 
    union { 
        u32 R; 
        struct { 
           u32:3; 
           u32 TPADD0:29; 
        } B; 
    } BUF0IND;				/*00000030*/ 
 
    union { 
        u32 R; 
        struct { 
           u32:3; 
           u32 TPADD0:29; 
        } B; 
    } BUF1IND;				/*00000034*/ 
 
    union { 
        u32 R; 
        struct { 
           u32:3; 
           u32 TPADD0:29; 
        } B; 
    } BUF2IND;				/*00000038*/ 
 
    uint8_t QSPI_reserved2[196]; 
 
    union { 
        u32 R; 
        struct { 
           u32 SFADR:32; 
        } B; 
    } SFAR;				/*00000100*/ 
 
    uint8_t QSPI_reserved3[4]; 
 
    union { 
        u32 R; 
        struct { 
           u32 HSENA:1; 
           u32 HSPHS:1; 
           u32 HSDLY:1; 
           u32:2; 
           u32 FSPHS:1; 
           u32 FSDLY:1; 
           u32:9; 
           u32 DDRSMP:3; 
           u32:13; 
        } B; 
    } SMPR;				/*00000108*/ 
 
    union { 
        u32 R; 
        struct { 
           u32:8; 
           u32 RDBFL:6; 
           u32:2; 
           u32 RDCTR:16; 
        } B; 
    } RBSR;				/*0000010C*/ 
 
    union { 
        u32 R; 
        struct { 
           u32 WMRK:5; 
           u32:3; 
           u32 RXBRD:1; 
           u32:23; 
        } B; 
    } RBCT;				/*00000110*/ 
 
    uint8_t QSPI_reserved4[60]; 
 
    union { 
        u32 R; 
        struct { 
           u32:8; 
           u32 TRBFL:5; 
           u32:3; 
           u32 TRCTR:16; 
        } B; 
    } TBSR;				/*00000150*/ 
 
    union { 
        u32 R; 
        struct { 
           u32 TXDATA:32; 
        } B; 
    } TBDR;				/*00000154*/ 
 
    uint8_t QSPI_reserved5[4]; 
 
    union { 
        u32 R; 
        struct { 
           u32 BUSY:1; 
           u32 IP_ACC:1; 
           u32 AHB_ACC:1; 
           u32:2; 
           u32 AHBGNT:1; 
           u32 AHBTRN:1; 
           u32 AHB0NE:1; 
           u32 AHB1NE:1; 
           u32 AHB2NE:1; 
           u32 AHB3NE:1; 
           u32 AHB0FULL:1; 
           u32 AHB1FULL:1; 
           u32 AHB2FULL:1; 
           u32 AHB3FULL:1; 
           u32:1; 
           u32 RXWE:1; 
           u32:2; 
           u32 RXFULL:1; 
           u32:3; 
           u32 RXDMA:1; 
           u32 TXNE:1; 
           u32:2; 
           u32 TXFULL:1; 
           u32:1; 
           u32 DLPSMP:3; 
        } B; 
    } SR;				/*0000015C*/ 
 
    union { 
        u32 R; 
        struct { 
           u32 TFF:1; 
           u32:3; 
           u32 IPGEF:1; 
           u32:1; 
           u32 IPIEF:1; 
           u32 IPAEF:1; 
           u32:3; 
           u32 IUEF:1; 
           u32 ABOF:1; 
           u32:2; 
           u32 ABSEF:1; 
           u32 RBDF:1; 
           u32 RBOF:1; 
           u32:5; 
           u32 ILLINE:1; 
           u32:2; 
           u32 TBUF:1; 
           u32 TBFF:1; 
           u32:3; 
           u32 DLPFF:1; 
        } B; 
    } FR;				/*00000160*/ 
 
    union { 
        u32 R; 
        struct { 
           u32 TFIE:1; 
           u32:3; 
           u32 IPGEIE:1; 
           u32:1; 
           u32 IPIEIE:1; 
           u32 IPAEIE:1; 
           u32:3; 
           u32 IUEIE:1; 
           u32 ABOIE:1; 
           u32:2; 
           u32 ABSEIE:1; 
           u32 RBDIE:1; 
           u32 RBOIE:1; 
           u32:3; 
           u32 RBDDE:1; 
           u32:1; 
           u32 ILLINE:1; 
           u32:2; 
           u32 TBUIE:1; 
           u32 TBFIE:1; 
           u32:3; 
           u32 DLPFIE:1; 
        } B; 
    } RSER;				/*00000164*/ 
 
    union { 
        u32 R; 
        struct { 
           u32 SUSPND:1; 
           u32:5; 
           u32 SPDBUF:2; 
           u32:3; 
           u32 DATLFT:5; 
           u32:16; 
        } B; 
    } SPNDST;				/*00000168*/ 
 
    union { 
        u32 R; 
        struct { 
           u32 BFPTRC:1; 
           u32:7; 
           u32 IPPTRC:1; 
           u32:23; 
        } B; 
    } SPTRCLR;				/*0000016C*/ 
 
    uint8_t QSPI_reserved6[16]; 
 
    union { 
        u32 R; 
        struct { 
           u32:10; 
           u32 TPADA1:22; 
        } B; 
    } SFA1AD;				/*00000180*/ 
 
    union { 
        u32 R; 
        struct { 
           u32:10; 
           u32 TPADA2:22; 
        } B; 
    } SFA2AD;				/*00000184*/ 
 
    union { 
        u32 R; 
        struct { 
           u32:10; 
           u32 TPADB1:22; 
        } B; 
    } SFB1AD;				/*00000188*/ 
 
    union { 
        u32 R; 
        struct { 
           u32:10; 
           u32 TPADB2:22; 
        } B; 
    } SFB2AD;				/*0000018C*/ 
 
    uint8_t QSPI_reserved7[112]; 
 
    union { 
        u32 R; 
        struct { 
           u32 RXDATA:32; 
        } B; 
    } RBDR[32];				/*0000027C*/ 
 
    uint8_t QSPI_reserved8[128]; 
 
    union { 
        u32 R; 
        struct { 
           u32 KEY:32; 
        } B; 
    } LUTKEY;				/*00000300*/ 
 
    union { 
        u32 R; 
        struct { 
           u32 LOCK:1; 
           u32 UNLOCK:1; 
           u32:30; 
        } B; 
    } LCKCR;				/*00000304*/ 
 
    uint8_t QSPI_reserved9[8]; 
 
    union { 
        u32 R; 
        struct { 
           u32 OPRND0:8; 
           u32 PAD0:2; 
           u32 INSTR0:6; 
           u32 OPRND1:8; 
           u32 PAD1:2; 
           u32 INSTR1:6; 
        } B; 
    } LUT[64];				/**/ 

};

#define QSPI0    (*(volatile struct QSPI_tag *)        0x40044000)

#define CMD_RESET	0
#define CMD_WREN	1
#define CMD_BULKERASE	2
#define CMD_RDSTATUS	3
#define CMD_PAGEPROG	4
#define CMD_WRCONFIG	5
#define CMD_RDCONFIG	6
#define CMD_SECTORERASE	7
#define CMD_READ	8
#define CMD_QUADREAD	9
#define CMD_DDRREAD	10
#define CMD_QUADDDRRD	11
#define CMD_READID	12
#define CMD_READCFI	13
#define CMD_WRITEVDLP	14

void setup_iomux_quadspi(void)
{
	__raw_writel(0x001030C2, IOMUXC_PAD_079);	/* SCK */
	__raw_writel(0x001030FE, IOMUXC_PAD_080);	/* CS0 */
	__raw_writel(0x001030C3, IOMUXC_PAD_081);	/* D3 */
	__raw_writel(0x001030C3, IOMUXC_PAD_082);	/* D2 */
	__raw_writel(0x001030C3, IOMUXC_PAD_083);	/* D1 */
	__raw_writel(0x001030C3, IOMUXC_PAD_084);	/* D0 */

	__raw_writel(0x001030C2, IOMUXC_PAD_086);	/* SCK */
	__raw_writel(0x001030FE, IOMUXC_PAD_087);	/* CS0 */
	__raw_writel(0x001030C3, IOMUXC_PAD_088);	/* D3 */
	__raw_writel(0x001030C3, IOMUXC_PAD_089);	/* D2 */
	__raw_writel(0x001030C3, IOMUXC_PAD_090);	/* D1 */
	__raw_writel(0x001030C3, IOMUXC_PAD_091);	/* D0 */
}

void quadspi_setup_clocks(void)
{
	u32 temp;

	temp = __raw_readl(0x4006b010) & 0xfc3fffff;
	temp |= 0x02800000;
	__raw_writel(temp, 0x4006b010);
	temp = __raw_readl(0x4006b01C) | 0x1F1D;	// cdsr3
	__raw_writel(temp, 0x4006b01C);

	mdelay(100);
}

void quadspi_config_spi0(void)
{
	QSPI0.MCR.B.DDR_EN = 0x1;
	QSPI0.MCR.B.MDIS = 0x1;

	QSPI0.BUF0IND.R = 0x100;
	QSPI0.BUF1IND.R = 0x100;
	QSPI0.BUF2IND.R = 0x0;

	QSPI0.BUF0CR.R = 0x005;
	QSPI0.BUF1CR.R = 0x002;
	QSPI0.BUF3CR.R = 0x80000000;

#if defined(CONFIG_VYBRID_QSPI_128MBIT_DEVICE)
	QSPI0.SFA1AD.R = 0x21000000;	// top address of FA1 
	QSPI0.SFA2AD.R = 0x21000000;	// top address of FA2 
	QSPI0.SFB1AD.R = 0x22000000;	// top address of FB1 
	QSPI0.SFB2AD.R = 0x22000000;	// top address of FB2
#elif  defined(CONFIG_VYBRID_QSPI_512MBIT_DEVICE)
	QSPI0.SFA1AD.R = 0x24000000;	// top address of FA1 
	QSPI0.SFA2AD.R = 0x24000000;	// top address of FA2 
	QSPI0.SFB1AD.R = 0x28000000;	// top address of FB1 
	QSPI0.SFB2AD.R = 0x28000000;	// top address of FB2
#else
#error Unsupported QSPI device size
#endif
}

void quadspi_unlocklookuptable(void)
{
	QSPI0.LUTKEY.R = 0x5AF05AF0;
	QSPI0.LCKCR.R = 0x2;
	while (QSPI0.LCKCR.B.UNLOCK != 1) ;	// this may not be needed    
}

void quadspi_locklookuptable(void)
{
	QSPI0.LUTKEY.R = 0x5AF05AF0;
	QSPI0.LCKCR.R = 0x1;
	while (QSPI0.LCKCR.B.LOCK != 1) ;	// this may not be needed   
}

void quadspi_setuplookuptable(void)
{
	int lkuptbl;
#define CMD	1
#define ADDR	2
#define DUMMY	3
#define MODE	4
#define MODE2	5
#define MODE4	6
#define READ	7
#define WRITE	8
#define JMP_ON_CS	9
#define ADDR_DDR	10
#define MODE_DDR	11
#define READ_DDR	14

	quadspi_unlocklookuptable();

	// reset
	lkuptbl = 0;
	QSPI0.LUT[lkuptbl].B.INSTR0 = CMD;
	QSPI0.LUT[lkuptbl].B.PAD0 = 0;
	QSPI0.LUT[lkuptbl].B.OPRND0 = 0xF0;

	// Write enable 
	lkuptbl = 4;
	QSPI0.LUT[lkuptbl].R = 0x406;

	// Bulk Erase 
	lkuptbl = 8;
	QSPI0.LUT[lkuptbl].R = 0x460;

	// Read Status 
	lkuptbl = 12;
	QSPI0.LUT[lkuptbl].R = 0x1c010405;

	// Page Program 
	lkuptbl = 16;
	QSPI0.LUT[lkuptbl++].R = 0x08200412;	// 32bit address 
	QSPI0.LUT[lkuptbl].R = 0x2004;		// default 4-byte write 

	// Write Config/Status 
	lkuptbl = 20;
	QSPI0.LUT[lkuptbl].R = 0x20020401;	//2-byte write 

	// Read Config 
	lkuptbl = 24;
	QSPI0.LUT[lkuptbl].R = 0x1c010435;	//1-byte read 

	// Sector Erase 
	lkuptbl = 28;
	QSPI0.LUT[lkuptbl].R = 0x082004DC;

	// read
	lkuptbl = 32;
	QSPI0.LUT[lkuptbl].B.INSTR0 = CMD;
	QSPI0.LUT[lkuptbl].B.PAD0 = 0;
	QSPI0.LUT[lkuptbl].B.OPRND0 = 0x3;
	QSPI0.LUT[lkuptbl].B.INSTR1 = ADDR;
	QSPI0.LUT[lkuptbl].B.PAD1 = 0;
	QSPI0.LUT[lkuptbl++].B.OPRND1 = 24;
	QSPI0.LUT[lkuptbl].B.INSTR0 = READ;
	QSPI0.LUT[lkuptbl].B.PAD0 = 0;
	QSPI0.LUT[lkuptbl].B.OPRND0 = 8;

	// quad read
	lkuptbl = 36;
	QSPI0.LUT[lkuptbl].B.INSTR0 = CMD;
	QSPI0.LUT[lkuptbl].B.PAD0 = 0;
	QSPI0.LUT[lkuptbl].B.OPRND0 = 0xEB;
	QSPI0.LUT[lkuptbl].B.INSTR1 = ADDR;
	QSPI0.LUT[lkuptbl].B.PAD1 = 2;
	QSPI0.LUT[lkuptbl++].B.OPRND1 = 24;

	QSPI0.LUT[lkuptbl].B.INSTR0 = MODE;
	QSPI0.LUT[lkuptbl].B.PAD0 = 2;
	QSPI0.LUT[lkuptbl].B.OPRND0 = 0xA5;
	QSPI0.LUT[lkuptbl].B.INSTR1 = DUMMY;
	QSPI0.LUT[lkuptbl].B.PAD1 = 2;
	QSPI0.LUT[lkuptbl++].B.OPRND1 = 4;

	QSPI0.LUT[lkuptbl].B.INSTR0 = READ;
	QSPI0.LUT[lkuptbl].B.PAD0 = 2;
	QSPI0.LUT[lkuptbl].B.OPRND0 = 128;
	QSPI0.LUT[lkuptbl].B.INSTR1 = JMP_ON_CS;
	QSPI0.LUT[lkuptbl].B.PAD1 = 0;
	QSPI0.LUT[lkuptbl].B.OPRND1 = 1;

	// DDR Read
	lkuptbl = 40;
	QSPI0.LUT[lkuptbl].B.INSTR0 = CMD;
	QSPI0.LUT[lkuptbl].B.PAD0 = 0;
	QSPI0.LUT[lkuptbl].B.OPRND0 = 0x0D;
	// 24 bit addresses 
	QSPI0.LUT[lkuptbl].B.INSTR1 = ADDR_DDR;
	QSPI0.LUT[lkuptbl].B.PAD1 = 0;
	QSPI0.LUT[lkuptbl++].B.OPRND1 = 0x18;

	//mode bits and 2 dummy cycles
	QSPI0.LUT[lkuptbl].B.INSTR0 = MODE_DDR;
	QSPI0.LUT[lkuptbl].B.PAD0 = 0;
	QSPI0.LUT[lkuptbl].B.OPRND0 = 0xFF;
	QSPI0.LUT[lkuptbl].B.INSTR1 = DUMMY;
	QSPI0.LUT[lkuptbl].B.PAD1 = 0;
	QSPI0.LUT[lkuptbl++].B.OPRND1 = 0x2;

	//read 128 bytes
	QSPI0.LUT[lkuptbl].B.INSTR0 = READ_DDR;
	QSPI0.LUT[lkuptbl].B.PAD0 = 0;
	QSPI0.LUT[lkuptbl].B.OPRND0 = 0x80;
	QSPI0.LUT[lkuptbl].B.INSTR1 = JMP_ON_CS;
	QSPI0.LUT[lkuptbl].B.PAD1 = 0;
	QSPI0.LUT[lkuptbl].B.OPRND1 = 1;

	// Quad DDR read
	lkuptbl = 44;
	QSPI0.LUT[lkuptbl++].R = 0x2a2004ee;
	QSPI0.LUT[lkuptbl++].R = 0x0c022eff;
	QSPI0.LUT[lkuptbl++].R = 0x3a804234;
	QSPI0.LUT[lkuptbl].R = 0;

	// readID
	lkuptbl = 48;
	QSPI0.LUT[lkuptbl].B.INSTR0 = CMD;
	QSPI0.LUT[lkuptbl].B.PAD0 = 0;
	QSPI0.LUT[lkuptbl].B.OPRND0 = 0x90;
	QSPI0.LUT[lkuptbl].B.INSTR1 = ADDR;
	QSPI0.LUT[lkuptbl].B.PAD1 = 0;
	QSPI0.LUT[lkuptbl++].B.OPRND1 = 0x18;

	QSPI0.LUT[lkuptbl].B.INSTR0 = READ;
	QSPI0.LUT[lkuptbl].B.PAD0 = 0;
	QSPI0.LUT[lkuptbl].B.OPRND0 = 3;
	QSPI0.LUT[lkuptbl].B.INSTR1 = 0;
	QSPI0.LUT[lkuptbl].B.PAD1 = 0;
	QSPI0.LUT[lkuptbl].B.OPRND1 = 0;

	// readID CFI
	lkuptbl = 52;
	QSPI0.LUT[lkuptbl].B.INSTR0 = CMD;
	QSPI0.LUT[lkuptbl].B.PAD0 = 0;
	QSPI0.LUT[lkuptbl].B.OPRND0 = 0x9F;
	QSPI0.LUT[lkuptbl].B.INSTR1 = ADDR;
	QSPI0.LUT[lkuptbl].B.PAD1 = 0;
	QSPI0.LUT[lkuptbl++].B.OPRND1 = 24;

	QSPI0.LUT[lkuptbl].B.INSTR0 = READ;
	QSPI0.LUT[lkuptbl].B.PAD0 = 0;
	QSPI0.LUT[lkuptbl].B.OPRND0 = 0x80;

	// Write to the volatile Data Learning Pattern 
	lkuptbl = 56;
	QSPI0.LUT[lkuptbl].R = 0x2001044A;	//1-byte write 

	quadspi_locklookuptable();
}

#define ARDB	(*(volatile unsigned long *)0x7c000000)
void wait_while_flash_busy(void)
{
	uint32_t status_value = 0x1;
	while ((status_value & 0x1) == 0x1) {
		QSPI0.IPCR.R = CMD_RDSTATUS << 24;
		while (QSPI0.SR.B.BUSY) ;
		while (!QSPI0.SR.B.RXWE) ;
		status_value = ARDB;
		QSPI0.FR.R = 0x10000;	/* read complete */
	}
}

void enable_quad_bit(u32 status)
{
	//write enable 
	QSPI0.IPCR.R = CMD_WREN << 24;

	while (QSPI0.SR.B.BUSY) ;

	// write data to Tx Buffer
	QSPI0.TBDR.R = status;

	// send write command 
	QSPI0.IPCR.R = (CMD_WRCONFIG << 24);

	while (QSPI0.SR.B.BUSY) ;
	wait_while_flash_busy();

	// Read config reg to ensure write was successful 
	QSPI0.IPCR.R = (CMD_RDCONFIG << 24);
	while (QSPI0.SR.B.BUSY) ;
	while (!QSPI0.SR.B.RXWE) ;

	QSPI0.FR.R = 0x10000;	/* read complete */
}

/*
 // To enable DLP usage, leave the calls to write_VDLP(0x34000000) in 
 // quadspi_init() enabled AND have the LUT entry #11 look as follows: 
 //	lkuptbl = 44;
 // QSPI0.LUT[lkuptbl++].R = 0x2a1804ed;
 // QSPI0.LUT[lkuptbl++].R = 0x0c022eff;
 // QSPI0.LUT[lkuptbl++].R = 0x3a804234;
 // QSPI0.LUT[lkuptbl].R = 0;

 // To disable DLP usage, Comment out the call to write_VDLP(0x34000000) in 
 // quadspi_init() AND have the LUT entry #11 look as follows: 
 //	lkuptbl = 44;
 // QSPI0.LUT[lkuptbl++].R = 0x2a1804ed;
 // QSPI0.LUT[lkuptbl++].R = 0x0c062eff;
 // QSPI0.LUT[lkuptbl++].R = 0x3a80;
 // QSPI0.LUT[lkuptbl].R = 0;
*/

void write_VDLP(u32 vdlp)
{
	//write enable 
	QSPI0.IPCR.R = CMD_WREN << 24;

	while (QSPI0.SR.B.BUSY) ;

	// write data to Tx Buffer
	QSPI0.TBDR.R = vdlp;

	// send write VDLP command 
	QSPI0.IPCR.R = (CMD_WRITEVDLP << 24);

	while (QSPI0.SR.B.BUSY) ;
	wait_while_flash_busy();  // Normally this is not needed - Does not harm. 
}

#define SMBR	0x30000
#define USETBL	0xb000
#define FLASH_BASE_ADR	0x20000000
#if defined(CONFIG_VYBRID_QSPI_128MBIT_DEVICE)
#define FLASH_BASE1_ADR	0x21000000
#elif  defined(CONFIG_VYBRID_QSPI_512MBIT_DEVICE)
#define FLASH_BASE1_ADR	0x24000000
#else
#error Unsupported QSPI device size
#endif
int quadspi_init(void)
{
	setup_iomux_quadspi();	/* port configuration */
	/* make sure clock gate is enable */
	quadspi_setup_clocks();
	quadspi_config_spi0();

	quadspi_setuplookuptable();

	// set mdis bit 
	QSPI0.MCR.B.MDIS = 1;
	QSPI0.MCR.B.DDR_EN = 1;
	QSPI0.SMPR.R = SMBR;
	// clear mdis bit 
	QSPI0.MCR.B.MDIS = 0;

	// set quad ddr as xbar read instruction 
	QSPI0.BFGENCR.R = USETBL;

	QSPI0.MCR.B.CLR_TXF = 1;

	QSPI0.SFAR.R = FLASH_BASE_ADR;
	enable_quad_bit(0x00020000);
	write_VDLP(0x34000000);
	QSPI0.SFAR.R = FLASH_BASE1_ADR;
	enable_quad_bit(0x00020000);
	write_VDLP(0x34000000);

	QSPI0.SFAR.R = FLASH_BASE_ADR;

#if defined(CONFIG_VYBRID_QSPI_128MBIT_DEVICE)
	QSPI0.SFA1AD.R = (1 << 24) | 0x20000000;
	QSPI0.SFA2AD.R = (1 << 24) | 0x20000000;
	QSPI0.SFB1AD.R = (2 << 24) | 0x20000000;
	QSPI0.SFB2AD.R = (2 << 24) | 0x20000000;
#elif  defined(CONFIG_VYBRID_QSPI_512MBIT_DEVICE)
	QSPI0.SFA1AD.R = (4 << 24) | 0x20000000;
	QSPI0.SFA2AD.R = (4 << 24) | 0x20000000;
	QSPI0.SFB1AD.R = (8 << 24) | 0x20000000;
	QSPI0.SFB2AD.R = (8 << 24) | 0x20000000;
#else
#error Unsupported QSPI device size
#endif

	return 0;
}

void erase_sector(u32 flashbase)
{
	unsigned long mcr_reg;

	mcr_reg = QSPI0.MCR.R;
	QSPI0.MCR.R = QSPI_MCR_CLR_RXF_MASK | QSPI_MCR_CLR_TXF_MASK | 0xf0080;
	QSPI0.RBCT.R = 0x100;

	QSPI0.SFAR.R = flashbase;;

	//write enable 
	QSPI0.IPCR.R = CMD_WREN << 24;
	while (QSPI0.SR.B.BUSY) ;

	//send erase command 
	QSPI0.IPCR.R = CMD_SECTORERASE << 24;
	while (QSPI0.SR.B.BUSY) ;

	wait_while_flash_busy();

	QSPI0.MCR.R = mcr_reg;
}

void erase_flash(u32 flashbase)
{
	unsigned long mcr_reg;

	mcr_reg = QSPI0.MCR.R;
	QSPI0.MCR.R = QSPI_MCR_CLR_RXF_MASK | QSPI_MCR_CLR_TXF_MASK | 0xf0080;
	QSPI0.RBCT.R = 0x100;

	QSPI0.SFAR.R = flashbase;;

	//write enable 
	QSPI0.IPCR.R = CMD_WREN << 24;
	while (QSPI0.SR.B.BUSY) ;

	//send erase command 
	QSPI0.IPCR.R = CMD_BULKERASE << 24;
	while (QSPI0.SR.B.BUSY) ;

	wait_while_flash_busy();

	QSPI0.MCR.R = mcr_reg;
}

int qspi_flash_write(u32 offset, size_t len, void *buf)
{
	unsigned int *start_address = (unsigned int *)buf, tmp;
	unsigned int *end_address = (unsigned int *)(buf + roundup(len, flash->page_size));
	unsigned int *page_address = start_address;
	unsigned int *flash_address = (unsigned int *)(0x20000000 + offset);
	int page_size = flash->page_size;
	unsigned int *current_address = start_address;
	unsigned long mcr_reg;
	int i;

	mcr_reg = QSPI0.MCR.R;
	QSPI0.MCR.R = QSPI_MCR_CLR_RXF_MASK | QSPI_MCR_CLR_TXF_MASK | 0xf0080;
	QSPI0.RBCT.R = flash->page_size;

	// 1024 offset for spansion
	QSPI0.SFAR.R = (unsigned int)flash_address;

	page_address = start_address + (page_size >> 2);

	while (page_address <= end_address) {
		// write enable 
		QSPI0.IPCR.R = CMD_WREN << 24;
		while (QSPI0.SR.B.BUSY) ;
		QSPI0.MCR.B.CLR_TXF = 1;

		while (current_address < page_address) {
			// fill tx fifo (64 bytes)
			for (i = 0; i < 16; i++)
				if (swap_data) {
					tmp = *current_address++;
					QSPI0.TBDR.R = ___swab32(tmp);
				}
				else
					QSPI0.TBDR.R = *current_address++;

			QSPI0.IPCR.R = (CMD_PAGEPROG << 24) | page_size; //page program 256/512 bytes 
			for (i = 0; i < (flash->page_size >> 2) - 16; i++) {
				while (QSPI0.SR.B.TXFULL) ;
				if (swap_data) {
					tmp = *current_address++;
					QSPI0.TBDR.R = ___swab32(tmp);
				}
				else
					QSPI0.TBDR.R = *current_address++;
			}
			while (QSPI0.SR.B.BUSY) ;
			wait_while_flash_busy();

		}
		page_address += (page_size >> 2);
		flash_address += (page_size >> 2);
		QSPI0.SFAR.R = (unsigned int)flash_address;
	}

	QSPI0.MCR.R = mcr_reg;

	invalidate_dcache_range(0x20000000 + offset, 0x20000000 + offset +
			roundup(len, flash->page_size));

	return 0;
}

int qspi_flash_read(u32 offset, size_t len, void *buf)
{
	int size = 0, tmp, i;
	unsigned long *rxbuf = buf, mcr_reg;
	
	mcr_reg = QSPI0.MCR.R;
	QSPI0.MCR.R = QSPI_MCR_CLR_RXF_MASK | QSPI_MCR_CLR_TXF_MASK | 0xf0080;
	QSPI0.RBCT.R = flash->page_size;

	offset += 0x20000000;

	while (len > 0) {
		QSPI0.SFAR.R = offset;
		size = (len > RX_BUFFER_SIZE) ?
			RX_BUFFER_SIZE : len;
		QSPI0.IPCR.R = (0xb << QSPI_IPCR_SEQID_SHIFT) | size;
		while (QSPI0.SR.B.BUSY);

		offset += size;
		len -= size;

		i = 0;
		while ((RX_BUFFER_SIZE >= size) && (size > 0)) {
			tmp = QSPI0.RBDR[i].R;
			*rxbuf = ___swab32(tmp);
			rxbuf++;
			size -= 4;
			i++;
		}
		QSPI0.MCR.B.CLR_RXF = 1;
	}

	QSPI0.MCR.R = mcr_reg;

	return 0;
}

static const char *qspi_flash_update_block(struct spi_flash *flash, u32 offset,
		size_t len, const char *buf, char *cmp_buf, size_t *skipped)
{
	debug("offset=%#x, sector_size=%#x, len=%#zx\n",
		offset, flash->sector_size, len);

	cmp_buf = (char *)(0x20000000 + offset);

	if (memcmp(cmp_buf, buf, len) == 0) {
		debug("Skip region %x size %zx: no change\n",
			offset, len);
		*skipped += len;
		return NULL;
	}

	if (spi_flash_erase(flash, offset, len))
		return "erase";
	if (spi_flash_write(flash, offset, len, buf))
		return "write";
	return NULL;
}

/**
 * Update an area of SPI flash by erasing and writing any blocks which need
 * to change. Existing blocks with the correct data are left unchanged.
 *
 * @param flash		flash context pointer
 * @param offset	flash offset to write
 * @param len		number of bytes to write
 * @param buf		buffer to write from
 * @return 0 if ok, 1 on error
 */
static int qspi_flash_update(struct spi_flash *flash, u32 offset,
		size_t len, const char *buf)
{
	const char *err_oper = NULL;
	char *cmp_buf;
	const char *end = buf + len;
	size_t todo;		/* number of bytes to do in this pass */
	size_t skipped = 0;	/* statistics */

	for (; buf < end && !err_oper; buf += todo, offset += todo) {
		todo = min(end - buf, flash->sector_size);
		err_oper = qspi_flash_update_block(flash, offset, todo,
				buf, cmp_buf, &skipped);
	}

	if (err_oper) {
		printf("SPI flash failed in %s step\n", err_oper);
		return 1;
	}
	printf("%zu bytes written, %zu bytes skipped\n", len - skipped,
	       skipped);

	return 0;
}

/*
 * This function computes the length argument for the erase command.
 * The length on which the command is to operate can be given in two forms:
 * 1. <cmd> offset len  - operate on <'offset',  'len')
 * 2. <cmd> offset +len - operate on <'offset',  'round_up(len)')
 * If the second form is used and the length doesn't fall on the
 * sector boundary, than it will be adjusted to the next sector boundary.
 * If it isn't in the flash, the function will fail (return -1).
 * Input:
 *    arg: length specification (i.e. both command arguments)
 * Output:
 *    len: computed length for operation
 * Return:
 *    1: success
 *   -1: failure (bad format, bad address).
 */
static int sf_parse_len_arg(char *arg, ulong *len)
{
	char *ep;
	char round_up_len; /* indicates if the "+length" form used */
	ulong len_arg;

	round_up_len = 0;
	if (*arg == '+') {
		round_up_len = 1;
		++arg;
	}

	len_arg = simple_strtoul(arg, &ep, 16);
	if (ep == arg || *ep != '\0')
		return -1;

	if (round_up_len && flash->sector_size > 0)
		*len = ROUND(len_arg, flash->sector_size);
	else
		*len = len_arg;

	return 1;
}

int qspi_flash_probe(int swap)
{
	static int probed = 0;

	swap_data = swap;

	if (probed)
		return 0;

	quadspi_init();

	flash = malloc(100);
	flash->name = "Spansion";

#if defined(CONFIG_VYBRID_QSPI_128MBIT_DEVICE)
	flash->size = 0x2000000;
	flash->page_size = 0x100;
	flash->sector_size = 0x10000;
#elif  defined(CONFIG_VYBRID_QSPI_512MBIT_DEVICE)
	flash->size = 0x8000000;
	flash->page_size = 0x200;
	flash->sector_size = 0x40000;
#else
#error Unsupported QSPI device size
#endif

	probed = 1;

	invalidate_dcache_range(0x20000000, 0x20000000 + flash->size);

	printf("QSPI:  %iMb\n", flash->size / 1024 / 1024);

	return 0;
}

int do_qspi_flash_probe(int argc, char * const argv[])
{
	char *endp;
	return qspi_flash_probe(simple_strtoul(argv[1], &endp, 16));
}

static int do_qspi_flash_read_write(int argc, char * const argv[])
{
	unsigned long addr;
	unsigned long offset;
	unsigned long len;
	void *buf;
	char *endp;
	int ret;

	if (argc < 4)
		return -1;

	addr = simple_strtoul(argv[1], &endp, 16);
	if (*argv[1] == 0 || *endp != 0)
		return -1;
	offset = simple_strtoul(argv[2], &endp, 16);
	if (*argv[2] == 0 || *endp != 0)
		return -1;
	len = simple_strtoul(argv[3], &endp, 16);
	if (*argv[3] == 0 || *endp != 0)
		return -1;

	buf = map_physmem(addr, len, MAP_WRBACK);
	if (!buf) {
		puts("Failed to map physical memory\n");
		return 1;
	}

	if (strcmp(argv[0], "write") == 0)
		ret = qspi_flash_write(offset, len, buf);
	else if (strcmp(argv[0], "update") == 0)
		ret = qspi_flash_update(flash, offset, len, buf);
	else
		ret = qspi_flash_read(offset, len, buf);

	unmap_physmem(buf, len);

	if (ret) {
		printf("SPI flash %s failed\n", argv[0]);
		return 1;
	}

	return 0;
}

int qspi_flash_erase(unsigned long offset, unsigned int len)
{
	if ((offset == 0) && (len == flash->size)) {
		erase_flash(0x20000000);
		erase_flash(0x20000000 + flash->size / 2);
	}
	else {
		u32 base = 0x20000000 + offset;
		s32 i = len;

		while (i > 0) {
			erase_sector(base);
			base += flash->sector_size;
			i -= flash->sector_size;
		}
	}

	invalidate_dcache_range(0x20000000 + offset, 0x20000000 + offset + len);

	return 0;
}

static int do_qspi_flash_erase(int argc, char * const argv[])
{
	unsigned long offset;
	unsigned long len;
	char *endp;
	int ret;

	if (argc < 3)
		return -1;

	offset = simple_strtoul(argv[1], &endp, 16);
	if (*argv[1] == 0 || *endp != 0)
		return -1;

	ret = sf_parse_len_arg(argv[2], &len);
	if (ret != 1)
		return -1;

	return qspi_flash_erase(offset, len);
}

static int do_qspi_flash(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	const char *cmd;
	int ret;

	/* need at least two arguments */
	if (argc < 2)
		goto usage;

	cmd = argv[1];
	--argc;
	++argv;

	if (strcmp(cmd, "probe") == 0) {
		ret = do_qspi_flash_probe(argc, argv);
		goto done;
	}

	if (strcmp(cmd, "read") == 0 || strcmp(cmd, "write") == 0 ||
	    strcmp(cmd, "update") == 0)
		ret = do_qspi_flash_read_write(argc, argv);
	else if (strcmp(cmd, "erase") == 0)
		ret = do_qspi_flash_erase(argc, argv);
	else
		ret = -1;

done:
	if (ret != -1)
		return ret;

usage:
	return CMD_RET_USAGE;
}

U_BOOT_CMD(
	qspi,	5,	1,	do_qspi_flash,
	"QSPI flash sub-system",
	"probe swap			- init flash device on given SPI bus\n"
	"				  and chip select\n"
	"qspi write addr offset len	- write `len' bytes from memory\n"
	"				  at `addr' to flash at `offset'\n"
	"qspi erase offset [+]len	- erase `len' bytes from `offset'\n"
	"				  `+len' round up `len' to block size\n"
/*	"qspi update addr offset len	- erase and write `len' bytes from memory\n"
	"				  at `addr' to flash at `offset'"*/
);
