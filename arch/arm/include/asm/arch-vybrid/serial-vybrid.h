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

#ifndef __SERIAL_VYBRID_H__
#define __SERIAL_VYBRID_H__

#define UBDH		0x00
#define UBDL		0x01
#define UC1		0x02
#define UC2		0x03
#define US1		0x04
#define US2		0x05
#define UC3		0x06
#define UD		0x07
#define UMA1		0x08
#define UMA2		0x09
#define UC4		0x0A
#define UC5		0x0B
#define UED		0x0C
#define UMODEM		0x0D
#define UIR		0x0E
#define UPFIFO		0x10
#define UCFIFO		0x11
#define USFIFO		0x12
#define UTWFIFO		0x13
#define UTCFIFO		0x14
#define URWFIFO		0x15
#define URCFIFO		0x16
#define UC7816		0x18
#define UIE7816		0x19
#define UIS7816		0x1A
#define UWP7816T0	0x1B
#define UWP7816T1	0x1B
#define UWN7816		0x1C
#define UWF7816		0x1D
#define UET78416	0x1E
#define UTL7816		0x1F
#define UC6		0x21
#define UPCTH		0x22
#define UPCTL		0x23
#define UB1T		0x24
#define USDTH		0x25
#define USDTL		0x26
#define UPRE		0x27
#define UTPL		0x28
#define UIE		0x29
#define UWB		0x2A
#define US3		0x2B
#define US4		0x2C
#define UPRL		0x2D
#define URPREL		0x2E
#define UCPW		0x2F
#define URIDT		0x30
#define UTIDT		0x31

#define UBDH_LBKDIE	(1 << 7)
#define UBDH_RXEDGIE	(1 << 6)
#define UBDH_SBR(x)	(x & 0x1F)

#define UC1_LOOPS	(1 << 7)
#define UC1_SWAI	(1 << 6)
#define UC1_RSRC	(1 << 5)
#define UC1_M		(1 << 4)
#define UC1_WAKE	(1 << 3)
#define UC1_ILT		(1 << 2)
#define UC1_PE		(1 << 1)
#define UC1_PT		(1 << 0)

#define UC2_TIE		(1 << 7)
#define UC2_TCIE	(1 << 6)
#define UC2_RIE		(1 << 5)
#define UC2_ILIE	(1 << 4)
#define UC2_TE		(1 << 3)
#define UC2_RE		(1 << 2)
#define UC2_RWU		(1 << 1)
#define UC2_SBK		(1 << 0)

#define US1_TDRE	(1 << 7)
#define US1_TC		(1 << 6)
#define US1_RDRF	(1 << 5)
#define US1_IDLE	(1 << 4)
#define US1_OR		(1 << 3)
#define US1_NF		(1 << 2)
#define US1_FE		(1 << 1)
#define US1_PF		(1 << 0)

#define US2_LBKDIF	(1 << 7)
#define US2_RXEDGIF	(1 << 6)
#define US2_MSBF	(1 << 5)
#define US2_RXINV	(1 << 4)
#define US2_RWUID	(1 << 3)
#define US2_BRK13	(1 << 2)
#define US2_RBKDE	(1 << 1)
#define US2_RAF		(1 << 0)

#define UC3_R8		(1 << 7)
#define UC3_T8		(1 << 6)
#define UC3_TXDIR	(1 << 5)
#define UC3_TXINV	(1 << 4)
#define UC3_ORIE	(1 << 3)
#define UC3_NEIF	(1 << 2)
#define UC3_FEIF	(1 << 1)
#define UC3_PEIE	(1 << 0)

#define UC4_MAEN1	(1 << 7)
#define UC4_MAEN2	(1 << 6)
#define UC4_M10		(1 << 5)
#define UC4_BRFA(x)	(x & 0x1F)

#define UC5_TDMAS	(1 << 7)
#define UC5_RDMAS	(1 << 5)

#define UED_NOISY	(1 << 7)
#define UED_PARITYE	(1 << 6)

#define UMODEM_RXRTSE	(1 << 3)
#define UMODEM_TXRTSPOL	(1 << 2)
#define UMODEM_TXRTSE	(1 << 1)
#define UMODEM_TXCTSE	(1 << 0)

#define UIR_IREN	(1 << 2)
#define UIR_TNP(x)	(x & 3)

#define UPFIFO_TXFE	(1 << 7)
#define UPFIFO_TXFSZ(x)	((x & 7) << 4)
#define UPFIFO_RXFE	(1 << 3)
#define UPFIFO_RXFSZ(x)	((x & 7) << 0)

#define UCFIFO_TXFLUSH	(1 << 7)
#define UCFIFO_RXFLUSH	(1 << 6)
#define UCFIFO_TXOFE	(1 << 1)
#define UCFIFO_RXUFE	(1 << 0)

#define USFIFO_TXEMPT	(1 << 7)
#define USFIFO_RXEMPT	(1 << 6)
#define USFIFO_TXOF	(1 << 1)
#define USFIFO_RXUF	(1 << 0)

#define UC7816_ONACK	(1 << 4)
#define UC7816_ANACK	(1 << 3)
#define UC7816_INIT	(1 << 2)
#define UC7816_TTYPE	(1 << 1)
#define UC7816_7816E	(1 << 0)

#define UIE7816_WTE	(1 << 7)
#define UIE7816_CWTE	(1 << 6)
#define UIE7816_BWTE	(1 << 5)
#define UIE7816_INITDE	(1 << 4)
#define UIE7816_GTVE	(1 << 2)
#define UIE7816_TXTE	(1 << 1)
#define UIE7816_RXTE	(1 << 0)

#define UIS7816_WTE	(1 << 7)
#define UIS7816_CWTE	(1 << 6)
#define UIS7816_BWTE	(1 << 5)
#define UIS7816_INITDE	(1 << 4)
#define UIS7816_GTVE	(1 << 2)
#define UIS7816_TXTE	(1 << 1)
#define UIS7816_RXTE	(1 << 0)

#define UWP7816T1_CWI(x)	((x & 0xf) << 4)
#define UWP7816T1_BWI(x)	((x & 0xf) << 0)

#define UET78416_TXTHRESHOLD(x)	((x & 0xf) << 4)
#define UET78416_RXTHRESHOLD(x)	((x & 0xf) << 0)

#define UC6_EN709	(1 << 7)
#define UC6_TX709	(1 << 6)
#define UC6_CE		(1 << 5)
#define UC6_CP		(1 << 4)

#define UIE_WBEIE	(1 << 6)
#define UIE_ISDIE	(1 << 5)
#define UIE_PRXIE	(1 << 4)
#define UIE_PTXIE	(1 << 3)
#define UIE_PCTEIE	(1 << 2)
#define UIE_PSIE	(1 << 1)
#define UIE_TXFIE	(1 << 0)

#define US3_PEF		(1 << 7)
#define US3_WBEF	(1 << 6)
#define US3_ISD		(1 << 5)
#define US3_PRXF	(1 << 4)
#define US3_PTXF	(1 << 3)
#define US3_PCTEF	(1 << 2)
#define US3_PSF		(1 << 1)
#define US3_TXFF	(1 << 0)

#define US4_INITF	(1 << 4)
#define US4_CDET(x)	((x & 3) << 2)
#define US4_ILCV	(1 << 1)
#define US4_FE		(1 << 0)

#endif			/* __SERIAL_VYBRID_H__ */
