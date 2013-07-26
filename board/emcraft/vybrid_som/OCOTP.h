/***************************************************************** 
 * FILE        : OCOTP.h
 * 
 * DESCRIPTION : This is the header file describing the register
 *               set for:
 *               FARADAY
 *
 *               Please send any bugs fo b06321@freescale.com
 *               using [FARADAY_HEADER] in subject          
 * 
 * COPYRIGHT   :(c) 2011, Freescale Semiconductor
 * 
 * VERSION     : 0
 * DATE        : 07 September 2011
 * AUTHOR      : b06321 
 * HISTORY     : 
 * v0: 	Alpha release made up from currently available BG. Not yet  
 * 		fully tested. Regs and bits are liable to change up until
 * 		tape-out.
 * 
 *
 ******************************************************************/
 
/* ================================================================================
   ==============================       OCOTP        ==============================  
   ================================================================================ */
#ifndef OCOTP_H
#define OCOTP_H
#include "typedefs.h"

/* OCOTP - Peripheral register structure */
typedef struct OCOTP_MemMap {
  vuint32_t CTRL;      /* offset: 0x00000000*/
  vuint32_t CTRL_SET;      /* offset: 0x00000004*/
  vuint32_t CTRL_CLR;      /* offset: 0x00000008*/
  vuint32_t CTRL_TOG;      /* offset: 0x0000000C*/
  vuint32_t TIMING;    /* offset: 0x00000010*/
  vuint8_t RESERVED0[12];
  vuint32_t DATA;      /* offset: 0x00000020*/
  vuint8_t RESERVED1[12];
  vuint32_t READ_CTRL; /* offset: 0x00000030*/
  vuint8_t RESERVED2[12];
  vuint32_t READ_FUSE_DATA; /* offset: 0x00000040*/
  vuint8_t RESERVED3[12];
  vuint32_t SW_STICKY;	/* offset: 0x00000050*/
  vuint8_t RESERVED4[12];
  vuint32_t SCS;       /* offset: 0x00000060*/
  vuint32_t SCS_SET;      /* offset: 0x00000064*/
  vuint32_t SCS_CLR;      /* offset: 0x00000068*/
  vuint32_t SCS_TOG;      /* offset: 0x0000006C*/
  vuint32_t CRC_ADDR;      /* offset: 0x00000070*/
  vuint8_t RESERVED5[12];
  vuint32_t CRC_VALUE;      /* offset: 0x00000080*/
  vuint8_t RESERVED6[12];
  vuint32_t VERSION;   /* offset: 0x00000090*/
  vuint8_t RESERVED7[876];	
  vuint32_t LOCK;      /* offset: 0x00000400*/
  vuint8_t RESERVED8[12];
  vuint32_t CFG0;      /* offset: 0x00000410*/
  vuint8_t RESERVED9[12];
  vuint32_t CGF1;      /* offset: 0x00000420*/
  vuint8_t RESERVED10[12];
  vuint32_t CFG2;      /* offset: 0x00000430*/
  vuint8_t RESERVED11[12];
  vuint32_t CFG3;      /* offset: 0x00000440*/
  vuint8_t RESERVED12[12];
  vuint32_t CFG4;      /* offset: 0x00000450*/
  vuint8_t RESERVED13[12];
  vuint32_t CFG5;      /* offset: 0x00000460*/
  vuint8_t RESERVED14[12];
  vuint32_t CFG6;      /* offset: 0x00000470*/
  vuint8_t RESERVED15[12];
  vuint32_t MEM0;      /* offset: 0x00000480*/
  vuint8_t RESERVED16[12];
  vuint32_t MEM1;      /* offset: 0x00000490*/
  vuint8_t RESERVED17[12];
  vuint32_t MEM2;      /* offset: 0x000004A0*/
  vuint8_t RESERVED18[12];
  vuint32_t MEM3;      /* offset: 0x000004B0*/
  vuint8_t RESERVED19[12];
  vuint32_t MEM4;      /* offset: 0x000004C0*/
  vuint8_t RESERVED20[12];
  vuint32_t ANA0;      /* offset: 0x000004D0*/
  vuint8_t RESERVED21[12];
  vuint32_t ANA1;      /* offset: 0x000004E0*/
  vuint8_t RESERVED22[12];
  vuint32_t ANA2;      /* offset: 0x000004F0*/
  vuint8_t RESERVED23[12];
  vuint32_t OTPMK0;    /* offset: 0x00000500*/
  vuint8_t RESERVED24[12];
  vuint32_t OTPMK1;    /* offset: 0x00000510*/
  vuint8_t RESERVED25[12];
  vuint32_t OTPMK2;    /* offset: 0x00000520*/
  vuint8_t RESERVED26[12];
  vuint32_t OTPMK3;    /* offset: 0x00000530*/
  vuint8_t RESERVED27[12];
  vuint32_t OTPMK4;    /* offset: 0x00000540*/
  vuint8_t RESERVED28[12];
  vuint32_t OTPMK5;    /* offset: 0x00000550*/
  vuint8_t RESERVED29[12];
  vuint32_t OTPMK6;    /* offset: 0x00000560*/
  vuint8_t RESERVED30[12];
  vuint32_t OTPMK7;    /* offset: 0x00000570*/
  vuint8_t RESERVED31[12];
  vuint32_t SRK0;      /* offset: 0x00000580*/
  vuint8_t RESERVED32[12];
  vuint32_t SRK1;      /* offset: 0x00000590*/
  vuint8_t RESERVED33[12];
  vuint32_t SRK2;      /* offset: 0x000005A0*/
  vuint8_t RESERVED34[12];
  vuint32_t SRK3;      /* offset: 0x000005B0*/
  vuint8_t RESERVED35[12];
  vuint32_t SRK4;      /* offset: 0x000005C0*/
  vuint8_t RESERVED36[12];
  vuint32_t SRK5;      /* offset: 0x000005D0*/
  vuint8_t RESERVED37[12];
  vuint32_t SRK6;      /* offset: 0x000005E0*/
  vuint8_t RESERVED38[12];
  vuint32_t SRK7;      /* offset: 0x000005F0*/
  vuint8_t RESERVED39[12];
  vuint32_t RESP0; /* offset: 0x00000600*/
  vuint8_t RESERVED40[12];
  vuint32_t RESP1; /* offset: 0x00000610*/
  vuint8_t RESERVED41[12];
  vuint32_t MAC0;      /* offset: 0x00000620*/
  vuint8_t RESERVED42[12];
  vuint32_t MAC1;      /* offset: 0x00000630*/
  vuint8_t RESERVED43[12];
  vuint32_t MAC2;      /* offset: 0x00000640*/
  vuint8_t RESERVED44[12];
  vuint32_t MAC3;      /* offset: 0x00000650*/
  vuint8_t RESERVED45[12];
  vuint32_t GP1;       /* offset: 0x00000660*/
  vuint8_t RESERVED46[12];
  vuint32_t GP2;       /* offset: 0x00000670*/
  vuint8_t RESERVED47[92];
  vuint32_t MISC_CONF;       /* offset: 0x000006D0*/
  vuint8_t RESERVED48[12];
  vuint32_t FIELD_RETURN;	/* offset: 0x000006E0*/
  vuint8_t RESERVED49[12];
  vuint32_t SRK_REVOKE;	/* offset: 0x000006F0*/
  vuint8_t RESERVED50[396];
  vuint32_t TFUSE0; /* offset: 0x00000880*/
  vuint8_t RESERVED51[12];
  vuint32_t TFUSE1; /* offset: 0x00000890*/
  vuint8_t RESERVED52[28]; 
  vuint32_t PMUR; /* offset: 0x000008B0*/
  vuint8_t RESERVED53[12];
  vuint32_t PMU; /* offset: 0x000008C0*/
  vuint8_t RESERVED54[12];
  vuint32_t RNG; /* offset: 0x000008D0*/
  vuint8_t RESERVED55[28]; 
  vuint32_t VTMON; /* offset: 0x000008F0*/
  vuint8_t RESERVED56[12];
  vuint32_t CRC0; /* offset: 0x00000900*/
  vuint8_t RESERVED57[12];
  vuint32_t CRC1; /* offset: 0x00000910*/
  vuint8_t RESERVED58[12];
  vuint32_t CRC2; /* offset: 0x00000920*/
  vuint8_t RESERVED59[12];
  vuint32_t CRC3; /* offset: 0x00000930*/
  vuint8_t RESERVED60[12];
  vuint32_t CRC4; /* offset: 0x00000940*/
  vuint8_t RESERVED61[12];
  vuint32_t CRC5; /* offset: 0x00000950*/
  vuint8_t RESERVED62[12];
  vuint32_t CRC6;	/* offset: 0x00000960*/
  vuint8_t RESERVED63[12];
  vuint32_t CRC7;	/* offset: 0x00000970*/
} volatile *OCOTP_MemMapPtr;

/* OCOTP - Register offsets */
#define OCOTP_CTRL_OFFSET               0x00000000
#define OCOTP_CTRL_SET_OFFSET           0x00000004
#define OCOTP_CTRL_CLR_OFFSET           0x00000008
#define OCOTP_CTRL_TOG_OFFSET           0x0000000C
#define OCOTP_TIMING_OFFSET             0x00000010
#define OCOTP_DATA_OFFSET               0x00000020
#define OCOTP_READ_CTRL_OFFSET          0x00000030
#define OCOTP_READ_FUSE_DATA_OFFSET     0x00000040
#define OCOTP_SW_STICKY_OFFSET          0x00000050
#define OCOTP_SCS_OFFSET                0x00000060
#define OCOTP_SCS_SET_OFFSET            0x00000064
#define OCOTP_SCS_CLR_OFFSET            0x00000068
#define OCOTP_SCS_TOG_OFFSET            0x0000006C
#define OCOTP_CRC_ADDR_OFFSET           0x00000070
#define OCOTP_CRC_VALUE_OFFSET          0x00000080
#define OCOTP_VERSION_OFFSET            0x00000090
#define OCOTP_LOCK_OFFSET               0x00000400
#define OCOTP_CFG0_OFFSET               0x00000410
#define OCOTP_CGF1_OFFSET               0x00000420
#define OCOTP_CFG2_OFFSET               0x00000430
#define OCOTP_CFG3_OFFSET               0x00000440
#define OCOTP_CFG4_OFFSET               0x00000450
#define OCOTP_CFG5_OFFSET               0x00000460
#define OCOTP_CFG6_OFFSET               0x00000470
#define OCOTP_MEM0_OFFSET               0x00000480
#define OCOTP_MEM1_OFFSET               0x00000490
#define OCOTP_MEM2_OFFSET               0x000004A0
#define OCOTP_MEM3_OFFSET               0x000004B0
#define OCOTP_MEM4_OFFSET               0x000004C0
#define OCOTP_ANA0_OFFSET               0x000004D0
#define OCOTP_ANA1_OFFSET               0x000004E0
#define OCOTP_ANA2_OFFSET               0x000004F0
#define OCOTP_OTPMK0_OFFSET             0x00000500
#define OCOTP_OTPMK1_OFFSET             0x00000510
#define OCOTP_OTPMK2_OFFSET             0x00000520
#define OCOTP_OTPMK3_OFFSET             0x00000530
#define OCOTP_OTPMK4_OFFSET             0x00000540
#define OCOTP_OTPMK5_OFFSET             0x00000550
#define OCOTP_OTPMK6_OFFSET             0x00000560
#define OCOTP_OTPMK7_OFFSET             0x00000570
#define OCOTP_SRK0_OFFSET               0x00000580
#define OCOTP_SRK1_OFFSET               0x00000590
#define OCOTP_SRK2_OFFSET               0x000005A0
#define OCOTP_SRK3_OFFSET               0x000005B0
#define OCOTP_SRK4_OFFSET               0x000005C0
#define OCOTP_SRK5_OFFSET               0x000005D0
#define OCOTP_SRK6_OFFSET               0x000005E0
#define OCOTP_SRK7_OFFSET               0x000005F0
#define OCOTP_RESP0_OFFSET              0x00000600
#define OCOTP_RESP1_OFFSET              0x00000610
#define OCOTP_MAC0_OFFSET               0x00000620
#define OCOTP_MAC1_OFFSET               0x00000630
#define OCOTP_MAC2_OFFSET               0x00000640
#define OCOTP_MAC3_OFFSET               0x00000650
#define OCOTP_GP1_OFFSET                0x00000660
#define OCOTP_GP2_OFFSET                0x00000670
#define OCOTP_MISC_CONF_OFFSET          0x000006D0
#define OCOTP_FIELD_RETURN_OFFSET       0x000006E0
#define OCOTP_SRK_REVOKE_OFFSET         0x000006F0
#define OCOTP_SRK_RSDV0_OFFSET          0x00000800   //added by Pankaj
#define OCOTP_SRK_RSDV1_OFFSET          0x00000810   //added by Pankaj
#define OCOTP_SRK_RSDV2_OFFSET          0x00000820   //added by Pankaj
#define OCOTP_SRK_RSDV3_OFFSET          0x00000830   //added by Pankaj
#define OCOTP_SRK_RSDV4_OFFSET          0x00000840   //added by Pankaj
#define OCOTP_TFUSE0_OFFSET             0x00000880
#define OCOTP_TFUSE1_OFFSET             0x00000890
#define OCOTP_PMUR_OFFSET               0x000008B0
#define OCOTP_PMU_OFFSET                0x000008C0
#define OCOTP_RNG_OFFSET                0x000008D0
#define OCOTP_VTMON_OFFSET              0x000008F0
#define OCOTP_CRC0_OFFSET               0x00000900
#define OCOTP_CRC1_OFFSET               0x00000910
#define OCOTP_CRC2_OFFSET               0x00000920
#define OCOTP_CRC3_OFFSET               0x00000930
#define OCOTP_CRC4_OFFSET               0x00000940
#define OCOTP_CRC5_OFFSET               0x00000950
#define OCOTP_CRC6_OFFSET               0x00000960
#define OCOTP_CRC7_OFFSET               0x00000970

/* OCOTP Register per module */
#define OCOTP_CTRL                       (OCOTP_BASE + OCOTP_CTRL_OFFSET)
#define OCOTP_CTRL_SET                   (OCOTP_BASE + OCOTP_CTRL_SET_OFFSET)
#define OCOTP_CTRL_CLR                   (OCOTP_BASE + OCOTP_CTRL_CLR_OFFSET)
#define OCOTP_CTRL_TOG                   (OCOTP_BASE + OCOTP_CTRL_TOG_OFFSET)
#define OCOTP_TIMING                     (OCOTP_BASE + OCOTP_TIMING_OFFSET)
#define OCOTP_DATA                       (OCOTP_BASE + OCOTP_DATA_OFFSET)
#define OCOTP_READ_CTRL                  (OCOTP_BASE + OCOTP_READ_CTRL_OFFSET)
#define OCOTP_READ_FUSE_DATA             (OCOTP_BASE + OCOTP_READ_FUSE_DATA_OFFSET)
#define OCOTP_SW_STICKY                  (OCOTP_BASE + OCOTP_SW_STICKY_OFFSET)
#define OCOTP_SCS                        (OCOTP_BASE + OCOTP_SCS_OFFSET)
#define OCOTP_SCS_SET                    (OCOTP_BASE + OCOTP_SCS_SET_OFFSET)
#define OCOTP_SCS_CLR                    (OCOTP_BASE + OCOTP_SCS_CLR_OFFSET)
#define OCOTP_SCS_TOG                    (OCOTP_BASE + OCOTP_SCS_TOG_OFFSET)
#define OCOTP_CRC_ADDR                   (OCOTP_BASE + OCOTP_CRC_ADDR_OFFSET)
#define OCOTP_CRC_VALUE                  (OCOTP_BASE + OCOTP_CRC_VALUE_OFFSET)
#define OCOTP_VERSION                    (OCOTP_BASE + OCOTP_VERSION_OFFSET)
#define OCOTP_LOCK                       (OCOTP_BASE + OCOTP_LOCK_OFFSET)
#define OCOTP_CFG0                       (OCOTP_BASE + OCOTP_CFG0_OFFSET)
#define OCOTP_CGF1                       (OCOTP_BASE + OCOTP_CGF1_OFFSET)
#define OCOTP_CFG2                       (OCOTP_BASE + OCOTP_CFG2_OFFSET)
#define OCOTP_CFG3                       (OCOTP_BASE + OCOTP_CFG3_OFFSET)
#define OCOTP_CFG4                       (OCOTP_BASE + OCOTP_CFG4_OFFSET)
#define OCOTP_CFG5                       (OCOTP_BASE + OCOTP_CFG5_OFFSET)
#define OCOTP_CFG6                       (OCOTP_BASE + OCOTP_CFG6_OFFSET)
#define OCOTP_MEM0                       (OCOTP_BASE + OCOTP_MEM0_OFFSET)
#define OCOTP_MEM1                       (OCOTP_BASE + OCOTP_MEM1_OFFSET)
#define OCOTP_MEM2                       (OCOTP_BASE + OCOTP_MEM2_OFFSET)
#define OCOTP_MEM3                       (OCOTP_BASE + OCOTP_MEM3_OFFSET)
#define OCOTP_MEM4                       (OCOTP_BASE + OCOTP_MEM4_OFFSET)
#define OCOTP_ANA0                       (OCOTP_BASE + OCOTP_ANA0_OFFSET)
#define OCOTP_ANA1                       (OCOTP_BASE + OCOTP_ANA1_OFFSET)
#define OCOTP_ANA2                       (OCOTP_BASE + OCOTP_ANA1_OFFSET)
#define OCOTP_OTPMK0                     (OCOTP_BASE + OCOTP_OTPMK0_OFFSET)
#define OCOTP_OTPMK1                     (OCOTP_BASE + OCOTP_OTPMK1_OFFSET)
#define OCOTP_OTPMK2                     (OCOTP_BASE + OCOTP_OTPMK2_OFFSET)
#define OCOTP_OTPMK3                     (OCOTP_BASE + OCOTP_OTPMK3_OFFSET)
#define OCOTP_OTPMK4                     (OCOTP_BASE + OCOTP_OTPMK4_OFFSET)
#define OCOTP_OTPMK5                     (OCOTP_BASE + OCOTP_OTPMK5_OFFSET)
#define OCOTP_OTPMK6                     (OCOTP_BASE + OCOTP_OTPMK6_OFFSET)
#define OCOTP_OTPMK7                     (OCOTP_BASE + OCOTP_OTPMK7_OFFSET)
#define OCOTP_SRK0                       (OCOTP_BASE + OCOTP_SRK0_OFFSET)
#define OCOTP_SRK1                       (OCOTP_BASE + OCOTP_SRK1_OFFSET)
#define OCOTP_SRK2                       (OCOTP_BASE + OCOTP_SRK2_OFFSET)
#define OCOTP_SRK3                       (OCOTP_BASE + OCOTP_SRK3_OFFSET)
#define OCOTP_SRK4                       (OCOTP_BASE + OCOTP_SRK4_OFFSET)
#define OCOTP_SRK5                       (OCOTP_BASE + OCOTP_SRK5_OFFSET)
#define OCOTP_SRK6                       (OCOTP_BASE + OCOTP_SRK6_OFFSET)
#define OCOTP_SRK7                       (OCOTP_BASE + OCOTP_SRK7_OFFSET)
#define OCOTP_RESP0                      (OCOTP_BASE + OCOTP_RESP0_OFFSET)
#define OCOTP_RESP1                      (OCOTP_BASE + OCOTP_RESP1_OFFSET)
#define OCOTP_MAC0                       (OCOTP_BASE + OCOTP_MAC0_OFFSET)
#define OCOTP_MAC1                       (OCOTP_BASE + OCOTP_MAC1_OFFSET)
#define OCOTP_MAC2                       (OCOTP_BASE + OCOTP_MAC2_OFFSET)
#define OCOTP_MAC3                       (OCOTP_BASE + OCOTP_MAC3_OFFSET)
#define OCOTP_GP1                        (OCOTP_BASE + OCOTP_GP1_OFFSET)
#define OCOTP_GP2                        (OCOTP_BASE + OCOTP_GP2_OFFSET)
#define OCOTP_MISC_CONF                  (OCOTP_BASE + OCOTP_MISC_CONF_OFFSET)
#define OCOTP_FIELD_RETURN               (OCOTP_BASE + OCOTP_FIELD_RETURN_OFFSET)
#define OCOTP_SRK_REVOKE                 (OCOTP_BASE + OCOTP_SRK_REVOKE_OFFSET)

#define OCOTP_SRK_RSDV0                  (OCOTP_BASE + OCOTP_SRK_RSDV0_OFFSET)   //added by Pankaj
#define OCOTP_SRK_RSDV1                  (OCOTP_BASE + OCOTP_SRK_RSDV1_OFFSET)   //added by Pankaj
#define OCOTP_SRK_RSDV2                  (OCOTP_BASE + OCOTP_SRK_RSDV2_OFFSET)   //added by Pankaj
#define OCOTP_SRK_RSDV3                  (OCOTP_BASE + OCOTP_SRK_RSDV3_OFFSET)   //added by Pankaj
#define OCOTP_SRK_RSDV4                  (OCOTP_BASE + OCOTP_SRK_RSDV4_OFFSET)   //added by Pankaj
#define OCOTP_TFUSE0                     (OCOTP_BASE + OCOTP_TFUSE0_OFFSET)
#define OCOTP_TFUSE1                     (OCOTP_BASE + OCOTP_TFUSE1_OFFSET)
#define OCOTP_PMUR                       (OCOTP_BASE + OCOTP_PMUR_OFFSET)
#define OCOTP_PMU                        (OCOTP_BASE + OCOTP_PMU_OFFSET)
#define OCOTP_RNG                        (OCOTP_BASE + OCOTP_RNG_OFFSET)
#define OCOTP_VTMON                      (OCOTP_BASE + OCOTP_VTMON_OFFSET)

#define OCOTP_CRC0                       (OCOTP_BASE + OCOTP_CRC0_OFFSET)
#define OCOTP_CRC1                       (OCOTP_BASE + OCOTP_CRC1_OFFSET)
#define OCOTP_CRC2                       (OCOTP_BASE + OCOTP_CRC2_OFFSET)
#define OCOTP_CRC3                       (OCOTP_BASE + OCOTP_CRC3_OFFSET)
#define OCOTP_CRC4                       (OCOTP_BASE + OCOTP_CRC4_OFFSET)
#define OCOTP_CRC5                       (OCOTP_BASE + OCOTP_CRC5_OFFSET)
#define OCOTP_CRC6                       (OCOTP_BASE + OCOTP_CRC6_OFFSET)
#define OCOTP_CRC7                       (OCOTP_BASE + OCOTP_CRC7_OFFSET)


/* Field definitions for CTRL */
#define OCOTP_CTRL_ADDR_SHIFT               (0)
#define OCOTP_CTRL_ADDR_MASK                ((0x0000007F) << (OCOTP_CTRL_ADDR_SHIFT)) 

#define OCOTP_CTRL_BUSY_SHIFT               (8)  
#define OCOTP_CTRL_BUSY_MASK                ((1) << (OCOTP_CTRL_BUSY_SHIFT)) 

#define OCOTP_CTRL_ERROR_SHIFT              (9)  
#define OCOTP_CTRL_ERROR_MASK               ((1) << (OCOTP_CTRL_ERROR_SHIFT)) 

#define OCOTP_CTRL_RELOAD_SHADOWS_SHIFT     (10)  
#define OCOTP_CTRL_RELOAD_SHADOWS_MASK      ((1) << (OCOTP_CTRL_RELOAD_SHADOWS_SHIFT)) 

#define OCOTP_CTRL_CRC_TEST_SHIFT           (11)
#define OCOTP_CTRL_CRC_TEST_MASK            ((1) << (OCOTP_CTRL_CRC_TEST_SHIFT)) 

#define OCOTP_CTRL_CRC_FAIL_SHIFT           (12)
#define OCOTP_CTRL_CRC_FAIL_MASK            ((1) << (OCOTP_CTRL_CRC_FAIL_SHIFT)) 

#define OCOTP_CTRL_WR_UNLOCK_SHIFT          (16)
#define OCOTP_CTRL_WR_UNLOCK_MASK           ((0x0000FFFF) << (OCOTP_CTRL_WR_UNLOCK_SHIFT)) 



/* Field definitions for CTRL_SET */
#define OCOTP_CTRL_SET_ADDR_SHIFT               (0)
#define OCOTP_CTRL_SET_ADDR_MASK                ((0x0000007F) << (OCOTP_CTRL_SET_ADDR_SHIFT)) 

#define OCOTP_CTRL_SET_BUSY_SHIFT               (8)  
#define OCOTP_CTRL_SET_BUSY_MASK                ((1) << (OCOTP_CTRL_SET_BUSY_SHIFT)) 

#define OCOTP_CTRL_SET_ERROR_SHIFT              (9)  
#define OCOTP_CTRL_SET_ERROR_MASK               ((1) << (OCOTP_CTRL_SET_ERROR_SHIFT)) 

#define OCOTP_CTRL_SET_RELOAD_SHADOWS_SHIFT     (10)  
#define OCOTP_CTRL_SET_RELOAD_SHADOWS_MASK      ((1) << (OCOTP_CTRL_SET_RELOAD_SHADOWS_SHIFT)) 

#define OCOTP_CTRL_SET_CRC_TEST_SHIFT           (11)
#define OCOTP_CTRL_SET_CRC_TEST_MASK            ((1) << (OCOTP_CTRL_SET_CRC_TEST_SHIFT)) 

#define OCOTP_CTRL_SET_CRC_FAIL_SHIFT           (12)
#define OCOTP_CTRL_SET_CRC_FAIL_MASK            ((1) << (OCOTP_CTRL_SET_CRC_FAIL_SHIFT)) 

#define OCOTP_CTRL_SET_WR_UNLOCK_SHIFT          (16)
#define OCOTP_CTRL_SET_WR_UNLOCK_MASK           ((0x0000FFFF) << (OCOTP_CTRL_SET_WR_UNLOCK_SHIFT)) 



/* Field definitions for CTRL_CLR */
#define OCOTP_CTRL_CLR_ADDR_SHIFT               (0)
#define OCOTP_CTRL_CLR_ADDR_MASK                ((0x0000007F) << (OCOTP_CTRL_CLR_ADDR_SHIFT)) 

#define OCOTP_CTRL_CLR_BUSY_SHIFT               (8)  
#define OCOTP_CTRL_CLR_BUSY_MASK                ((1) << (OCOTP_CTRL_CLR_BUSY_SHIFT)) 

#define OCOTP_CTRL_CLR_ERROR_SHIFT              (9)  
#define OCOTP_CTRL_CLR_ERROR_MASK               ((1) << (OCOTP_CTRL_CLR_ERROR_SHIFT)) 

#define OCOTP_CTRL_CLR_RELOAD_SHADOWS_SHIFT     (10)  
#define OCOTP_CTRL_CLR_RELOAD_SHADOWS_MASK      ((1) << (OCOTP_CTRL_CLR_RELOAD_SHADOWS_SHIFT)) 

#define OCOTP_CTRL_CLR_CRC_TEST_SHIFT           (11)
#define OCOTP_CTRL_CLR_CRC_TEST_MASK            ((1) << (OCOTP_CTRL_CLR_CRC_TEST_SHIFT)) 

#define OCOTP_CTRL_CLR_CRC_FAIL_SHIFT           (12)
#define OCOTP_CTRL_CLR_CRC_FAIL_MASK            ((1) << (OCOTP_CTRL_CLR_CRC_FAIL_SHIFT)) 

#define OCOTP_CTRL_CLR_WR_UNLOCK_SHIFT          (16)
#define OCOTP_CTRL_CLR_WR_UNLOCK_MASK           ((0x0000FFFF) << (OCOTP_CTRL_CLR_WR_UNLOCK_SHIFT)) 



/* Field definitions for CTRL_TOG */
#define OCOTP_CTRL_TOG_ADDR_SHIFT               (0)
#define OCOTP_CTRL_TOG_ADDR_MASK                ((0x0000007F) << (OCOTP_CTRL_TOG_ADDR_SHIFT)) 

#define OCOTP_CTRL_TOG_BUSY_SHIFT               (8)  
#define OCOTP_CTRL_TOG_BUSY_MASK                ((1) << (OCOTP_CTRL_TOG_BUSY_SHIFT)) 

#define OCOTP_CTRL_TOG_ERROR_SHIFT              (9)  
#define OCOTP_CTRL_TOG_ERROR_MASK               ((1) << (OCOTP_CTRL_TOG_ERROR_SHIFT)) 

#define OCOTP_CTRL_TOG_RELOAD_SHADOWS_SHIFT     (10)  
#define OCOTP_CTRL_TOG_RELOAD_SHADOWS_MASK      ((1) << (OCOTP_CTRL_TOG_RELOAD_SHADOWS_SHIFT)) 

#define OCOTP_CTRL_TOG_CRC_TEST_SHIFT           (11)
#define OCOTP_CTRL_TOG_CRC_TEST_MASK            ((1) << (OCOTP_CTRL_TOG_CRC_TEST_SHIFT)) 

#define OCOTP_CTRL_TOG_CRC_FAIL_SHIFT           (12)
#define OCOTP_CTRL_TOG_CRC_FAIL_MASK            ((1) << (OCOTP_CTRL_TOG_CRC_FAIL_SHIFT)) 

#define OCOTP_CTRL_TOG_WR_UNLOCK_SHIFT          (16)
#define OCOTP_CTRL_TOG_WR_UNLOCK_MASK           ((0x0000FFFF) << (OCOTP_CTRL_TOG_WR_UNLOCK_SHIFT)) 



/* Field definitions for TIMING */
#define OCOTP_TIMING_STROBE_PROG_SHIFT        (0)
#define OCOTP_TIMING_STROBE_PROG_MASK         ((0x00000FFF) << (OCOTP_TIMING_STROBE_PROG_SHIFT)) 

#define OCOTP_TIMING_RELAX_SHIFT              (12)
#define OCOTP_TIMING_RELAX_MASK               ((0x0000000F) << (OCOTP_TIMING_RELAX_SHIFT)) 

#define OCOTP_TIMING_STROBE_READ_SHIFT        (16)
#define OCOTP_TIMING_STROBE_READ_MASK         ((0x0000003F) << (OCOTP_TIMING_STROBE_READ_SHIFT)) 

#define OCOTP_TIMING_WAIT_SHIFT               (22)
#define OCOTP_TIMING_WAIT_MASK                ((0x0000003F) << (OCOTP_TIMING_WAIT_SHIFT)) 



/* Field definitions for DATA */
#define OCOTP_DATA_DATA_SHIFT               (0)
#define OCOTP_DATA_DATA_MASK                ((0xFFFFFFFF) << (OCOTP_DATA_DATA_SHIFT)) 



/* Field definitions for READ_CTRL */
#define OCOTP_READ_CTRL_READ_FUSE_SHIFT          (0)  
#define OCOTP_READ_CTRL_READ_FUSE_MASK           ((1) << (OCOTP_READ_CTRL_READ_FUSE_SHIFT)) 



/* Field definitions for READ_FUSE */
#define OCOTP_READ_FUSE_DATA_DATA_SHIFT               (0)
#define OCOTP_READ_FUSE_DATA_DATA_MASK                ((0xFFFFFFFF) << (OCOTP_READ_FUSE_DATA_DATA_SHIFT)) 



/* Field definitions for SW_STICKY */
#define OCOTP_SW_STICKY_SRK_REVOKE_LOCK_SHIFT         (1)  
#define OCOTP_SW_STICKY_SRK_REVOKE_LOCK_MASK          ((1) << (OCOTP_SW_STICKY_SRK_REVOKE_LOCK_SHIFT)) 

#define OCOTP_SW_STICKY_SRK_FIELD_RETURN_LOCK_SHIFT         (2)  
#define OCOTP_SW_STICKY_SRK_FIELD_RETURN_LOCK_MASK          ((1) << (OCOTP_SW_STICKY_SRK_FIELD_RETURN_LOCK_SHIFT))

#define OCOTP_SW_STICKY_BLOCK_ROM_PART_SHIFT         (3)  
#define OCOTP_SW_STICKY_BLOCK_ROM_PART_MASK          ((1) << (OCOTP_SW_STICKY_BLOCK_ROM_PART_SHIFT))  

#define OCOTP_SW_STICKY_JTAG_BLOCK_RELEASE_SHIFT       (4)  
#define OCOTP_SW_STICKY_JTAG_BLOCK_RELEASE_MASK        ((1) << (OCOTP_SW_STICKY_JTAG_BLOCK_RELEASE_SHIFT)) 



/* Field definitions for SCS */
#define OCOTP_SCS_HAB_JDE_SHIFT            (0)  
#define OCOTP_SCS_HAB_JDE_MASK             ((1) << (OCOTP_SCS_HAB_JDE_SHIFT)) 

#define OCOTP_SCS_SPARE_SHIFT              (1)
#define OCOTP_SCS_SPARE_MASK               ((0x3FFFFFFF) << (OCOTP_SCS_SPARE_SHIFT)) 

#define OCOTP_SCS_LOCK_SHIFT               (31)  
#define OCOTP_SCS_LOCK_MASK                ((1) << (OCOTP_SCS_LOCK_SHIFT)) 



/* Field definitions for SCS_SET */
#define OCOTP_SCS_SET_HAB_JDE_SHIFT            (0)  
#define OCOTP_SCS_SET_HAB_JDE_MASK             ((1) << (OCOTP_SCS_SET_HAB_JDE_SHIFT)) 

#define OCOTP_SCS_SET_SPARE_SHIFT              (1)
#define OCOTP_SCS_SET_SPARE_MASK               ((0x3FFFFFFF) << (OCOTP_SCS_SET_SPARE_SHIFT)) 

#define OCOTP_SCS_SET_LOCK_SHIFT               (31)  
#define OCOTP_SCS_SET_LOCK_MASK                ((1) << (OCOTP_SCS_SET_LOCK_SHIFT)) 



/* Field definitions for SCS_CLR */
#define OCOTP_SCS_CLR_HAB_JDE_SHIFT            (0)  
#define OCOTP_SCS_CLR_HAB_JDE_MASK             ((1) << (OCOTP_SCS_CLR_HAB_JDE_SHIFT)) 

#define OCOTP_SCS_CLR_SPARE_SHIFT              (1)
#define OCOTP_SCS_CLR_SPARE_MASK               ((0x3FFFFFFF) << (OCOTP_SCS_CLR_SPARE_SHIFT)) 

#define OCOTP_SCS_CLR_LOCK_SHIFT               (31)  
#define OCOTP_SCS_CLR_LOCK_MASK                ((1) << (OCOTP_SCS_CLR_LOCK_SHIFT)) 



/* Field definitions for SCS_TOG */
#define OCOTP_SCS_TOG_HAB_JDE_SHIFT            (0)  
#define OCOTP_SCS_TOG_HAB_JDE_MASK             ((1) << (OCOTP_SCS_TOG_HAB_JDE_SHIFT)) 

#define OCOTP_SCS_TOG_SPARE_SHIFT              (1)
#define OCOTP_SCS_TOG_SPARE_MASK               ((0x3FFFFFFF) << (OCOTP_SCS_TOG_SPARE_SHIFT)) 

#define OCOTP_SCS_TOG_LOCK_SHIFT               (31)  
#define OCOTP_SCS_TOG_LOCK_MASK                ((1) << (OCOTP_SCS_TOG_LOCK_SHIFT)) 



/* Field definitions for OCOTP_CRC_ADDR */
#define OCOTP_CRC_ADDR_DATA_START_ADDR_SHIFT          	(0)  
#define OCOTP_CRC_ADDR_DATA_START_ADDR_MASK             ((0xFF) << (OCOTP_CRC_ADDR_DATA_START_ADDR_SHIFT)) 

#define OCOTP_CRC_ADDR_DATA_END_ADDR_SHIFT          	(8)  
#define OCOTP_CRC_ADDR_DATA_END_ADDR_MASK             	((0xFF) << (OCOTP_CRC_ADDR_DATA_END_ADDR_SHIFT)) 

#define OCOTP_CRC_ADDR_CRC_ADDR_SHIFT               (16)  
#define OCOTP_CRC_ADDR_CRC_ADDR_MASK                ((0x7) << (OCOTP_CRC_ADDR_CRC_ADDR_SHIFT)) 



/* Field definitions for OCOTP_CRC_VALUE */
#define OCOTP_CRC_VALUE_DATA_SHIFT               (0)
#define OCOTP_CRC_VALUE_DATA_MASK                ((0xFFFFFFFF) << (OCOTP_CRC_VALUE_DATA_SHIFT)) 



/* Field definitions for VERSION */
#define OCOTP_VERSION_STEP_SHIFT               (0)
#define OCOTP_VERSION_STEP_MASK                ((0x0000FFFF) << (OCOTP_VERSION_STEP_SHIFT)) 

#define OCOTP_VERSION_MINOR_SHIFT              (16)
#define OCOTP_VERSION_MINOR_MASK               ((0x000000FF) << (OCOTP_VERSION_MINOR_SHIFT)) 

#define OCOTP_VERSION_MAJOR_SHIFT              (24)
#define OCOTP_VERSION_MAJOR_MASK               ((0x000000FF) << (OCOTP_VERSION_MAJOR_SHIFT)) 



/* Field definitions for LOCK */
#define OCOTP_LOCK_TESTER_SHIFT             (0)
#define OCOTP_LOCK_TESTER_MASK              ((0x00000003) << (OCOTP_LOCK_TESTER_SHIFT)) 

#define OCOTP_LOCK_BOOT_CFG_SHIFT           (2)
#define OCOTP_LOCK_BOOT_CFG_MASK            ((0x00000003) << (OCOTP_LOCK_BOOT_CFG_SHIFT)) 

#define OCOTP_LOCK_MEM_TRIM_SHIFT           (4)
#define OCOTP_LOCK_MEM_TRIM_MASK            ((0x00000003) << (OCOTP_LOCK_MEM_TRIM_SHIFT)) 

#define OCOTP_LOCK_SJC_RESP_SHIFT           (6)  
#define OCOTP_LOCK_SJC_RESP_MASK            ((1) << (OCOTP_LOCK_SJC_RESP_SHIFT)) 

#define OCOTP_LOCK_MAC_ADDR_SHIFT           (8)
#define OCOTP_LOCK_MAC_ADDR_MASK            ((0x00000003) << (OCOTP_LOCK_MAC_ADDR_SHIFT)) 

#define OCOTP_LOCK_GP1_SHIFT                (10)
#define OCOTP_LOCK_GP1_MASK                 ((0x00000003) << (OCOTP_LOCK_GP1_SHIFT)) 

#define OCOTP_LOCK_GP2_SHIFT                (12)
#define OCOTP_LOCK_GP2_MASK                 ((0x00000003) << (OCOTP_LOCK_GP2_SHIFT)) 

#define OCOTP_LOCK_SRK_SHIFT          	  (14)  
#define OCOTP_LOCK_SRK_MASK          	   ((1) << (OCOTP_LOCK_SRK_SHIFT)) 

#define OCOTP_LOCK_OTPMK_SHIFT              (17)  
#define OCOTP_LOCK_OTPMK_MASK               ((1) << (OCOTP_LOCK_OTPMK_SHIFT)) 

#define OCOTP_LOCK_ANALOG_SHIFT             (18)
#define OCOTP_LOCK_ANALOG_MASK              ((0x00000003) << (OCOTP_LOCK_ANALOG_SHIFT)) 

#define OCOTP_LOCK_PMU_SHIFT           		(20)  
#define OCOTP_LOCK_PMU_MASK            		((0x3) << (OCOTP_LOCK_PMU_SHIFT)) 

#define OCOTP_LOCK_MISC_CONF_SHIFT          (22)  
#define OCOTP_LOCK_MISC_CONF_MASK           ((1) << (OCOTP_LOCK_MISC_CONF_SHIFT)) 

#define OCOTP_LOCK_PIN_SHIFT                (25)  
#define OCOTP_LOCK_PIN_MASK                 ((1) << (OCOTP_LOCK_PIN_SHIFT)) 

#define OCOTP_LOCK_CRC_GP_LO_LOCK_SHIFT                (26)  
#define OCOTP_LOCK_CRC_GP_LO_LOCK_MASK                 ((0x3) << (OCOTP_LOCK_CRC_GP_LO_LOCK_SHIFT)) 

#define OCOTP_LOCK_CRC_GP_HI_LOCK_SHIFT                (28)  
#define OCOTP_LOCK_CRC_GP_HI_LOCK_MASK                 ((0x3) << (OCOTP_LOCK_CRC_GP_HI_LOCK_SHIFT))

#define OCOTP_LOCK_UNALLOCATED_SHIFT        (30)
#define OCOTP_LOCK_UNALLOCATED_MASK         ((0x00000003) << (OCOTP_LOCK_UNALLOCATED_SHIFT)) 



/* Field definitions for CFGn */
#define OCOTP_CFGn_BITS_SHIFT               (0)
#define OCOTP_CFGn_BITS_MASK                ((0xFFFFFFFF) << (OCOTP_CFGn_BITS_SHIFT)) 



/* Field definitions for MEMn */
#define OCOTP_MEMn_BITS_SHIFT               (0)
#define OCOTP_MEMn_BITS_MASK                ((0xFFFFFFFF) << (OCOTP_MEMn_BITS_SHIFT)) 



/* Field definitions for ANAn */
#define OCOTP_ANAn_BITS_SHIFT               (0)
#define OCOTP_ANAn_BITS_MASK                ((0xFFFFFFFF) << (OCOTP_ANAn_BITS_SHIFT)) 



/* Field definitions for OTPMKn */
#define OCOTP_OTPMKn_BITS_SHIFT               (0)
#define OCOTP_OTPMKn_BITS_MASK                ((0xFFFFFFFF) << (OCOTP_OTPMKn_BITS_SHIFT)) 



/* Field definitions for SRKn */
#define OCOTP_SRKn_BITS_SHIFT               (0)
#define OCOTP_SRKn_BITS_MASK                ((0xFFFFFFFF) << (OCOTP_SRKn_BITS_SHIFT)) 



/* Field definitions for RESPn */
#define OCOTP_RESPn_BITS_SHIFT               (0)
#define OCOTP_RESPn_BITS_MASK                ((0xFFFFFFFF) << (OCOTP_RESPn_BITS_SHIFT)) 



/* Field definitions for MACn */
#define OCOTP_MACn_BITS_SHIFT               (0)
#define OCOTP_MACn_BITS_MASK                ((0xFFFFFFFF) << (OCOTP_MACn_BITS_SHIFT)) 



/* Field definitions for GPn */
#define OCOTP_GPn_BITS_SHIFT               (0)
#define OCOTP_GPn_BITS_MASK                ((0xFFFFFFFF) << (OCOTP_GPn_BITS_SHIFT)) 



/* Field definitions for MISC_CONF */
#define OCOTP_MISC_CONF_BITS_SHIFT               (0)
#define OCOTP_MISC_CONF_BITS_MASK                ((0xFFFFFFFF) << (OCOTP_MISC_CONF_BITS_SHIFT)) 



/* Field definitions for FIELD_RETURN */
#define OCOTP_FIELD_RETURN_BITS_SHIFT               (0)
#define OCOTP_FIELD_RETURN_BITS_MASK                ((0xFFFFFFFF) << (OCOTP_FIELD_RETURN_BITS_SHIFT)) 



/* Field definitions for SRK_REVOKE */
#define OCOTP_SRK_REVOKE_BITS_SHIFT               (0)
#define OCOTP_SRK_REVOKE_BITS_MASK                ((0xFFFFFFFF) << (OCOTP_SRK_REVOKE_BITS_SHIFT)) 



/* Field definitions for TFUSEn */
#define OCOTP_TFUSEn_BITS_SHIFT               (0)
#define OCOTP_TFUSEn_BITS_MASK                ((0xFFFFFFFF) << (OCOTP_TFUSEn_BITS_SHIFT)) 



/* Field definitions for PMUR */
#define OCOTP_PMUR_BITS_SHIFT               (0)
#define OCOTP_PMUR_BITS_MASK                ((0xFFFFFFFF) << (OCOTP_PMUR_BITS_SHIFT)) 



/* Field definitions for PMU */
#define OCOTP_PMU_BITS_SHIFT               (0)
#define OCOTP_PMU_BITS_MASK                ((0xFFFFFFFF) << (OCOTP_PMU_BITS_SHIFT)) 



/* Field definitions for RNG */
#define OCOTP_RNG_BITS_SHIFT               (0)
#define OCOTP_RNG_BITS_MASK                ((0xFFFFFFFF) << (OCOTP_RNG_BITS_SHIFT)) 



/* Field definitions for VTMON */
#define OCOTP_VTMON_BITS_SHIFT               (0)
#define OCOTP_VTMON_BITS_MASK                ((0xFFFFFFFF) << (OCOTP_VTMON_BITS_SHIFT)) 



/* Field definitions for CRCn */
#define OCOTP_CRCn_BITS_SHIFT               (0)
#define OCOTP_CRCn_BITS_MASK                ((0xFFFFFFFF) << (OCOTP_CRCn_BITS_SHIFT)) 
/*** End of bit definitions ***/

/*** define base address ***/
#define OCOTP_BASE		0x400A5000

#endif
