/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
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

#include <stdlib.h>
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"

#include "fsl_sdram.h"
#include "fsl_card.h"
#include "music.h"
#include "fsl_wm8960.h"
#include "fsl_sai.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_enet.h"
#include "fsl_phy.h"
#include "fsl_gpio.h"
#include "fsl_cache.h"
#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
#include "fsl_lpi2c.h"
#else
#include "fsl_i2c.h"
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef enum FAT_Test_Items
{
    FAT_TEST_ITEM_START = 1,
    ALL_TEST = FAT_TEST_ITEM_START,
    SDRAM_RW,
    //SPI_FLASH_READID,
    EHTERNET_LOOPBACK,
    AUDIO_PLAY,
    MOTION_SENSOR_READID,
    //LCD_DISPLAY,
    //USB_HOST_UDISK,
    //USB_OTG_UDISK,
    //CAN_LOOPBACK,
    //SDCARD_RW,
    //CAMERA_CAPTURE,
    FAT_TEST_ITEM_NUM = MOTION_SENSOR_READID
} FAT_Test_Items_t;

typedef struct FAT_Test_Func
{
    uint8_t *test_item_str;
    int32_t (* test_func)(void);
} FAT_Test_Func_t;

#define DEMO_I2C LPI2C1
#define DEMO_CODEC_WM8960
#define DEMO_SAI SAI1
#define DEMO_SAI_CHANNEL (0)
#define DEMO_SAI_BITWIDTH (kSAI_WordWidth16bits)
#define DEMO_SAI_CLK_FREQ \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / ((CLOCK_GetDiv(kCLOCK_Sai1PreDiv) + 1U) * (CLOCK_GetDiv(kCLOCK_Sai1Div) + 1U)))
#define DEMO_I2C_CLK_FREQ (CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8 / (CLOCK_GetDiv(kCLOCK_Lpi2cDiv) + 1U))
#define DEMO_SAI_IRQ SAI1_IRQn
#define SAI_TxIRQHandler SAI1_IRQHandler

#define FXOS8700_WHO_AM_I_REG_ADDR      (0x0D)
#define FXOS8700_WHO_AM_I_PROD_VALUE    (0xC7)
#define FXOS8700_I2C_ADDR               (0x1F)

#define I2C_RELEASE_SDA_GPIO GPIO1
#define I2C_RELEASE_SDA_PIN 17U
#define I2C_RELEASE_SCL_GPIO GPIO1
#define I2C_RELEASE_SCL_PIN 16U
#define I2C_RELEASE_BUS_COUNT 100U
#define OVER_SAMPLE_RATE (384U)

/*! @brief Data block count accessed in card */
#define DATA_BLOCK_COUNT (5U)
/*! @brief Start data block number accessed in card */
#define DATA_BLOCK_START (2U)
/*! @brief Data buffer size */
#define DATA_BUFFER_SIZE (FSL_SDMMC_DEFAULT_BLOCK_SIZE * DATA_BLOCK_COUNT)

#define EXAMPLE_ENET ENET
#define EXAMPLE_PHY  0x02U
#define CORE_CLK_FREQ  CLOCK_GetFreq(kCLOCK_IpgClk)
#define APP_ENET_BUFF_ALIGNMENT MAX(ENET_BUFF_ALIGNMENT, FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)
#define ENET_RXBD_NUM (4)
#define ENET_TXBD_NUM (4)
#define ENET_RXBUFF_SIZE (ENET_FRAME_MAX_FRAMELEN)
#define ENET_TXBUFF_SIZE (ENET_FRAME_MAX_FRAMELEN)
#define ENET_DATA_LENGTH (1000)
#define ENET_TRANSMIT_DATA_NUM (20)
#ifndef APP_ENET_BUFF_ALIGNMENT
#define APP_ENET_BUFF_ALIGNMENT ENET_BUFF_ALIGNMENT
#endif
        
#define SDRAM_MEM_LENGTH        32*1024*1024
#define MEM_START_ADDRESS       0x80000000

/*!
* @brief printf the card information log.
*
* @param card Card descriptor.
*/
static void CardInformationLog(sd_card_t *card);

static int32_t fat_do_all_test(void);
static int32_t fat_do_sdram_rw_test(void);
static int32_t fat_do_eth_loopback_test(void);
static int32_t fat_do_audio_play_test(void);
static int32_t fat_do_motion_sensor_readid_test(void);
static int32_t fat_do_lcd_display_test(void);
static int32_t fat_do_usb_host_udisk_test(void);
static int32_t fat_do_usb_otg_udisk_test(void);
static int32_t fat_do_can_loop_back_test(void);
static int32_t fat_do_sdcard_rw_test(void);
static int32_t fat_do_camera_capture_test(void);

/*! @brief Card descriptor */
sd_card_t g_sd;

/*! @brief Data written to the card */
SDK_ALIGN(uint8_t g_dataWrite[SDK_SIZEALIGN(DATA_BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CAHCE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CAHCE, HOST_DMA_BUFFER_ADDR_ALIGN));
/*! @brief Data read from the card */
SDK_ALIGN(uint8_t g_dataRead[SDK_SIZEALIGN(DATA_BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CAHCE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CAHCE, HOST_DMA_BUFFER_ADDR_ALIGN));

/*! @brief Buffer descriptors should be in non-cacheable region and should be align to "ENET_BUFF_ALIGNMENT". */
AT_NONCACHEABLE_SECTION_ALIGN(enet_rx_bd_struct_t g_rxBuffDescrip[ENET_RXBD_NUM], ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(enet_tx_bd_struct_t g_txBuffDescrip[ENET_TXBD_NUM], ENET_BUFF_ALIGNMENT);
/*! @brief The data buffers can be in cacheable region or in non-cacheable region.
 * If use cacheable region, the alignment size should be the maximum size of "CACHE LINE SIZE" and "ENET_BUFF_ALIGNMENT"
 * If use non-cache region, the alignment size is the "ENET_BUFF_ALIGNMENT".
 */
SDK_ALIGN(uint8_t g_rxDataBuff[ENET_RXBD_NUM][SDK_SIZEALIGN(ENET_RXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT)],
          APP_ENET_BUFF_ALIGNMENT);
SDK_ALIGN(uint8_t g_txDataBuff[ENET_TXBD_NUM][SDK_SIZEALIGN(ENET_TXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT)],
          APP_ENET_BUFF_ALIGNMENT);

enet_handle_t g_handle;
uint8_t g_frame[ENET_DATA_LENGTH + 14];
uint32_t g_testTxNum = 0;

/*! @brief The MAC address for ENET device. */
uint8_t g_macAddr[6] = {0xd4, 0xbe, 0xd9, 0x45, 0x22, 0x60};
uint32_t memtest_buff[256];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
const FAT_Test_Func_t FAT_Test_Menu[FAT_TEST_ITEM_NUM + 1] =
{
    { "", NULL },
    { "Do All Tests", fat_do_all_test },
    { "SDRAM RW Test", fat_do_sdram_rw_test },
    { "Ethernet Loopback Test", fat_do_eth_loopback_test },
    { "Audio Play Test", fat_do_audio_play_test },
    { "Motion Sensor ReadID Test", fat_do_motion_sensor_readid_test },
    //{ "LCD Display Test", fat_do_lcd_display_test },
    //{ "USB Host UDisk Test", fat_do_usb_host_udisk_test },
    //{ "USB OTG UDisk Test", fat_do_usb_otg_udisk_test },
    //{ "Can Loopback Test", fat_do_can_loop_back_test },
    //{ "SDCARD RW Test", fat_do_sdcard_rw_test },
    //{ "Camera Capture Test", fat_do_camera_capture_test },
};

const clock_audio_pll_config_t g_ccmConfigAudioPll = {
    .loopDivider = 30, .postDivider = 1, .numerator = 72, .denominator = 100
};

static size_t g_index = 0;
static volatile bool isFinished = false;
#if defined(DEMO_CODEC_WM8960)
wm8960_handle_t codecHandle = {0};
#elif defined (DEMO_CODEC_DA7212)
da7212_handle_t codecHandle = {0};
#else
sgtl_handle_t codecHandle = {0};
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
static void BOARD_EnableSaiMclkOutput(bool enable)
{
    if (enable)
    {
        IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK;
    }
    else
    {
        IOMUXC_GPR->GPR1 &= (~IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK);
    }
}

static void i2c_release_bus_delay(void)
{
    uint32_t i = 0;
    for (i = 0; i < I2C_RELEASE_BUS_COUNT; i++)
    {
        __NOP();
    }
}

static void BOARD_I2C_ReleaseBus(void)
{
    uint8_t i = 0;
    gpio_pin_config_t pin_config;

    pin_config.direction = kGPIO_DigitalOutput;
    pin_config.outputLogic = 1U;
    pin_config.interruptMode = kGPIO_NoIntmode;

    GPIO_PinInit(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, &pin_config);
    GPIO_PinInit(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, &pin_config);

    /* Drive SDA low first to simulate a start */
    GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA high */
    for (i = 0; i < 9; i++)
    {
        GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
        i2c_release_bus_delay();

        GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
        i2c_release_bus_delay();

        GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
    i2c_release_bus_delay();

    GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
    i2c_release_bus_delay();
}

static status_t i2c_write(uint8_t dev_reg, uint16_t dev_addr, void *data, uint32_t dataLength)
{
    if (kStatus_Success == LPI2C_MasterStart(DEMO_I2C, dev_addr, kLPI2C_Write))
    {
        LPI2C_MasterSend(DEMO_I2C, &dev_reg, 1);
        LPI2C_MasterSend(DEMO_I2C, data, dataLength);
        LPI2C_MasterStop(DEMO_I2C);
    }
    else
        return kStatus_Fail;

    return kStatus_Success;
}

static status_t i2c_read(uint8_t dev_reg, uint16_t dev_addr, void *data, uint32_t dataLength)
{
    if (kStatus_Success == LPI2C_MasterStart(DEMO_I2C, dev_addr, kLPI2C_Write))
    {
        LPI2C_MasterSend(DEMO_I2C, &dev_reg, 1);

        LPI2C_MasterStart(DEMO_I2C, dev_addr, kLPI2C_Read);

        LPI2C_MasterReceive(DEMO_I2C, data, dataLength);
        while (LPI2C_MasterGetStatusFlags(DEMO_I2C) & kLPI2C_MasterNackDetectFlag)
        {
        }

        LPI2C_MasterStop(DEMO_I2C);
    }
    else
        return kStatus_Fail;

    return kStatus_Success;
}

void SAI_TxIRQHandler(void)
{
    uint8_t i = 0;
    uint8_t j = 0;
    uint32_t data = 0;
    uint32_t temp = 0;

    /* Clear the FIFO error flag */
    if (SAI_TxGetStatusFlag(DEMO_SAI) & kSAI_FIFOErrorFlag)
    {
        SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);
    }

    if (SAI_TxGetStatusFlag(DEMO_SAI) & kSAI_FIFOWarningFlag)
    {
        for (i = 0; i < FSL_FEATURE_SAI_FIFO_COUNT; i++)
        {
            data = 0;

            for (j = 0; j < DEMO_SAI_BITWIDTH / 8U; j++)
            {
                temp = (uint32_t)(music[g_index]);
                data |= (temp << (8U * j));
                g_index++;
            }
            SAI_WriteData(DEMO_SAI, DEMO_SAI_CHANNEL, data);
        }
    }

    if (g_index >= MUSIC_LEN)
    {
        isFinished = true;
        SAI_TxDisableInterrupts(DEMO_SAI, kSAI_FIFOWarningInterruptEnable | kSAI_FIFOErrorInterruptEnable);
        SAI_TxEnable(DEMO_SAI, false);
    }
}

static void CardInformationLog(sd_card_t *card)
{
    assert(card);

    PRINTF("\r\nCard size %d * %d bytes\r\n", card->blockCount, card->blockSize);
    PRINTF("\r\nWorking condition:\r\n");
    if (card->operationVoltage == kCARD_OperationVoltage330V)
    {
        PRINTF("\r\n  Voltage : 3.3V\r\n");
    }
    else if (card->operationVoltage == kCARD_OperationVoltage180V)
    {
        PRINTF("\r\n  Voltage : 1.8V\r\n");
    }

    if (card->currentTiming == kSD_TimingSDR12DefaultMode)
    {
        if (card->operationVoltage == kCARD_OperationVoltage330V)
        {
            PRINTF("\r\n  Timing mode: Default mode\r\n");
        }
        else if (card->operationVoltage == kCARD_OperationVoltage180V)
        {
            PRINTF("\r\n  Timing mode: SDR12 mode\r\n");
        }
    }
    else if (card->currentTiming == kSD_TimingSDR25HighSpeedMode)
    {
        if (card->operationVoltage == kCARD_OperationVoltage180V)
        {
            PRINTF("\r\n  Timing mode: SDR25\r\n");
        }
        else
        {
            PRINTF("\r\n  Timing mode: High Speed\r\n");
        }
    }
    else if (card->currentTiming == kSD_TimingSDR50Mode)
    {
        PRINTF("\r\n  Timing mode: SDR50\r\n");
    }
    else if (card->currentTiming == kSD_TimingSDR104Mode)
    {
        PRINTF("\r\n  Timing mode: SDR104\r\n");
    }
    else if (card->currentTiming == kSD_TimingDDR50Mode)
    {
        PRINTF("\r\n  Timing mode: DDR50\r\n");
    }

    PRINTF("\r\n  Freq : %d HZ\r\n", card->busClock_Hz);
}

static int32_t fat_do_all_test(void)
{
    int32_t ret_val = 0;
    int32_t err_in_test = 0;
    PRINTF("\r\n"
           "+---------------------------------------------+\r\n");
    PRINTF("+              Doing All Tests                +\r\n");
    PRINTF("+---------------------------------------------+\r\n");
    ret_val = fat_do_sdram_rw_test();
    if (ret_val > 0)
    {
        PRINTF("\r\n"
               "+v_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_v+\r\n");
        PRINTF("+              SDRAM RW Test FAIL                 +\r\n");
        PRINTF("+v_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_v+\r\n");
        err_in_test = 1;
    }
    ret_val = fat_do_eth_loopback_test();
    if (ret_val > 0)
    {
        PRINTF("\r\n"
               "+v_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_v+\r\n");
        PRINTF("+              Eth loopback Test FAIL         +\r\n");
        PRINTF("+v_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_v+\r\n");
        err_in_test = 1;
    }
    ret_val = fat_do_audio_play_test();
    if (ret_val > 0)
    {
        PRINTF("\r\n"
               "+v_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_v+\r\n");
        PRINTF("+              Audio Play Test FAIL           +\r\n");
        PRINTF("+v_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_v+\r\n");
        err_in_test = 1;
    }
    ret_val = fat_do_motion_sensor_readid_test();
    if (ret_val > 0)
    {
        PRINTF("\r\n"
               "+v_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_v+\r\n");
        PRINTF("+       Motion Sensor Readid Test FAIL        +\r\n");
        PRINTF("+v_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_vv_v+\r\n");
        err_in_test = 1;
    }

    if (0 == err_in_test)
    {
        PRINTF("+(^_*)(^_*)(^_*)(^_*)(^_*)(^_*)(^_*)(^_*)(^_*)(^_*)+\r\n");
        PRINTF("+           All Tests Done Successfully            +\r\n");
        PRINTF("+(^_*)(^_*)(^_*)(^_*)(^_*)(^_*)(^_*)(^_*)(^_*)(^_*)+\r\n");
    }
    else
    {
        PRINTF("\r\n"
               "+@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@+\r\n");
        PRINTF("+              ERRORs in All Tests            +\r\n");
        PRINTF("+@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@+\r\n");
    }
    return 0;
}

/*******************************************************************************
 * SDRAM
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void sdram_mem_write(uint32_t seed,uint32_t count)
{
  volatile uint32_t *p;
  p = (volatile uint32_t *)MEM_START_ADDRESS;
  uint32_t i;
  for(i=0;i<count;i++)
  {
    p[i] = i+seed;
  }
}
void sdram_mem_read(uint32_t *Rd,uint32_t count)
{
  volatile uint32_t *p;
  p = (volatile uint32_t *)MEM_START_ADDRESS;
  uint32_t i;
  for(i=0;i<256;i++)
  {
    Rd[i] = p[i];
  }
}

uint32_t sdram_mem_check(uint32_t seed,uint32_t count)
{
  uint32_t i;
  uint32_t errcount = 0;
  volatile uint32_t *pSdram;
  pSdram = (uint32_t *)MEM_START_ADDRESS;
  for(i=0;i<count;i++)
  {
    if(pSdram[i] != (i+seed))
    {
      errcount++;
    }
  }
  return errcount;
}

static int32_t fat_do_sdram_rw_test(void)
{
    PRINTF("\r\n========== Doing SDRAM RW Tests ===========\r\n");
    
    unsigned char bl,cl;
    uint32_t errcount;
    bl = 3;
    cl = 3;
    SDRAM_Init(bl,cl);

    PRINTF("==>> Writing seed data to SDRAM\r\n");
    sdram_mem_write(0, SDRAM_MEM_LENGTH / 4);
    PRINTF("==>> Verifying seed data on SDRAM\r\n");
    errcount = sdram_mem_check(0, SDRAM_MEM_LENGTH / 4);
    if(errcount)
    {
        PRINTF("==> sdram test fail, error count %d\r\n", errcount);
        return -1;
    }
    else
    {
        PRINTF("========== SDRAM Tests Done Successfully ===========\r\n\r\n");
        return 0;
    }
}

/*! @brief Build ENET broadcast frame. */
static void ENET_BuildBroadCastFrame(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Build Frame for transmit. */
static void ENET_BuildBroadCastFrame(void)
{
    uint32_t count = 0;
    uint32_t length = ENET_DATA_LENGTH - 14;

    for (count = 0; count < 6U; count++)
    {
        g_frame[count] = 0xFFU;
    }
    memcpy(&g_frame[6], &g_macAddr[0], 6U);
    g_frame[12] = (length >> 8) & 0xFFU;
    g_frame[13] = length & 0xFFU;

    for (count = 0; count < length; count++)
    {
        g_frame[count + 14] = count % 0xFFU;
    }
}

void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 1000000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

int32_t fat_do_eth_loopback_test(void)
{   
    enet_config_t config;
    uint32_t length = 0;
    uint32_t sysClock;
    bool link = false;
    phy_speed_t speed;
    phy_duplex_t duplex;
    uint32_t txnumber = 0;
    status_t status;
    enet_data_error_stats_t eErrStatic;
    const clock_enet_pll_config_t pll_config =
    {
       true,
       false,
       false,
       1,
       1
    };

    PRINTF("\r\n========== Doing Ethernet Loopback Tests ===========\r\n");

    /* Hardware Initialization. */
    BOARD_Ethernet_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_InitEnetPll(&pll_config);

    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    GPIO_PinInit(GPIO1, 9, &gpio_config);
    GPIO_PinInit(GPIO1, 10, &gpio_config);
    // pull up the ENET_INT before RESET
    GPIO_WritePinOutput(GPIO1, 10, 1);
    GPIO_WritePinOutput(GPIO1, 9, 0);
    delay();
    GPIO_WritePinOutput(GPIO1, 9, 1);

    SCB_DisableDCache();

    /* prepare the buffer configuration. */
    enet_buffer_config_t buffConfig = {
        ENET_RXBD_NUM,
        ENET_TXBD_NUM,
        SDK_SIZEALIGN(ENET_RXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT),
        SDK_SIZEALIGN(ENET_TXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT),
        &g_rxBuffDescrip[0],
        &g_txBuffDescrip[0],
        &g_rxDataBuff[0][0],
        &g_txDataBuff[0][0],
    };

    /* Get default configuration. */
    /*
     * config.miiMode = kENET_RmiiMode;
     * config.miiSpeed = kENET_MiiSpeed100M;
     * config.miiDuplex = kENET_MiiFullDuplex;
     * config.rxMaxFrameLen = ENET_FRAME_MAX_FRAMELEN;
     */
    ENET_GetDefaultConfig(&config);

    /* Set SMI to get PHY link status. */
    sysClock = CORE_CLK_FREQ;
    PHY_Init(EXAMPLE_ENET, EXAMPLE_PHY, sysClock);
    PHY_GetLinkStatus(EXAMPLE_ENET, EXAMPLE_PHY, &link);
    if (link)
    {
        /* Get the actual PHY link speed. */
        PHY_GetLinkSpeedDuplex(EXAMPLE_ENET, EXAMPLE_PHY, &speed, &duplex);
        /* Change the MII speed and duplex for actual link status. */
        config.miiSpeed = (enet_mii_speed_t)speed;
        config.miiDuplex = (enet_mii_duplex_t)duplex;
    }
    else
    {
        PRINTF("\r\n==>> PHY Link down, please check the cable connection and link partner setting.\r\n");
    }

    uint32_t data;
    PHY_Read(EXAMPLE_ENET, EXAMPLE_PHY, PHY_CONTROL1_REG, &data);
    PRINTF("\r\n==>> CTR1:%x\r\n", data);
    PHY_Read(EXAMPLE_ENET, EXAMPLE_PHY, PHY_CONTROL2_REG, &data);
    PRINTF("==>> CTR2:%x\r\n", data);
    PHY_Read(EXAMPLE_ENET, EXAMPLE_PHY, 0x1B, &data);
    PRINTF("==>> INTR:%x\r\n", data);
    PHY_Read(EXAMPLE_ENET, EXAMPLE_PHY, 0x0, &data);
    PRINTF("==>> BC:%x\r\n", data);
    PHY_Read(EXAMPLE_ENET, EXAMPLE_PHY, 0x1, &data);
    PRINTF("==>> BS:%x\r\n", data);

    ENET_Init(EXAMPLE_ENET, &g_handle, &config, &buffConfig, &g_macAddr[0], sysClock);
    ENET_ActiveRead(EXAMPLE_ENET);

    /* Build broadcast for sending. */
    ENET_BuildBroadCastFrame();

    while (1)
    {
        /* Get the Frame size */
        status = ENET_GetRxFrameSize(&g_handle, &length);
        /* Call ENET_ReadFrame when there is a received frame. */
        if (length != 0)
        {
            /* Received valid frame. Deliver the rx buffer with the size equal to length. */
            uint8_t *data = (uint8_t *)malloc(length);
            status = ENET_ReadFrame(EXAMPLE_ENET, &g_handle, data, length);
            if (status == kStatus_Success)
            {
                PRINTF("==>> A frame received. the length is %d\r\n", length);
                PRINTF("==>> Dest Address %02x:%02x:%02x:%02x:%02x:%02x\r\n",
                       data[0], data[1], data[2], data[3], data[4], data[5]);
                PRINTF("==>> Src  Address %02x:%02x:%02x:%02x:%02x:%02x\r\n",
                       data[6], data[7], data[8], data[9], data[10], data[11]);
            }
            free(data);
            PRINTF("========== Ethernet Loopback Tests Done Successfully ===========\r\n\r\n");
            SCB_EnableDCache();
            return 0;
        }
        else if (status == kStatus_ENET_RxFrameError)
        {
            /* Update the received buffer when error happened. */
            /* Get the error information of the received g_frame. */
            ENET_GetRxErrBeforeReadFrame(&g_handle, &eErrStatic);
            /* update the receive buffer. */
            ENET_ReadFrame(EXAMPLE_ENET, &g_handle, NULL, 0);
        }

        if (g_testTxNum < ENET_TRANSMIT_DATA_NUM)
        {
            /* Send a multicast frame when the PHY is link up. */
            if (kStatus_Success == PHY_GetLinkStatus(EXAMPLE_ENET, EXAMPLE_PHY, &link))
            {
                if (link)
                {
                    g_testTxNum++;
                    txnumber++;
                    if (kStatus_Success == ENET_SendFrame(EXAMPLE_ENET, &g_handle, &g_frame[0], ENET_DATA_LENGTH))
                    {
                        PRINTF("==>> %d frame transmitted success!\r\n", txnumber);
                    }
                    else
                    {
                        PRINTF("==>> Transmit frame failed!\r\n");
                        SCB_EnableDCache();
                        return 1;
                    }
                }
                else
                {
                  PRINTF("==> Transmit frame failed!\r\n");
                  SCB_EnableDCache();
                  return 1;
                }
            }
        }
    }
}

bool g_HeadSetInputSignal = false;
void GPIO1_Combined_16_31_IRQHandler(void)
{
    /* clear the interrupt status */
    GPIO_ClearPinsInterruptFlags(GPIO1, 1U << 24);
    if(GPIO_ReadPinInput(GPIO1, 24))
      g_HeadSetInputSignal = true;
    else
      g_HeadSetInputSignal = false;
}

static int32_t fat_do_audio_play_test(void)
{
    sai_config_t config;
    uint32_t mclkSourceClockHz = 0U;
    sai_transfer_format_t format;
#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    lpi2c_master_handle_t i2cHandle;
    lpi2c_master_config_t i2cConfig = {0};
#else
    i2c_master_handle_t i2cHandle;
    i2c_master_config_t i2cConfig = {0};
#endif
    uint32_t i2cSourceClock;
    int32_t ret_val = 0;

    PRINTF("\r\n========== Doing Audio Play Tests ===========\r\n");
    BOARD_I2C_ReleaseBus();
    BOARD_I2C_ConfigurePins();
    BOARD_EnableSaiMclkOutput(true);

    CLOCK_SetMux(kCLOCK_Lpi2cMux, 0);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, 0);
    
    /* Enable PLL4 368.64M */
    CLOCK_InitAudioPll(&g_ccmConfigAudioPll);
    /* Set SAI source to PLL4*/
    CLOCK_SetMux(kCLOCK_Sai1Mux, 2);
    /* Set SAI pre-divider to 6 */
    CLOCK_SetDiv(kCLOCK_Sai1PreDiv, 5U);
    /* Set SAI divider to 10 */
    CLOCK_SetDiv(kCLOCK_Sai1Div, 9U);
    
    /*
     * config.masterSlave = kSAI_Master;
     * config.mclkSource = kSAI_MclkSourceSysclk;
     * config.protocol = kSAI_BusLeftJustified;
     * config.syncMode = kSAI_ModeAsync;
     * config.mclkOutputEnable = true;
     */
    SAI_TxGetDefaultConfig(&config);
    SAI_TxInit(DEMO_SAI, &config);

    /* Configure the audio format */
    format.bitWidth = DEMO_SAI_BITWIDTH;
    format.channel = DEMO_SAI_CHANNEL;
    format.sampleRate_Hz = kSAI_SampleRate16KHz;
#if (defined FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER && FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) || \
    (defined FSL_FEATURE_PCC_HAS_SAI_DIVIDER && FSL_FEATURE_PCC_HAS_SAI_DIVIDER)
    format.masterClockHz = OVER_SAMPLE_RATE * format.sampleRate_Hz;
#else
    format.masterClockHz = DEMO_SAI_CLK_FREQ;
#endif
    format.protocol = config.protocol;
    format.stereo = kSAI_Stereo;

    /* Configure Codec I2C */
    codecHandle.base = DEMO_I2C;
    codecHandle.i2cHandle = &i2cHandle;
    i2cSourceClock = DEMO_I2C_CLK_FREQ;

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    /*
     * i2cConfig.debugEnable = false;
     * i2cConfig.ignoreAck = false;
     * i2cConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * i2cConfig.baudRate_Hz = 100000U;
     * i2cConfig.busIdleTimeout_ns = 0;
     * i2cConfig.pinLowTimeout_ns = 0;
     * i2cConfig.sdaGlitchFilterWidth_ns = 0;
     * i2cConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&i2cConfig);
    LPI2C_MasterInit(DEMO_I2C, &i2cConfig, i2cSourceClock);
    LPI2C_MasterTransferCreateHandle(DEMO_I2C, &i2cHandle, NULL, NULL);
#else
    /*
     * i2cConfig.baudRate_Bps = 100000U;
     * i2cConfig.enableStopHold = false;
     * i2cConfig.glitchFilterWidth = 0U;
     * i2cConfig.enableMaster = true;
     */
    I2C_MasterGetDefaultConfig(&i2cConfig);
    I2C_MasterInit(DEMO_I2C, &i2cConfig, i2cSourceClock);
    I2C_MasterTransferCreateHandle(DEMO_I2C, &i2cHandle, NULL, NULL);
#endif

    WM8960_Init(&codecHandle, NULL);
    WM8960_ConfigDataFormat(&codecHandle, format.masterClockHz, format.sampleRate_Hz, format.bitWidth);
    WM8960_SetVolume(&codecHandle, kWM8960_ModuleDAC, 0xFF); 

    mclkSourceClockHz = DEMO_SAI_CLK_FREQ;
    SAI_TxSetFormat(DEMO_SAI, &format, mclkSourceClockHz, format.masterClockHz);
    WM8960_SetModule(&codecHandle, kWM8960_ModuleHP, true);
    WM8960_SetVolume(&codecHandle, kWM8960_ModuleHP, 0x6F);
    WM8960_SetModule(&codecHandle, kWM8960_ModuleSpeaker, 1);
    WM8960_SetVolume(&codecHandle, kWM8960_ModuleSpeaker, 0x6F);

    
/* Define the init structure for the input switch pin */
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput, 0,
        kGPIO_IntRisingOrFallingEdge,
    };

    IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_08_GPIO1_IO24,        /* GPIO_B1_08 is configured as GPIO1_IO24 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
      
    IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B1_08_GPIO1_IO24,        /* GPIO_B1_08 PAD functional properties : */
      0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */  
    
        /* Init input switch GPIO. */
    EnableIRQ(GPIO1_Combined_16_31_IRQn);
    GPIO_PinInit(GPIO1, 24, &sw_config);

    /* Enable GPIO pin interrupt */  
    GPIO_EnableInterrupts(GPIO1, 1U << 24);

    /*  Enable interrupt */
    EnableIRQ(DEMO_SAI_IRQ);

    /* Headphone test */
    PRINTF("==>> Please plug in headphones to the HEADPHONE OUT jack\r\n");
    
    while(g_HeadSetInputSignal == false)
    {
    }
    
    PRINTF("==>> Please listen to the music from headphone\r\n");
    
    do {
        uint8_t ch_in = 0;
        SAI_TxEnableInterrupts(DEMO_SAI, kSAI_FIFOWarningInterruptEnable | kSAI_FIFOErrorInterruptEnable);
        SAI_TxEnable(DEMO_SAI, true);

        /* Wait until finished */
        while (isFinished != true)
        {
        }
        PRINTF("==>> Do you need to hear it again? (y/n) ");
        ch_in = GETCHAR();
        PUTCHAR(ch_in);
        PRINTF("\r\n");
        if ('y' == ch_in)
        {
            g_index = 0;
            isFinished = false;
            continue;
        }
        else if ('n' == ch_in)
        {
            PRINTF("==>> Do you hear audio from headphone? (y/n) ");
            ch_in = GETCHAR();
            PUTCHAR(ch_in);
            PRINTF("\r\n");
            if ('y' == ch_in)
            {
                PRINTF("========== SAI I2S Playback Headphone Test PASS. ===========\r\n");
            }
            else if ('n' == ch_in)
            {
                PRINTF("########## SAI I2S Playback Headphone Test FAIL! ##########\r\n");
                ret_val = -1;
            }
            break;
        }
    } while (1);

    PRINTF("==>> Please remove the headphones from the HEADPHONE OUT jack\r\n");
    isFinished = false;
    
    while(g_HeadSetInputSignal == true)
    {
    }
    
    PRINTF("==>> Please listen to the music from speaker\r\n");
    
    do {
        uint8_t ch_in = 0;
        SAI_TxEnableInterrupts(DEMO_SAI, kSAI_FIFOWarningInterruptEnable | kSAI_FIFOErrorInterruptEnable);
        SAI_TxEnable(DEMO_SAI, true);

        /* Wait until finished */
        while (isFinished != true)
        {
        }
        PRINTF("==>> Do you need to hear it again? (y/n) ");
        ch_in = GETCHAR();
        PUTCHAR(ch_in);
        PRINTF("\r\n");
        if ('y' == ch_in)
        {
            g_index = 0;
            isFinished = false;
            continue;
        }
        else if ('n' == ch_in)
        {
            PRINTF("==>> Do you hear audio from speaker? (y/n) ");
            ch_in = GETCHAR();
            PUTCHAR(ch_in);
            PRINTF("\r\n");
            if ('y' == ch_in)
            {
                PRINTF("========== SAI I2S Playback Speaker Test PASS. ===========\r\n\r\n");
            }
            else if ('n' == ch_in)
            {
                PRINTF("########## SAI I2S Playback Speaker Test FAIL! ##########r\n\r\n");
                ret_val = -1;
            }
            break;
        }
    } while (1);
 
    return ret_val;
}

static int32_t fat_do_motion_sensor_readid_test(void)
{
    uint32_t i2cSourceClock = DEMO_I2C_CLK_FREQ;
#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    lpi2c_master_config_t i2cConfig = {0};
    lpi2c_master_handle_t i2cHandle;
#else
    i2c_master_config_t i2cConfig = {0};
    i2c_master_handle_t i2cHandle;
#endif
    int32_t status = 0;
    uint8_t fxos_whoami = 0;

    PRINTF("\r\n========== Doing Motion Sensor ReadID Tests ===========\r\n");
    BOARD_I2C_ReleaseBus();
    BOARD_I2C_ConfigurePins();

    CLOCK_SetMux(kCLOCK_Lpi2cMux, 0);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, 0);
    
    #if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    /*
     * i2cConfig.debugEnable = false;
     * i2cConfig.ignoreAck = false;
     * i2cConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * i2cConfig.baudRate_Hz = 100000U;
     * i2cConfig.busIdleTimeout_ns = 0;
     * i2cConfig.pinLowTimeout_ns = 0;
     * i2cConfig.sdaGlitchFilterWidth_ns = 0;
     * i2cConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&i2cConfig);
    LPI2C_MasterInit(DEMO_I2C, &i2cConfig, i2cSourceClock);
    LPI2C_MasterTransferCreateHandle(DEMO_I2C, &i2cHandle, NULL, NULL);
#else
    /*
     * i2cConfig.baudRate_Bps = 100000U;
     * i2cConfig.enableStopHold = false;
     * i2cConfig.glitchFilterWidth = 0U;
     * i2cConfig.enableMaster = true;
     */
    I2C_MasterGetDefaultConfig(&i2cConfig);
    I2C_MasterInit(DEMO_I2C, &i2cConfig, i2cSourceClock);
    I2C_MasterTransferCreateHandle(DEMO_I2C, &i2cHandle, NULL, NULL);
#endif
    
    /*!  Read and store the device's WHO_AM_I.*/
    PRINTF("==>> Try to read sensor ID\r\n");
    status = i2c_read(FXOS8700_WHO_AM_I_REG_ADDR, FXOS8700_I2C_ADDR, &fxos_whoami, 1);

    if ((kStatus_Success != status) || (fxos_whoami != FXOS8700_WHO_AM_I_PROD_VALUE))
    {
        PRINTF("########## Motion Sensor ReadID Tests FAIL ##########\r\n");
    }
    else
    {
        PRINTF("========== Motion Sensor ReadID Tests PASS (0x%02x) ===========\r\n\r\n", fxos_whoami);
    }
    return 0;
}

static int32_t fat_do_lcd_display_test(void)
{
    PRINTF("\r\nLCD Display Test not supported yet\r\n\r\n");
    return 1;
}

static int32_t fat_do_usb_host_udisk_test(void)
{
    PRINTF("\r\nUSB Host Test not supported yet\r\n\r\n");
    return 1;
}

static int32_t fat_do_usb_otg_udisk_test(void)
{
    PRINTF("\r\nUSB OTG Test not supported yet\r\n\r\n");
    return 1;
}

static int32_t fat_do_can_loop_back_test(void)
{
    PRINTF("\r\nCAN Test not supported yet\r\n\r\n");
    return 1;
}

static int32_t fat_do_sdcard_rw_test(void)
{
    sd_card_t *card = &g_sd;
    bool isReadOnly;
    uint8_t ch_in = 0;

    PRINTF("\r\n========== Doing SDCARD RW Tests ===========\r\n");
    /* Save host information. */
    card->host.base = SD_HOST_BASEADDR;
    card->host.sourceClock_Hz = SD_HOST_CLK_FREQ;

    PRINTF("==>> Please insert SD card into Slot SD0\r\n");
    do {
        PRINTF("==>> Please enter y or Y to confirm: ");
        ch_in = GETCHAR();
        if (('y' == ch_in) || ('Y' == ch_in))
        {
            break;
        }
    } while (1);

    /* Init card. */
    PRINTF("==>> 1. Init card\r\n");
    if (SD_Init(card))
    {
        PRINTF("==>> SD card init failed.\r\n");
        return 1;
    }
    PRINTF("==>>\tCard on SD0 is detected\r\n");
    /* card information log */
    CardInformationLog(card);

    /* Check if card is readonly. */
    isReadOnly = SD_CheckReadOnly(card);
    PRINTF("==>>\tCard on SD0 is %swrite protected\r\n", isReadOnly ? "" : "not ");
#if 0
    if (isReadOnly)
    {
        while (true)
        {
            if (failedFlag || (ch == 'q'))
            {
                break;
            }

            PRINTF("\r\nRead one data block......\r\n");
            if (kStatus_Success != SD_ReadBlocks(card, g_dataRead, DATA_BLOCK_START, 1U))
            {
                PRINTF("Read one data block failed.\r\n");
                failedFlag = true;
                continue;
            }

            PRINTF("Read multiple data blocks......\r\n");
            if (kStatus_Success != SD_ReadBlocks(card, g_dataRead, DATA_BLOCK_START, DATA_BLOCK_COUNT))
            {
                PRINTF("Read multiple data blocks failed.\r\n");
                failedFlag = true;
                continue;
            }

            PRINTF(
                "\r\nInput 'q' to quit read process.\
                \r\nInput other char to read data blocks again.\r\n");
            ch = GETCHAR();
            PUTCHAR(ch);
        }
    }
    else
    {
        memset(g_dataWrite, 0x67U, sizeof(g_dataWrite));

        while (true)
        {
            if (failedFlag || (ch == 'q'))
            {
                break;
            }

            PRINTF("\r\nWrite/read one data block......\r\n");
            if (kStatus_Success != SD_WriteBlocks(card, g_dataWrite, DATA_BLOCK_START, 1U))
            {
                PRINTF("Write one data block failed.\r\n");
                failedFlag = true;
                continue;
            }

            memset(g_dataRead, 0U, sizeof(g_dataRead));
            if (kStatus_Success != SD_ReadBlocks(card, g_dataRead, DATA_BLOCK_START, 1U))
            {
                PRINTF("Read one data block failed.\r\n");
                failedFlag = true;
                continue;
            }

            PRINTF("Compare the read/write content......\r\n");
            if (memcmp(g_dataRead, g_dataWrite, FSL_SDMMC_DEFAULT_BLOCK_SIZE))
            {
                PRINTF("The read/write content isn't consistent.\r\n");
                failedFlag = true;
                continue;
            }
            PRINTF("The read/write content is consistent.\r\n");

            PRINTF("Write/read multiple data blocks......\r\n");
            if (kStatus_Success != SD_WriteBlocks(card, g_dataWrite, DATA_BLOCK_START, DATA_BLOCK_COUNT))
            {
                PRINTF("Write multiple data blocks failed.\r\n");
                failedFlag = true;
                continue;
            }

            memset(g_dataRead, 0U, sizeof(g_dataRead));
            if (kStatus_Success != SD_ReadBlocks(card, g_dataRead, DATA_BLOCK_START, DATA_BLOCK_COUNT))
            {
                PRINTF("Read multiple data blocks failed.\r\n");
                failedFlag = true;
                continue;
            }

            PRINTF("Compare the read/write content......\r\n");
            if (memcmp(g_dataRead, g_dataWrite, (FSL_SDMMC_DEFAULT_BLOCK_SIZE * DATA_BLOCK_COUNT)))
            {
                PRINTF("The read/write content isn't consistent.\r\n");
                failedFlag = true;
                continue;
            }
            PRINTF("The read/write content is consistent.\r\n");

            PRINTF("Erase multiple data blocks......\r\n");
            if (kStatus_Success != SD_EraseBlocks(card, DATA_BLOCK_START, DATA_BLOCK_COUNT))
            {
                PRINTF("Erase multiple data blocks failed.\r\n");
                failedFlag = true;
                continue;
            }

            PRINTF(
                "\r\nInput 'q' to quit read/write/erase process.\
                \r\nInput other char to read/write/erase data blocks again.\r\n");
            ch = GETCHAR();
            PUTCHAR(ch);
        }
    }
#endif

    SD_Deinit(card);

    PRINTF("========== SDCARD RW Tests Done Successfully ===========\r\n\r\n");
    return 0;
}

static int32_t fat_do_camera_capture_test(void)
{
    PRINTF("\r\nCamera Capture Test not supported yet\r\n\r\n");
    return 1;
}

static int32_t fat_tool_do_test_func(int32_t func_index)
{
    return FAT_Test_Menu[func_index].test_func();
}

static int32_t fat_tool_get_menu_input(void)
{
    int32_t menu_input_index = 0;

    while (1)
    {
        PRINTF("Please input index for test (1 - %d), 0 for restart: ", FAT_TEST_ITEM_NUM);
        SCANF("%d", &menu_input_index);
        if (menu_input_index < 0 || menu_input_index > FAT_TEST_ITEM_NUM)
        {
            PRINTF("\r\nWrong input index: %d!\r\n", menu_input_index);
            continue;
        }
        break;
    }
    return menu_input_index;
}

static int32_t fat_tool_show_menu(void)
{
    int32_t tmp_index = 0;

    PRINTF("\r\n");
    for (tmp_index = FAT_TEST_ITEM_START; tmp_index <= FAT_TEST_ITEM_NUM; ++tmp_index)
    {
        PRINTF("%d - %s\r\n", tmp_index, FAT_Test_Menu[tmp_index].test_item_str);
    }
    return 0;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Print info */
    PRINTF("***********************************************************\r\n");
    PRINTF("\tFAT Tool for iMXRT1050\r\n");
    PRINTF("***********************************************************\r\n");
    PRINTF("\r\n");
    PRINTF("***********************************************************\r\n");
    PRINTF("CPU:        %d MHz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
    PRINTF("SDR:        %d MHz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
    PRINTF("IPG:        %d MHz\r\n", CLOCK_GetFreq(kCLOCK_IpgClk));
    PRINTF("DEBUG UART: %d MHz\r\n", BOARD_DebugConsoleSrcFreq());
    PRINTF("***********************************************************\r\n");
    PRINTF("\r\n");

    while (1)
    {
        int32_t menu_input_index = 0, tmp_index = 0;
        int32_t ret_val = 0;

        /* Show menu */
        fat_tool_show_menu();
        menu_input_index = fat_tool_get_menu_input();
        PRINTF("\r\n");
        if (0 == menu_input_index)
        {
            continue;
        }
        ret_val = fat_tool_do_test_func(menu_input_index);
        if (ret_val < 0)
        {
            PRINTF("##################################################\r\n");
            PRINTF("%s FAILED!\r\n", FAT_Test_Menu[tmp_index].test_item_str);
            PRINTF("##################################################\r\n");
        }
        else if (ret_val > 0)
        {
            PRINTF("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\r\n");
            PRINTF("%s Not Supported yet!\r\n", FAT_Test_Menu[tmp_index].test_item_str);
            PRINTF("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\r\n");
        }
    }
}
