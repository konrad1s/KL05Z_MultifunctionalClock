#include "MKL05Z4.h"
#include "dma.h"

void DMA_init()
{
    // enable clocks
    SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
    SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;

    //reset DMA
    DMAMUX0_CHCFG0 = 0x00;

    // clear pending errors and/or the done bit
    if (((DMA_DSR_BCR0 & DMA_DSR_BCR_DONE_MASK) == DMA_DSR_BCR_DONE_MASK) | ((DMA_DSR_BCR0 & DMA_DSR_BCR_BES_MASK) == DMA_DSR_BCR_BES_MASK) | ((DMA_DSR_BCR0 & DMA_DSR_BCR_BED_MASK) == DMA_DSR_BCR_BED_MASK) | ((DMA_DSR_BCR0 & DMA_DSR_BCR_CE_MASK) == DMA_DSR_BCR_CE_MASK))
        DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;

    DMA_SAR0 = (uint32_t)&ADC0_RA;     // source address
    DMA_DSR_BCR0 = DMA_DSR_BCR_BCR(2); // 2 bytes (16 bits) per transfer

    // DMA settings:
    // enable interrupt
    // enable peripheral request
    // setsize to 16 bits
    // cycle steal mode
    // destination address modulo 16
    // destination size 16 bits

    DMA_DCR0 |= (DMA_DCR_EINT_MASK |
                 DMA_DCR_ERQ_MASK |
                 DMA_DCR_CS_MASK |
                 DMA_DCR_SSIZE(2) |
                 DMA_DCR_DINC_MASK |
                 DMA_DCR_DMOD(1) |
                 DMA_DCR_DSIZE(2));

    // destination address
    DMA_DAR0 = (uint32_t)&DMAvalue;

    // DMA channel ADC0
    DMAMUX0_CHCFG0 = DMA_REQUEST_SOURCE_ADC0;
    DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK;

    // enable interrupt
    NVIC_EnableIRQ(DMA0_IRQn);
}