/*
* Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
* All rights reserved.
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

#include "fsl_edma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define EDMA_TRANSFER_ENABLED_MASK 0x80U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Get instance number for EDMA.
 *
 * @param base EDMA peripheral base address.
 */
static uint32_t EDMA_GetInstance(DMA_Type *base);

/*!
 * @brief Push content of TCD structure into hardware TCD register.
 *
 * @param base EDMA peripheral base address.
 * @param channel EDMA channel number.
 * @param tcd Point to TCD structure.
 */
static void EDMA_InstallTCD(DMA_Type *base, uint32_t channel, edma_tcd_t *tcd);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Array to map EDMA instance number to base pointer. */
static DMA_Type *const s_edmaBases[] = DMA_BASE_PTRS;

/*! @brief Array to map EDMA instance number to clock name. */
static const clock_ip_name_t s_edmaClockName[] = EDMA_CLOCKS;

#if defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT == 1U)
/*! @brief Array to map EDMA instance number to IRQ number. */
static const IRQn_Type s_edmaIRQNumber[] = DMA_IRQS;
#elif defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 1U)
/*! @brief Array to map EDMA instance number to IRQ number. */
static const IRQn_Type s_edmaIRQNumber[][FSL_FEATURE_EDMA_MODULE_CHANNEL] = DMA_IRQS;
#endif

/*! @brief Pointers to transfer handle for each EDMA channel. */
static edma_handle_t *s_EDMAHandle[FSL_FEATURE_EDMA_MODULE_CHANNEL * FSL_FEATURE_SOC_EDMA_COUNT];

/*******************************************************************************
 * Code
 ******************************************************************************/

static uint32_t EDMA_GetInstance(DMA_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < FSL_FEATURE_SOC_EDMA_COUNT; instance++)
    {
        if (s_edmaBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < FSL_FEATURE_SOC_EDMA_COUNT);

    return instance;
}

static void EDMA_InstallTCD(DMA_Type *base, uint32_t channel, edma_tcd_t *tcd)
{
    assert(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);
    assert(tcd != NULL);
    assert(((uint32_t)tcd & 0x1FU) == 0);

    /* Push tcd into hardware TCD register */
    base->CH[channel].TCD_SADDR = tcd->SADDR;
    base->CH[channel].TCD_SOFF = tcd->SOFF;
    base->CH[channel].TCD_ATTR = tcd->ATTR;
    base->CH[channel].TCD_NBYTES_MLOFFNO = tcd->NBYTES;
    base->CH[channel].TCD_SLAST_SDA = tcd->SLAST;
    base->CH[channel].TCD_DADDR = tcd->DADDR;
    base->CH[channel].TCD_DOFF = tcd->DOFF;
    base->CH[channel].TCD_CITER_ELINKNO = tcd->CITER;
    base->CH[channel].TCD_DLAST_SGA = tcd->DLAST_SGA;
    /* Clear DONE bit first, otherwise ESG cannot be set */
    base->CH[channel].TCD_CSR = 0;
    base->CH[channel].TCD_CSR = tcd->CSR;
    base->CH[channel].TCD_BITER_ELINKNO = tcd->BITER;
}

void EDMA_Init(DMA_Type *base, const edma_config_t *config)
{
    assert(config != NULL);

    uint32_t tmpreg;
    uint32_t channel;

    /* Configure EDMA peripheral according to the configuration structure. */
    tmpreg = base->MP_CSR;
    tmpreg &= ~(DMA_MP_CSR_EMI_MASK | DMA_MP_CSR_HAE_MASK | DMA_MP_CSR_ERCA_MASK | DMA_MP_CSR_EDBG_MASK |
                DMA_MP_CSR_EBW_MASK);
    tmpreg |= (DMA_MP_CSR_EMI(config->enableMasterIdReplication) | DMA_MP_CSR_HAE(config->enableHaltOnError) |
               DMA_MP_CSR_ERCA(config->enableRoundRobinArbitration) | DMA_MP_CSR_EDBG(config->enableDebugMode) |
               DMA_MP_CSR_EBW(config->enableBufferedWrites));
    base->MP_CSR = tmpreg;
    base->MP_CSR &= (~DMA_MP_CSR_HALT_MASK);
    for (channel = 0; channel < FSL_FEATURE_EDMA_MODULE_CHANNEL; channel++)
    {
        base->CH[channel].CH_ES |= DMA_CH_ES_ERR_MASK;
    }
}

void EDMA_Deinit(DMA_Type *base)
{
    /* Gate EDMA periphral clock */
    CLOCK_DisableClock(s_edmaClockName[EDMA_GetInstance(base)]);
}

void EDMA_GetDefaultConfig(edma_config_t *config)
{
    assert(config != NULL);

    config->enableMasterIdReplication = true;
    config->enableHaltOnError = true;
    config->enableRoundRobinArbitration = false;
    config->enableDebugMode = false;
    config->enableBufferedWrites = false;
}

void EDMA_ResetChannel(DMA_Type *base, uint32_t channel)
{
    assert(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);

    EDMA_TcdReset((edma_tcd_t *)((uint32_t)&base->CH[channel] + 0x00000020));
}

void EDMA_SetTransferConfig(DMA_Type *base, uint32_t channel, const edma_transfer_config_t *config, edma_tcd_t *nextTcd)
{
    assert(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);
    assert(config != NULL);
    assert(((uint32_t)nextTcd & 0x1FU) == 0);

    EDMA_TcdSetTransferConfig((edma_tcd_t *)((uint32_t)&base->CH[channel] + 0x00000020), config, nextTcd);
}

void EDMA_SetMinorOffsetConfig(DMA_Type *base, uint32_t channel, const edma_minor_offset_config_t *config)
{
    assert(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);
    assert(config != NULL);

    EDMA_TcdSetMinorOffsetConfig((edma_tcd_t *)((uint32_t)&base->CH[channel] + 0x00000020), config);
}

void EDMA_SetChannelLink(DMA_Type *base, uint32_t channel, edma_channel_link_type_t type, uint32_t linkedChannel)
{
    assert(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);
    assert(linkedChannel < FSL_FEATURE_EDMA_MODULE_CHANNEL);

    EDMA_TcdSetChannelLink((edma_tcd_t *)((uint32_t)&base->CH[channel] + 0x00000020), type, linkedChannel);
}

void EDMA_SetBandWidth(DMA_Type *base, uint32_t channel, edma_bandwidth_t bandWidth)
{
    assert(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);

    base->CH[channel].TCD_CSR =
        (base->CH[channel].TCD_CSR & (~DMA_TCD_CSR_BWC_MASK)) | DMA_TCD_CSR_BWC(bandWidth);
}

void EDMA_SetModulo(DMA_Type *base, uint32_t channel, edma_modulo_t srcModulo, edma_modulo_t destModulo)
{
    assert(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);

    EDMA_TcdSetModulo((edma_tcd_t *)((uint32_t)&base->CH[channel] + 0x00000020), srcModulo, destModulo);
}

void EDMA_EnableChannelInterrupts(DMA_Type *base, uint32_t channel, uint32_t mask)
{
    assert(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);

    /* Enable error interrupt */
    if (mask & kEDMA_ErrorInterruptEnable)
    {
        base->CH[channel].CH_CSR = (base->CH[channel].CH_CSR & (~DMA_CH_CSR_DONE_MASK)) | DMA_CH_CSR_EEI_MASK;
    }

    /* Enable Major interrupt or Half major interrupt */
    else
    {
        EDMA_TcdEnableInterrupts((edma_tcd_t *)&base->CH[channel], mask);
    }
}

void EDMA_DisableChannelInterrupts(DMA_Type *base, uint32_t channel, uint32_t mask)
{
    assert(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);

    /* Disable error interrupt */
    if (mask & kEDMA_ErrorInterruptEnable)
    {
        base->CH[channel].CH_CSR = (base->CH[channel].CH_CSR & (~DMA_CH_CSR_DONE_MASK)) & ~DMA_CH_CSR_EEI_MASK;
    }

    /* Disable Major interrupt or Half major interrupt */
    else
    {
        EDMA_TcdDisableInterrupts((edma_tcd_t *)&base->CH[channel], mask);
    }
}

void EDMA_TcdReset(edma_tcd_t *tcd)
{
    assert(tcd != NULL);
    assert(((uint32_t)tcd & 0x1FU) == 0);

    /* Reset channel TCD */
    tcd->SADDR = 0U;
    tcd->SOFF = 0U;
    tcd->ATTR = 0U;
    tcd->NBYTES = 0U;
    tcd->SLAST = 0U;
    tcd->DADDR = 0U;
    tcd->DOFF = 0U;
    tcd->CITER = 0U;
    tcd->DLAST_SGA = 0U;
    /* Enable auto disable request feature */
    tcd->CSR = DMA_TCD_CSR_DREQ(true);
    tcd->BITER = 0U;
}

void EDMA_TcdSetTransferConfig(edma_tcd_t *tcd, const edma_transfer_config_t *config, edma_tcd_t *nextTcd)
{
    assert(tcd != NULL);
    assert(((uint32_t)tcd & 0x1FU) == 0);
    assert(config != NULL);
    assert(((uint32_t)nextTcd & 0x1FU) == 0);

    /* source address */
    tcd->SADDR = config->srcAddr;
    /* destination address */
    tcd->DADDR = config->destAddr;
    /* Source data and destination data transfer size */
    tcd->ATTR = DMA_TCD_ATTR_SSIZE(config->srcTransferSize) | DMA_TCD_ATTR_DSIZE(config->destTransferSize);
    /* Source address signed offset */
    tcd->SOFF = config->srcOffset;
    /* Destination address signed offset */
    tcd->DOFF = config->destOffset;
    /* Minor byte transfer count */
    tcd->NBYTES = config->minorLoopBytes;
    /* Current major iteration count */
    tcd->CITER = config->majorLoopCounts;
    /* Store destination address to system memory and end-of-packet processing option */
    tcd->CSR |= DMA_TCD_CSR_ESDA(config->enableStoreDestAddr) | DMA_TCD_CSR_EEOP(config->enableEndOfPacketProcessing);
    /* Starting major iteration count */
    tcd->BITER = config->majorLoopCounts;
    /* Enable scatter/gather processing */
    if (nextTcd != NULL)
    {
        tcd->DLAST_SGA = (uint32_t)nextTcd;
        /*
            Before call EDMA_TcdSetTransferConfig or EDMA_SetTransferConfig,
            user must call EDMA_TcdReset or EDMA_ResetChannel which will set
            DREQ, so must use "|" or "&" rather than "=".

            Clear the DREQ bit because scatter gather has been enabled, so the
            previous transfer is not the last transfer, and channel request should
            be enabled at the next transfer(the next TCD).
        */
        tcd->CSR = (tcd->CSR | DMA_TCD_CSR_ESG_MASK) & ~DMA_TCD_CSR_DREQ_MASK;
    }
}

void EDMA_TcdSetMinorOffsetConfig(edma_tcd_t *tcd, const edma_minor_offset_config_t *config)
{
    assert(tcd != NULL);
    assert(((uint32_t)tcd & 0x1FU) == 0);

    uint32_t tmpreg;

    tmpreg = tcd->NBYTES;
    tmpreg &=
        ~(DMA_TCD_NBYTES_MLOFFYES_SMLOE_MASK | DMA_TCD_NBYTES_MLOFFYES_DMLOE_MASK | DMA_TCD_NBYTES_MLOFFYES_MLOFF_MASK);
    tmpreg |= (DMA_TCD_NBYTES_MLOFFYES_SMLOE(config->enableSrcMinorOffset) |
               DMA_TCD_NBYTES_MLOFFYES_DMLOE(config->enableDestMinorOffset) |
               DMA_TCD_NBYTES_MLOFFYES_MLOFF(config->minorOffset));
    tcd->NBYTES = tmpreg;
}

void EDMA_TcdSetChannelLink(edma_tcd_t *tcd, edma_channel_link_type_t type, uint32_t linkedChannel)
{
    assert(tcd != NULL);
    assert(((uint32_t)tcd & 0x1FU) == 0);
    assert(linkedChannel < FSL_FEATURE_EDMA_MODULE_CHANNEL);

    if (type == kEDMA_MinorLink) /* Minor link config */
    {
        uint32_t tmpreg;

        /* Enable minor link */
        tcd->CITER |= DMA_TCD_CITER_ELINKYES_ELINK_MASK;
        tcd->BITER |= DMA_TCD_BITER_ELINKYES_ELINK_MASK;
        /* Set likned channel */
        tmpreg = tcd->CITER & (~DMA_TCD_CITER_ELINKYES_LINKCH_MASK);
        tmpreg |= DMA_TCD_CITER_ELINKYES_LINKCH(linkedChannel);
        tcd->CITER = tmpreg;
        tmpreg = tcd->BITER & (~DMA_TCD_BITER_ELINKYES_LINKCH_MASK);
        tmpreg |= DMA_TCD_BITER_ELINKYES_LINKCH(linkedChannel);
        tcd->BITER = tmpreg;
    }
    else if (type == kEDMA_MajorLink) /* Major link config */
    {
        uint32_t tmpreg;

        /* Enable major link */
        tcd->CSR |= DMA_TCD_CSR_MAJORELINK_MASK;
        /* Set major linked channel */
        tmpreg = tcd->CSR & (~DMA_TCD_CSR_MAJORLINKCH_MASK);
        tcd->CSR = tmpreg | DMA_TCD_CSR_MAJORLINKCH(linkedChannel);
    }
    else /* Link none */
    {
        tcd->CITER &= ~DMA_TCD_CITER_ELINKYES_ELINK_MASK;
        tcd->BITER &= ~DMA_TCD_BITER_ELINKYES_ELINK_MASK;
        tcd->CSR &= ~DMA_TCD_CSR_MAJORELINK_MASK;
    }
}

void EDMA_TcdSetModulo(edma_tcd_t *tcd, edma_modulo_t srcModulo, edma_modulo_t destModulo)
{
    assert(tcd != NULL);
    assert(((uint32_t)tcd & 0x1FU) == 0);

    uint32_t tmpreg;

    tmpreg = tcd->ATTR & (~(DMA_TCD_ATTR_SMOD_MASK | DMA_TCD_ATTR_DMOD_MASK));
    tcd->ATTR = tmpreg | DMA_TCD_ATTR_DMOD(destModulo) | DMA_TCD_ATTR_SMOD(srcModulo);
}

void EDMA_TcdEnableInterrupts(edma_tcd_t *tcd, uint32_t mask)
{
    assert(tcd != NULL);

    /* Enable Major interrupt */
    if (mask & kEDMA_MajorInterruptEnable)
    {
        tcd->CSR |= DMA_TCD_CSR_INTMAJOR_MASK;
    }

    /* Enable Half major interrupt */
    if (mask & kEDMA_HalfInterruptEnable)
    {
        tcd->CSR |= DMA_TCD_CSR_INTHALF_MASK;
    }
}

void EDMA_TcdDisableInterrupts(edma_tcd_t *tcd, uint32_t mask)
{
    assert(tcd != NULL);

    /* Disable Major interrupt */
    if (mask & kEDMA_MajorInterruptEnable)
    {
        tcd->CSR &= ~DMA_TCD_CSR_INTMAJOR_MASK;
    }

    /* Disable Half major interrupt */
    if (mask & kEDMA_HalfInterruptEnable)
    {
        tcd->CSR &= ~DMA_TCD_CSR_INTHALF_MASK;
    }
}

uint32_t EDMA_GetRemainingMajorLoopCount(DMA_Type *base, uint32_t channel)
{
    assert(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);

    uint32_t remainingCount = 0;

    if (DMA_CH_CSR_DONE_MASK & base->CH[channel].CH_CSR)
    {
        remainingCount = 0;
    }
    else
    {
        /* Calculate the unfinished bytes */
        if (base->CH[channel].TCD_CITER_ELINKNO & DMA_TCD_CITER_ELINKNO_ELINK_MASK)
        {
            remainingCount = (base->CH[channel].TCD_CITER_ELINKNO & DMA_TCD_CITER_ELINKYES_CITER_MASK) >>
                             DMA_TCD_CITER_ELINKYES_CITER_SHIFT;
        }
        else
        {
            remainingCount = (base->CH[channel].TCD_CITER_ELINKNO & DMA_TCD_CITER_ELINKNO_CITER_MASK) >>
                             DMA_TCD_CITER_ELINKNO_CITER_SHIFT;
        }
    }

    return remainingCount;
}

uint32_t EDMA_GetChannelStatusFlags(DMA_Type *base, uint32_t channel)
{
    assert(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);

    uint32_t retval = 0;

    /* Get DONE bit flag */
    retval |= ((base->CH[channel].CH_CSR & DMA_CH_CSR_DONE_MASK) >> DMA_CH_CSR_DONE_SHIFT);
    /* Get ERROR bit flag */
    retval |= (((base->CH[channel].CH_ES & DMA_CH_ES_ERR_MASK) >> DMA_CH_ES_ERR_SHIFT) << 1U);
    /* Get INT bit flag */
    retval |= (((base->CH[channel].CH_INT & DMA_CH_INT_INT_MASK) >> DMA_CH_INT_INT_SHIFT) << 2U);

    return retval;
}

void EDMA_ClearChannelStatusFlags(DMA_Type *base, uint32_t channel, uint32_t mask)
{
    assert(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);

    /* Clear DONE bit flag */
    if (mask & kEDMA_DoneFlag)
    {
        base->CH[channel].CH_CSR |= DMA_CH_CSR_DONE_MASK;
    }
    /* Clear ERROR bit flag */
    if (mask & kEDMA_ErrorFlag)
    {
        base->CH[channel].CH_ES |= DMA_CH_ES_ERR_MASK;
    }
    /* Clear INT bit flag */
    if (mask & kEDMA_InterruptFlag)
    {
        base->CH[channel].CH_INT |= DMA_CH_INT_INT_MASK;
    }
}

void EDMA_CreateHandle(edma_handle_t *handle, DMA_Type *base, uint32_t channel)
{
    assert(handle != NULL);
    assert(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);

    uint32_t edmaInstance;
    uint32_t channelIndex;
    edma_tcd_t *tcdRegs;

    handle->base = base;
    handle->channel = channel;
    /* Get the DMA instance number */
    edmaInstance = EDMA_GetInstance(base);
    channelIndex = (edmaInstance * FSL_FEATURE_EDMA_MODULE_CHANNEL) + channel;
    s_EDMAHandle[channelIndex] = handle;
#if defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT == 1U)
    /* Enable NVIC interrupt */
    EnableIRQ(s_edmaIRQNumber[channel]);
#elif defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 1U)
    /* Enable NVIC interrupt */
    EnableIRQ(s_edmaIRQNumber[edmaInstance][channel]);
#endif
    /*
       Reset TCD registers to zero. Unlike the EDMA_TcdReset(DREQ will be set),
       CSR will be 0. Because in order to suit EDMA busy check mechanism in
       EDMA_SubmitTransfer, CSR must be set 0.
    */
    tcdRegs = (edma_tcd_t *)((uint32_t)&handle->base->CH[handle->channel] + 0x00000020);
    tcdRegs->SADDR = 0;
    tcdRegs->SOFF = 0;
    tcdRegs->ATTR = 0;
    tcdRegs->NBYTES = 0;
    tcdRegs->SLAST = 0;
    tcdRegs->DADDR = 0;
    tcdRegs->DOFF = 0;
    tcdRegs->CITER = 0;
    tcdRegs->DLAST_SGA = 0;
    tcdRegs->CSR = 0;
    tcdRegs->BITER = 0;
}

void EDMA_InstallTCDMemory(edma_handle_t *handle, edma_tcd_t *tcdPool, uint32_t tcdSize)
{
    assert(handle != NULL);
    assert(((uint32_t)tcdPool & 0x1FU) == 0);

    /* Initialize tcd queue attibute. */
    handle->header = 0;
    handle->tail = 0;
    handle->tcdUsed = 0;
    handle->tcdSize = tcdSize;
    handle->flags = 0;
    handle->tcdPool = tcdPool;
}

void EDMA_SetCallback(edma_handle_t *handle, edma_callback callback, void *userData)
{
    assert(handle != NULL);

    handle->callback = callback;
    handle->userData = userData;
}

void EDMA_PrepareTransfer(edma_transfer_config_t *config,
                          void *srcAddr,
                          uint32_t srcWidth,
                          void *destAddr,
                          uint32_t destWidth,
                          uint32_t bytesEachRequest,
                          uint32_t transferBytes,
                          bool enableStoreDestAddr,
                          bool enableEndOfPacketProcessing,
                          edma_transfer_type_t type)
{
    assert(config != NULL);
    assert(srcAddr != NULL);
    assert(destAddr != NULL);
    assert((srcWidth == 1U) || (srcWidth == 2U) || (srcWidth == 4U) || (srcWidth == 16U) || (srcWidth == 32U));
    assert((destWidth == 1U) || (destWidth == 2U) || (destWidth == 4U) || (destWidth == 16U) || (destWidth == 32U));
    assert(transferBytes % bytesEachRequest == 0);

    config->destAddr = (uint32_t)destAddr + 0x15000000;
    config->srcAddr = (uint32_t)srcAddr + 0x15000000;
    config->minorLoopBytes = bytesEachRequest;
    config->majorLoopCounts = transferBytes / bytesEachRequest;
    switch (srcWidth)
    {
        case 1U:
            config->srcTransferSize = kEDMA_TransferSize1Bytes;
            break;
        case 2U:
            config->srcTransferSize = kEDMA_TransferSize2Bytes;
            break;
        case 4U:
            config->srcTransferSize = kEDMA_TransferSize4Bytes;
            break;
        case 16U:
            config->srcTransferSize = kEDMA_TransferSize16Bytes;
            break;
        case 32U:
            config->srcTransferSize = kEDMA_TransferSize32Bytes;
            break;
        case 64U:
            config->srcTransferSize = kEDMA_TransferSize64Bytes;
            break;
        default:
            break;
    }
    switch (destWidth)
    {
        case 1U:
            config->destTransferSize = kEDMA_TransferSize1Bytes;
            break;
        case 2U:
            config->destTransferSize = kEDMA_TransferSize2Bytes;
            break;
        case 4U:
            config->destTransferSize = kEDMA_TransferSize4Bytes;
            break;
        case 16U:
            config->destTransferSize = kEDMA_TransferSize16Bytes;
            break;
        case 32U:
            config->destTransferSize = kEDMA_TransferSize32Bytes;
            break;
        case 64U:
            config->destTransferSize = kEDMA_TransferSize64Bytes;
            break;
        default:
            break;
    }
    switch (type)
    {
        case kEDMA_MemoryToMemory:
            config->destOffset = destWidth;
            config->srcOffset = srcWidth;
            break;
        case kEDMA_MemoryToPeripheral:
            config->destOffset = 0U;
            config->srcOffset = srcWidth;
            break;
        case kEDMA_PeripheralToMemory:
            config->destOffset = destWidth;
            config->srcOffset = 0U;
            break;
        default:
            break;
    }
    config->enableStoreDestAddr = enableStoreDestAddr;
    config->enableEndOfPacketProcessing = enableEndOfPacketProcessing;
}

status_t EDMA_SubmitTransfer(edma_handle_t *handle, const edma_transfer_config_t *config)
{
    assert(handle != NULL);
    assert(config != NULL);

    edma_tcd_t *tcdRegs = (edma_tcd_t *)((uint32_t)&handle->base->CH[handle->channel] + 0x00000020);

    if (handle->tcdPool == NULL)
    {
        /*
            Check if EDMA is busy: if the given channel started transfer, CSR will be not zero. Because
            if it is the last transfer, DREQ will be set. If not, ESG will be set. So in order to suit
            this check mechanism, EDMA_CreatHandle will clear CSR register.
        */
        if ((tcdRegs->CSR != 0) && ((handle->base->CH[handle->channel].CH_CSR & DMA_CH_CSR_DONE_MASK) == 0))
        {
            return kStatus_EDMA_Busy;
        }
        else
        {
            EDMA_SetTransferConfig(handle->base, handle->channel, config, NULL);
            /* Enable auto disable request feature */
            handle->base->CH[handle->channel].TCD_CSR |= DMA_TCD_CSR_DREQ_MASK;
            /* Enable major interrupt */
            handle->base->CH[handle->channel].TCD_CSR |= DMA_TCD_CSR_INTMAJOR_MASK;

            return kStatus_Success;
        }
    }
    else /* Use the TCD queue. */
    {
        uint32_t primask;
        uint32_t csr;
        int8_t currentTcd;
        int8_t previousTcd;
        int8_t nextTcd;

        /* Check if tcd pool is full. */
        primask = DisableGlobalIRQ();
        if (handle->tcdUsed >= handle->tcdSize)
        {
            EnableGlobalIRQ(primask);

            return kStatus_EDMA_QueueFull;
        }
        currentTcd = handle->tail;
        handle->tcdUsed++;
        /* Calculate index of next TCD */
        nextTcd = currentTcd + 1U;
        if (nextTcd == handle->tcdSize)
        {
            nextTcd = 0U;
        }
        /* Advance queue tail index */
        handle->tail = nextTcd;
        EnableGlobalIRQ(primask);
        /* Calculate index of previous TCD */
        previousTcd = currentTcd ? currentTcd - 1U : handle->tcdSize - 1U;
        /* Configure current TCD block. */
        EDMA_TcdReset(&handle->tcdPool[currentTcd]);
        EDMA_TcdSetTransferConfig(&handle->tcdPool[currentTcd], config, NULL);
        /* Enable major interrupt */
        handle->tcdPool[currentTcd].CSR |= DMA_TCD_CSR_INTMAJOR_MASK;
        /* Link current TCD with next TCD for identification of current TCD */
        handle->tcdPool[currentTcd].DLAST_SGA = (uint32_t)&handle->tcdPool[nextTcd] + 0x15000000;
        /* Chain from previous descriptor unless tcd pool size is 1(this descriptor is its own predecessor). */
        if (currentTcd != previousTcd)
        {
            /* Enable scatter/gather feature in the previous TCD block. */
            csr = (handle->tcdPool[previousTcd].CSR | DMA_TCD_CSR_ESG_MASK) & ~DMA_TCD_CSR_DREQ_MASK;
            handle->tcdPool[previousTcd].CSR = csr;
            /*
                Check if the TCD blcok in the registers is the previous one (points to current TCD block). It
                is used to check if the previous TCD linked has been loaded in TCD register. If so, it need to
                link the TCD register in case link the current TCD with the dead chain when TCD loading occurs
                before link the previous TCD block.
            */
            if (tcdRegs->DLAST_SGA == (uint32_t)&handle->tcdPool[currentTcd] + 0x15000000)
            {
                /* Enable scatter/gather also in the TCD registers. */
                csr = (tcdRegs->CSR | DMA_TCD_CSR_ESG_MASK) & ~DMA_TCD_CSR_DREQ_MASK;
                /* Must write the CSR register one-time, because the transfer maybe finished anytime. */
                tcdRegs->CSR = csr;
                /*
                    It is very important to check the ESG bit!
                    Because this hardware design: if DONE bit is set, the ESG bit can not be set. So it can
                    be used to check if the dynamic TCD link operation is successful. If ESG bit is not set
                    and the DLAST_SGA is not the next TCD address(it means the dynamic TCD link succeed and
                    the current TCD block has been loaded into TCD registers), it means transfer finished
                    and TCD link operation fail, so must install TCD content into TCD registers and enable
                    transfer again. And if ESG is set, it means transfer has notfinished, so TCD dynamic
                    link succeed.
                */
                if (tcdRegs->CSR & DMA_TCD_CSR_ESG_MASK)
                {
                    return kStatus_Success;
                }
                /*
                    Check whether the current TCD block is already loaded in the TCD registers. It is another
                    condition when ESG bit is not set: it means the dynamic TCD link succeed and the current
                    TCD block has been loaded into TCD registers.
                */
                if (tcdRegs->DLAST_SGA == (uint32_t)&handle->tcdPool[nextTcd])
                {
                    return kStatus_Success;
                }
                /*
                    If go to this, means the previous transfer finished, and the DONE bit is set.
                    So shall configure TCD registers.
                */
            }
            else if (tcdRegs->DLAST_SGA != 0)
            {
                /* The current TCD block has been linked successfully. */
                return kStatus_Success;
            }
            else 
            {
                /*
                    DLAST_SGA is 0 and it means the first submit transfer, so shall configure
                    TCD registers.
                */
            }
        }
        /* There is no live chain, TCD block need to be installed in TCD registers. */
        EDMA_InstallTCD(handle->base, handle->channel, &handle->tcdPool[currentTcd]);
        /* Enable channel request again. */
        if (handle->flags & EDMA_TRANSFER_ENABLED_MASK)
        {
            handle->base->CH[handle->channel].CH_CSR =
                (handle->base->CH[handle->channel].CH_CSR & (~DMA_CH_CSR_DONE_MASK)) | DMA_CH_CSR_ERQ_MASK;
        }

        return kStatus_Success;
    }
}

void EDMA_StartTransfer(edma_handle_t *handle)
{
    assert(handle != NULL);

    if (handle->tcdPool == NULL)
    {
        handle->base->CH[handle->channel].CH_CSR = (handle->base->CH[handle->channel].CH_CSR & (~DMA_CH_CSR_DONE_MASK)) | DMA_CH_CSR_ERQ_MASK;
    }
    else /* Use the TCD queue. */
    {
        uint32_t primask;
        edma_tcd_t *tcdRegs = (edma_tcd_t *)((uint32_t)&handle->base->CH[handle->channel] + 0x00000020);

        handle->flags |= EDMA_TRANSFER_ENABLED_MASK;

        /* Check if there was at least one descriptor submitted since reset (TCD in registers is valid) */
        if (tcdRegs->DLAST_SGA != 0U)
        {
            primask = DisableGlobalIRQ();
            /* Check if channel request is actually disable. */
            if ((handle->base->CH[handle->channel].CH_CSR & DMA_CH_CSR_ERQ_MASK & (1U << handle->channel)) == 0U)
            {
                /* Check if transfer is paused. */
                if ((!(handle->base->CH[handle->channel].CH_CSR & DMA_CH_CSR_DONE_MASK)) ||
                    (tcdRegs->CSR & DMA_TCD_CSR_ESG_MASK))
                {
                    /*
                        Re-enable channel request must be as soon as possible, so must put it into
                        critical section to avoid task switching or interrupt service routine.
                    */
                    handle->base->CH[handle->channel].CH_CSR =
                        (handle->base->CH[handle->channel].CH_CSR & (~DMA_CH_CSR_DONE_MASK)) | DMA_CH_CSR_ERQ_MASK;
                }
            }
            EnableGlobalIRQ(primask);
        }
    }
}

void EDMA_StopTransfer(edma_handle_t *handle)
{
    assert(handle != NULL);

    handle->flags &= (~EDMA_TRANSFER_ENABLED_MASK);
    handle->base->CH[handle->channel].CH_CSR = (handle->base->CH[handle->channel].CH_CSR & (~DMA_CH_CSR_DONE_MASK)) & ~DMA_CH_CSR_ERQ_MASK;
}

void EDMA_AbortTransfer(edma_handle_t *handle)
{
    handle->base->CH[handle->channel].CH_CSR = (handle->base->CH[handle->channel].CH_CSR & (~DMA_CH_CSR_DONE_MASK)) & ~DMA_CH_CSR_ERQ_MASK;
    /*
        Clear CSR to release channel. Because if the given channel started transfer,
        CSR will be not zero. Because if it is the last transfer, DREQ will be set.
        If not, ESG will be set.
    */
    handle->base->CH[handle->channel].TCD_CSR = 0;
    /* Cancel all next TCD transfer. */
    handle->base->CH[handle->channel].TCD_DLAST_SGA = 0;
}

void EDMA_HandleIRQ(edma_handle_t *handle)
{
    assert(handle != NULL);

    /* Clear EDMA interrupt flag */
    handle->base->CH[handle->channel].CH_INT |= DMA_CH_INT_INT_MASK;
    if ((handle->tcdPool == NULL) && (handle->callback != NULL))
    {
        (handle->callback)(handle, handle->userData, true, 0);
    }
    else /* Use the TCD queue. */
    {
        uint32_t sga = handle->base->CH[handle->channel].TCD_DLAST_SGA;
        uint32_t sga_index;
        int32_t tcds_done;
        uint8_t new_header;
        bool transfer_done;

        /* Check if transfer is already finished. */
        transfer_done = ((handle->base->CH[handle->channel].CH_CSR & DMA_CH_CSR_DONE_MASK) != 0);
        /* Get the offset of the current transfer TCD blcoks. */
        sga -= (uint32_t)handle->tcdPool;
        /* Get the index of the current transfer TCD blcoks. */
        sga_index = sga / sizeof(edma_tcd_t);
        /* Adjust header positions. */
        if (transfer_done)
        {
            /* New header shall point to the next TCD (current one is already finished) */
            new_header = sga_index;
        }
        else
        {
            /* New header shall point to this descriptor (not finished yet) */
            new_header = sga_index ? sga_index - 1U : handle->tcdSize - 1U;
        }
        /* Calculate the number of finished TCDs */
        if (new_header == handle->header)
        {
            if (handle->tcdUsed == handle->tcdSize)
            {
                tcds_done = handle->tcdUsed;
            }
            else
            {
                /* Internal error occurs. */
                tcds_done = 0;
            }
        }
        else
        {
            tcds_done = new_header - handle->header;
            if (tcds_done < 0)
            {
                tcds_done += handle->tcdSize;
            }
        }
        /* Advance header to the point beyond the last finished TCD block. */
        handle->header = new_header;
        /* Release TCD blocks. */
        handle->tcdUsed -= tcds_done;
        /* Invoke callback function. */
        if (handle->callback)
        {
            (handle->callback)(handle, handle->userData, transfer_done, tcds_done);
        }
    }
}

/* 32 channels (Shared): i.MX8QM */
#if defined(FSL_FEATURE_EDMA_MODULE_CHANNEL) && FSL_FEATURE_EDMA_MODULE_CHANNEL == 32U

void DMA_CH0_DriverIRQHandler(void)
{
    if ((EDMA_GetChannelStatusFlags(DMA0, 0U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[0]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 4U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[4]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 8U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[8]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 12U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[12]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 16U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[16]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 20U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[20]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 24U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[24]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 28U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[28]);
    }
}

void DMA_CH1_DriverIRQHandler(void)
{
    if ((EDMA_GetChannelStatusFlags(DMA0, 1U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[1]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 5U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[5]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 9U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[9]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 13U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[13]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 17U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[17]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 21U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[21]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 25U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[25]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 29U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[29]);
    }
}

void DMA_CH2_DriverIRQHandler(void)
{
    if ((EDMA_GetChannelStatusFlags(DMA0, 2U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[2]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 6U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[6]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 10U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[10]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 14U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[14]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 18U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[18]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 22U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[22]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 26U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[26]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 30U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[30]);
    }
}

void DMA_CH3_DriverIRQHandler(void)
{
    if ((EDMA_GetChannelStatusFlags(DMA0, 3U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[3]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 7U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[7]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 11U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[11]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 15U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[15]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 19U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[19]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 23U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[23]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 27U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[27]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 31U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[31]);
    }
}

#endif /* 32 channels (Shared) */
