/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "evkmimxrt1170_flexspi_nor_config.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

const flexspi_nor_config_t qspiflash_config = {
    .memConfig =
        {
            .tag              = FLEXSPI_CFG_BLK_TAG,
            .version          = FLEXSPI_CFG_BLK_VERSION,
            .readSampleClkSrc = kFlexSPIReadSampleClk_LoopbackFromDqsPad,
            .csHoldTime       = 3u,
            .csSetupTime      = 3u,
            // Enable DDR mode, Wordaddassable, Safe configuration, Differential clock
            .controllerMiscOption = 0x10,
            .deviceType           = kFlexSpiDeviceType_SerialNOR,
            .sflashPadType        = kSerialFlash_4Pads,
            .serialClkFreq        = kFlexSpiSerialClk_133MHz,
            .sflashA1Size         = 16u * 1024u * 1024u,
            .lookupTable =
                {
                    // Read LUTs
                    FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0xEB, RADDR_SDR, FLEXSPI_4PAD, 0x18),
                    FLEXSPI_LUT_SEQ(MODE8_SDR, FLEXSPI_4PAD, 0x00, DUMMY_SDR, FLEXSPI_4PAD, 0x04),
                    FLEXSPI_LUT_SEQ(READ_SDR, FLEXSPI_4PAD, 0x04, 0, 0, 0),
                },
        },
    .pageSize           = 256u,
    .sectorSize         = 4u * 1024u,
    .ipcmdSerialClkFreq = 0x1,
    .blockSize          = 256u * 1024u,
    .isUniformBlockSize = false,
};