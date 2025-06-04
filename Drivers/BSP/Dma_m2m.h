#ifndef _DMA_H
#define _DMA_H
#include "stm32f10x.h"
#include "stm32f10x_dma.h"

void DMA_M2M_Init(uint32_t *source, uint32_t *destination, uint32_t BufferSize);

#endif


