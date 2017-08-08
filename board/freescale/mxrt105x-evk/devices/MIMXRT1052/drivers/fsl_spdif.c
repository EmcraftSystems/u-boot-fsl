/*
 * Copyright (c) 2017, NXP Semiconductor, Inc.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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

#include "fsl_spdif.h"

/*******************************************************************************
 * Definitations
 ******************************************************************************/
enum _spdif_transfer_state
{
    kSPDIF_Busy = 0x0U, /*!< SPDIF is busy */
    kSPDIF_Idle,        /*!< Transfer is done. */
    kSPDIF_Error        /*!< Transfer error occured. */
};

/*! @brief Typedef for spdif tx interrupt handler. */
typedef void (*spdif_isr_t)(SPDIF_Type *base, spdif_handle_t *spdifHandle);
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Get the instance number for SPDIF.
 *
 * @param base SPDIF base pointer.
 */
uint32_t SPDIF_GetInstance(SPDIF_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Base pointer array */
static SPDIF_Type *const s_spdifBases[] = SPDIF_BASE_PTRS;
/*!@brief SPDIF handle pointer */
spdif_handle_t *s_spdifHandle[ARRAY_SIZE(s_spdifBases)][2];
/* IRQ number array */
static const IRQn_Type s_spdifIRQ[] = SPDIF_IRQS;
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/* Clock name array */
static const clock_ip_name_t s_spdifClock[] = SPDIF_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
/*! @brief Pointer to tx IRQ handler for each instance. */
static spdif_isr_t s_spdifTxIsr;
/*! @brief Pointer to rx IRQ handler for each instance. */
static spdif_isr_t s_spdifRxIsr;
/*! @brief Used for spdif gain */
static uint8_t s_spdif_gain[8] = {24U, 16U, 12U, 8U, 6U, 4U, 3U, 1U};
static uint8_t s_spdif_tx_watermark[4] = {16, 12, 8, 4};
static uint8_t s_spdif_rx_watermark[4] = {1, 4, 8, 16};

/*******************************************************************************
 * Code
 ******************************************************************************/
uint32_t SPDIF_GetInstance(SPDIF_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_spdifBases); instance++)
    {
        if (s_spdifBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < ARRAY_SIZE(s_spdifBases));

    return instance;
}

void SPDIF_Init(SPDIF_Type *base, const spdif_config_t *config)
{
    uint32_t val = 0;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Enable the SPDIF clock */
    CLOCK_EnableClock(s_spdifClock[SPDIF_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

    /* Reset the internal logic */
    base->SCR |= SPDIF_SCR_SOFT_RESET_MASK;

    /* Waiting for reset finish */
    while (base->SCR & SPDIF_SCR_SOFT_RESET_MASK)
    {
    }

    /* Setting the SPDIF settings */
    base->SCR = SPDIF_SCR_RXFIFO_CTRL_MASK | SPDIF_SCR_RXFIFOFULL_SEL(config->rxFullSelect) |
                SPDIF_SCR_RXAUTOSYNC(config->isRxAutoSync) | SPDIF_SCR_TXAUTOSYNC(config->isRxAutoSync) |
                SPDIF_SCR_TXFIFOEMPTY_SEL(config->txFullSelect) | SPDIF_SCR_TXFIFO_CTRL(1U) |
                SPDIF_SCR_VALCTRL(config->validityConfig) | SPDIF_SCR_TXSEL(config->txSource) |
                SPDIF_SCR_USRC_SEL(config->uChannelSrc);

    /* Set DPLL clock source */
    base->SRPC = SPDIF_SRPC_CLKSRC_SEL(config->DPLLClkSource);

    /* Set SPDIF tx clock source */
    val = base->STC & ~SPDIF_STC_TXCLK_SOURCE_MASK;
    val |= SPDIF_STC_TXCLK_SOURCE(config->txClkSource);
    base->STC = val;
}

void SPDIF_Deinit(SPDIF_Type *base)
{
    SPDIF_TxEnable(base, false);
    SPDIF_RxEnable(base, false);
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    CLOCK_DisableClock(s_spdifClock[SPDIF_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

void SPDIF_GetDefaultConfig(spdif_config_t *config)
{
    config->isTxAutoSync = false;
    config->isRxAutoSync = false;
    config->DPLLClkSource = 3;
    config->txClkSource = 3;
    config->rxFullSelect = kSPDIF_RxFull8Samples;
    config->txFullSelect = kSPDIF_TxEmpty8Samples;
    config->uChannelSrc = kSPDIF_NoUChannel;
    config->txSource = kSPDIF_txNormal;
    config->validityConfig = kSPDIF_validityFlagAlwaysSet;
    config->gain = kSPDIF_GAIN_8;
}

void SPDIF_TxEnable(SPDIF_Type *base, bool enable)
{
    uint32_t val = 0;

    if (enable)
    {
        /* Open Rx FIFO */
        val = base->SCR & (~SPDIF_SCR_TXFIFO_CTRL_MASK);
        val |= SPDIF_SCR_TXFIFO_CTRL(1U);
        base->SCR = val;
        /* Enable transfer clock */
        base->STC |= SPDIF_STC_TX_ALL_CLK_EN_MASK;
    }
    else
    {
        base->SCR &= ~(SPDIF_SCR_TXFIFO_CTRL_MASK | SPDIF_SCR_TXSEL_MASK);
        /* Disable transfer clock */
        base->STC &= ~SPDIF_STC_TX_ALL_CLK_EN_MASK;
    }
}

void SPDIF_TxSetSampleRate(SPDIF_Type *base, uint32_t sampleRate_Hz, uint32_t sourceClockFreq_Hz)
{
    uint32_t div = sourceClockFreq_Hz / sampleRate_Hz;
    uint32_t mod = sourceClockFreq_Hz % sampleRate_Hz;
    uint32_t val = 0;

    /* Compute the nearest divider */
    if (mod > (sampleRate_Hz / 2))
    {
        div += 1U;
    }

    /* If divider larger than 256, use SYSCLK_DF */
    if (div > 256)
    {
        val = base->STC & (~(SPDIF_STC_TXCLK_DF_MASK | SPDIF_STC_SYSCLK_DF_MASK));
        val |= SPDIF_STC_SYSCLK_DF(div / 128U - 1U) | SPDIF_STC_TXCLK_DF(127U);
        base->STC = val;
    }
    else
    {
        val = base->STC & (~(SPDIF_STC_TXCLK_DF_MASK | SPDIF_STC_SYSCLK_DF_MASK));
        val |= SPDIF_STC_SYSCLK_DF(1U) | SPDIF_STC_TXCLK_DF(div);
        base->STC = val;
    }
}

uint32_t SPDIF_GetRxSampleRate(SPDIF_Type *base, uint32_t clockSourceFreq_Hz)
{
    uint32_t gain = s_spdif_gain[((base->SRPC & SPDIF_SRPC_GAINSEL_MASK) >> SPDIF_SRPC_GAINSEL_SHIFT)];
    uint32_t measure = 0, sampleRate = 0;

    /* Wait the DPLL locked */
    while ((base->SRPC & SPDIF_SRPC_LOCK_MASK) == 0U)
    {
    }

    /* Get the measure value */
    measure = base->SRFM;
    sampleRate = clockSourceFreq_Hz / (1024 * 128 * gain);
    sampleRate *= measure;

    return sampleRate;
}

void SPDIF_WriteBlocking(SPDIF_Type *base, uint8_t *buffer, uint32_t size)
{
    assert(buffer);
    assert(size / 6U == 0U);

    uint32_t i = 0, j = 0, data = 0;

    while (i < size)
    {
        /* Wait until it can write data */
        while ((SPDIF_GetStatusFlag(base) & kSPDIF_TxFIFOEmpty) == 0U)
        {
        }

        /* Write left channel data */
        for (j = 0; j < 3U; j++)
        {
            data |= ((*buffer) << (j * 8U));
            buffer++;
        }
        SPDIF_WriteLeftData(base, data);

        /* Write right channel data */
        data = 0;
        for (j = 0; j < 3U; j++)
        {
            data |= ((*buffer) << (j * 8U));
            buffer++;
        }
        SPDIF_WriteRightData(base, data);

        i += 6U;
    }
}

void SPDIF_ReadBlocking(SPDIF_Type *base, uint8_t *buffer, uint32_t size)
{
    assert(buffer);
    assert(size / 6U == 0U);

    uint32_t i = 0, j = 0, data = 0;

    while (i < size)
    {
        /* Wait until it can write data */
        while ((SPDIF_GetStatusFlag(base) & kSPDIF_RxFIFOFull) == 0U)
        {
        }

        /* Write left channel data */
        data = SPDIF_ReadLeftData(base);
        for (j = 0; j < 3U; j++)
        {
            *buffer = ((data >> (j * 8U)) & 0xFFU);
            buffer++;
        }

        /* Write right channel data */
        data = SPDIF_ReadRightData(base);
        for (j = 0; j < 3U; j++)
        {
            *buffer = ((data >> (j * 8U)) & 0xFFU);
            buffer++;
        }

        i += 6U;
    }
}

void SPDIF_TransferTxCreateHandle(SPDIF_Type *base,
                                  spdif_handle_t *handle,
                                  spdif_transfer_callback_t callback,
                                  void *userData)
{
    assert(handle);

    /* Zero the handle */
    memset(handle, 0, sizeof(*handle));

    s_spdifHandle[SPDIF_GetInstance(base)][0] = handle;

    handle->callback = callback;
    handle->userData = userData;
    handle->watermark =
        s_spdif_tx_watermark[(base->SCR & SPDIF_SCR_TXFIFOEMPTY_SEL_MASK) >> SPDIF_SCR_TXFIFOEMPTY_SEL_SHIFT];

    /* Set the isr pointer */
    s_spdifTxIsr = SPDIF_TransferTxHandleIRQ;

    /* Enable Tx irq */
    EnableIRQ(s_spdifIRQ[SPDIF_GetInstance(base)]);
}

void SPDIF_TransferRxCreateHandle(SPDIF_Type *base,
                                  spdif_handle_t *handle,
                                  spdif_transfer_callback_t callback,
                                  void *userData)
{
    assert(handle);

    /* Zero the handle */
    memset(handle, 0, sizeof(*handle));

    s_spdifHandle[SPDIF_GetInstance(base)][1] = handle;

    handle->callback = callback;
    handle->userData = userData;
    handle->watermark =
        s_spdif_rx_watermark[(base->SCR & SPDIF_SCR_RXFIFOFULL_SEL_MASK) >> SPDIF_SCR_RXFIFOFULL_SEL_SHIFT];

    /* Set the isr pointer */
    s_spdifRxIsr = SPDIF_TransferRxHandleIRQ;

    /* Enable Rx irq */
    EnableIRQ(s_spdifIRQ[SPDIF_GetInstance(base)]);
}

status_t SPDIF_TransferSendNonBlocking(SPDIF_Type *base, spdif_handle_t *handle, spdif_transfer_t *xfer)
{
    assert(handle);

    /* Check if the queue is full */
    if (handle->spdifQueue[handle->queueUser].data)
    {
        return kStatus_SPDIF_QueueFull;
    }

    /* Add into queue */
    handle->transferSize[handle->queueUser] = xfer->dataSize;
    handle->spdifQueue[handle->queueUser].data = xfer->data;
    handle->spdifQueue[handle->queueUser].dataSize = xfer->dataSize;
    handle->queueUser = (handle->queueUser + 1) % SPDIF_XFER_QUEUE_SIZE;

    /* Set the state to busy */
    handle->state = kSPDIF_Busy;

    /* Enable interrupt */
    SPDIF_EnableInterrupts(base, kSPDIF_TxFIFOEmpty);

    /* Enable Tx transfer */
    SPDIF_TxEnable(base, true);

    return kStatus_Success;
}

status_t SPDIF_TransferReceiveNonBlocking(SPDIF_Type *base, spdif_handle_t *handle, spdif_transfer_t *xfer)
{
    assert(handle);

    /* Check if the queue is full */
    if (handle->spdifQueue[handle->queueUser].data)
    {
        return kStatus_SPDIF_QueueFull;
    }

    /* Add into queue */
    handle->transferSize[handle->queueUser] = xfer->dataSize;
    handle->spdifQueue[handle->queueUser].data = xfer->data;
    handle->spdifQueue[handle->queueUser].dataSize = xfer->dataSize;
    handle->spdifQueue[handle->queueUser].udata = xfer->udata;
    handle->spdifQueue[handle->queueUser].qdata = xfer->qdata;
    handle->queueUser = (handle->queueUser + 1) % SPDIF_XFER_QUEUE_SIZE;

    /* Set state to busy */
    handle->state = kSPDIF_Busy;

    /* Enable interrupt */
    SPDIF_EnableInterrupts(base,
                           kSPDIF_UChannelReceiveRegisterFull | kSPDIF_QChannelReceiveRegisterFull | kSPDIF_RxFIFOFull);

    /* Enable Rx transfer */
    SPDIF_RxEnable(base, true);

    return kStatus_Success;
}

status_t SPDIF_TransferGetSendCount(SPDIF_Type *base, spdif_handle_t *handle, size_t *count)
{
    assert(handle);

    status_t status = kStatus_Success;

    if (handle->state != kSPDIF_Busy)
    {
        status = kStatus_NoTransferInProgress;
    }
    else
    {
        *count = (handle->transferSize[handle->queueDriver] - handle->spdifQueue[handle->queueDriver].dataSize);
    }

    return status;
}

status_t SPDIF_TransferGetReceiveCount(SPDIF_Type *base, spdif_handle_t *handle, size_t *count)
{
    assert(handle);

    status_t status = kStatus_Success;

    if (handle->state != kSPDIF_Busy)
    {
        status = kStatus_NoTransferInProgress;
    }
    else
    {
        *count = (handle->transferSize[handle->queueDriver] - handle->spdifQueue[handle->queueDriver].dataSize);
    }

    return status;
}

void SPDIF_TransferAbortSend(SPDIF_Type *base, spdif_handle_t *handle)
{
    assert(handle);

    /* Stop Tx transfer and disable interrupt */
    SPDIF_TxEnable(base, false);

    /* Use FIFO request interrupt and fifo error */
    SPDIF_DisableInterrupts(base, kSPDIF_TxFIFOEmpty);

    handle->state = kSPDIF_Idle;

    /* Clear the queue */
    memset(handle->spdifQueue, 0, sizeof(spdif_transfer_t) * SPDIF_XFER_QUEUE_SIZE);
    handle->queueDriver = 0;
    handle->queueUser = 0;
}

void SPDIF_TransferAbortReceive(SPDIF_Type *base, spdif_handle_t *handle)
{
    assert(handle);

    /* Stop Tx transfer and disable interrupt */
    SPDIF_RxEnable(base, false);

    /* Disable interrupt */
    SPDIF_DisableInterrupts(
        base, kSPDIF_UChannelReceiveRegisterFull | kSPDIF_QChannelReceiveRegisterFull | kSPDIF_RxFIFOFull);

    handle->state = kSPDIF_Idle;

    /* Clear the queue */
    memset(handle->spdifQueue, 0, sizeof(spdif_transfer_t) * SPDIF_XFER_QUEUE_SIZE);
    handle->queueDriver = 0;
    handle->queueUser = 0;
}

void SPDIF_TransferTxHandleIRQ(SPDIF_Type *base, spdif_handle_t *handle)
{
    assert(handle);

    uint8_t *buffer = handle->spdifQueue[handle->queueDriver].data;
    uint8_t dataSize = 0;
    uint32_t val = ((base->SIE) & (base->SIS));
    uint32_t i = 0, j = 0, data = 0;

    /* Handle Error */
    SPDIF_ClearStatusFlags(base, val);

    /* Do Transfer */
    dataSize = handle->watermark;
    while (i < dataSize)
    {
        /* Write left channel data */
        for (j = 0; j < 3U; j++)
        {
            data |= ((*buffer) << (j * 8U));
            buffer++;
        }
        SPDIF_WriteLeftData(base, data);

        /* Write right channel data */
        data = 0;
        for (j = 0; j < 3U; j++)
        {
            data |= ((*buffer) << (j * 8U));
            buffer++;
        }
        SPDIF_WriteRightData(base, data);

        i++;
    }
    handle->spdifQueue[handle->queueDriver].dataSize -= dataSize * 6U;
    handle->spdifQueue[handle->queueDriver].data += dataSize * 6U;

    /* If finished a blcok, call the callback function */
    if (handle->spdifQueue[handle->queueDriver].dataSize == 0U)
    {
        memset(&handle->spdifQueue[handle->queueDriver], 0, sizeof(spdif_transfer_t));
        handle->queueDriver = (handle->queueDriver + 1) % SPDIF_XFER_QUEUE_SIZE;
        if (handle->callback)
        {
            (handle->callback)(base, handle, kStatus_SPDIF_TxIdle, handle->userData);
        }
    }

    /* If all data finished, just stop the transfer */
    if (handle->spdifQueue[handle->queueDriver].data == NULL)
    {
        SPDIF_TransferAbortSend(base, handle);
    }
}

void SPDIF_TransferRxHandleIRQ(SPDIF_Type *base, spdif_handle_t *handle)
{
    assert(handle);

    uint8_t *buffer = NULL;
    uint8_t dataSize = 0;
    uint32_t val = ((base->SIE) & (base->SIS));
    uint32_t i = 0, j = 0, data = 0;

    /* Handle Error */
    SPDIF_ClearStatusFlags(base, val);

    /* Handle Q channel full flag */
    if ((SPDIF_GetStatusFlag(base) & kSPDIF_QChannelReceiveRegisterFull) &&
        (base->SIE & kSPDIF_QChannelReceiveRegisterFull))
    {
        buffer = handle->spdifQueue[handle->queueDriver].qdata;
        data = SPDIF_ReadQChannel(base);
        buffer[0] = data & 0xFFU;
        buffer[1] = (data >> 8U) & 0xFFU;
        buffer[2] = (data >> 16U) & 0xFFU;
        handle->spdifQueue[handle->queueDriver].qdata += 3U;
    }

    /* Handle U channel full flag */
    if ((SPDIF_GetStatusFlag(base) & kSPDIF_UChannelReceiveRegisterFull) &&
        (base->SIE & kSPDIF_UChannelReceiveRegisterFull))
    {
        buffer = handle->spdifQueue[handle->queueDriver].udata;
        data = SPDIF_ReadUChannel(base);
        buffer[0] = data & 0xFFU;
        buffer[1] = (data >> 8U) & 0xFFU;
        buffer[2] = (data >> 16U) & 0xFFU;
        handle->spdifQueue[handle->queueDriver].udata += 3U;
    }

    /* Handle audio data transfer */
    if ((SPDIF_GetStatusFlag(base) & kSPDIF_RxFIFOFull) && (base->SIE & kSPDIF_RxFIFOFull))
    {
        dataSize = handle->watermark;
        buffer = handle->spdifQueue[handle->queueDriver].data;
        while (i < dataSize)
        {
            /* Read left channel data */
            data = SPDIF_ReadLeftData(base);
            for (j = 0; j < 3U; j++)
            {
                *buffer = ((data >> (j * 8U)) & 0xFFU);
                buffer++;
            }

            /* Read right channel data */
            data = SPDIF_ReadRightData(base);
            for (j = 0; j < 3U; j++)
            {
                *buffer = ((data >> (j * 8U)) & 0xFFU);
                buffer++;
            }

            i++;
        }
    }

    /* If finished a blcok, call the callback function */
    if (handle->spdifQueue[handle->queueDriver].dataSize == 0U)
    {
        memset(&handle->spdifQueue[handle->queueDriver], 0, sizeof(spdif_transfer_t));
        handle->queueDriver = (handle->queueDriver + 1) % SPDIF_XFER_QUEUE_SIZE;
        if (handle->callback)
        {
            (handle->callback)(base, handle, kStatus_SPDIF_RxIdle, handle->userData);
        }
    }

    /* If all data finished, just stop the transfer */
    if (handle->spdifQueue[handle->queueDriver].data == NULL)
    {
        SPDIF_TransferAbortReceive(base, handle);
    }
}

#if defined(SPDIF)
void SPDIF_DriverIRQHandler(void)
{
    if ((s_spdifHandle[0][1]) && ((SPDIF_GetStatusFlag(SPDIF) & kSPDIF_RxFIFOFull) && (SPDIF->SIE & kSPDIF_RxFIFOFull)))
    {
        s_spdifRxIsr(SPDIF, s_spdifHandle[0][1]);
    }
    if ((s_spdifHandle[0][0]) &&
        ((SPDIF_GetStatusFlag(SPDIF) & kSPDIF_TxFIFOEmpty) && (SPDIF->SIE & kSPDIF_TxFIFOEmpty)))
    {
        s_spdifTxIsr(SPDIF, s_spdifHandle[0][0]);
    }
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}
#endif
