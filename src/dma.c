
#include "../inc/dma.h"

void DMA_init()
{
  // enable clocks
  SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
  SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;

  //reset DMA
  DMAMUX0->CHCFG[0] = 0x00;

  // clear pending errors and/or the done bit
  if (((DMA0->DMA[0].DSR_BCR & DMA_DSR_BCR_DONE_MASK) == DMA_DSR_BCR_DONE_MASK) | ((DMA0->DMA[0].DSR_BCR & DMA_DSR_BCR_BES_MASK) == DMA_DSR_BCR_BES_MASK) | ((DMA0->DMA[0].DSR_BCR & DMA_DSR_BCR_BED_MASK) == DMA_DSR_BCR_BED_MASK) | ((DMA0->DMA[0].DSR_BCR & DMA_DSR_BCR_CE_MASK) == DMA_DSR_BCR_CE_MASK))
    DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE_MASK;

  DMA0->DMA[0].SAR = (uint32_t)&ADC0->R[0];  // source address
  DMA0->DMA[0].DSR_BCR = DMA_DSR_BCR_BCR(2); // 16 bits per transfer

  // DMA settings:
  // enable interrupt
  // enable peripheral request
  // setsize to 16 bits
  // cycle steal mode
  // destination address modulo 16
  // destination size 16 bits
  DMA0->DMA[0].DCR |= (DMA_DCR_EINT_MASK |
                       DMA_DCR_ERQ_MASK |
                       DMA_DCR_CS_MASK |
                       DMA_DCR_SSIZE(2) |
                       DMA_DCR_DINC_MASK |
                       DMA_DCR_DMOD(1) |
                       DMA_DCR_DSIZE(2));

  // destination address
  DMA0->DMA[0].DAR = (uint32_t)&DMAvalue[0];

  // DMA channel ADC0 = 40
  DMAMUX0->CHCFG[0] |= 0x28;
  DMAMUX0->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK;

  // enable interrupt
  NVIC_EnableIRQ(DMA0_IRQn);
}