#include "Dma_m2m.h"

void DMA_M2M_Init(uint32_t *source, uint32_t *destination, uint32_t BufferSize)
{
    // Enable DMA1 clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_DeInit(DMA1_Channel1); // Reset DMA1 Channel 1 to default state
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)source; // Peripheral base address
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)destination; // Memory base address
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // Direction: Peripheral to Memory
    DMA_InitStructure.DMA_BufferSize = BufferSize; // Buffer size in data units
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // Peripheral address not incremented








}


