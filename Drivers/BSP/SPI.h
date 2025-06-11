#ifndef SPI_H
#define SPI_H

#include "stm32f10x.h"
#include "Delay.h"
#include "usart.h"
// Your SPI function declarations and definitions go here
void SPI2_Init(void);
int SPI2_FLASH_Send_byte(uint8_t data);
int SPI2_FLASH_Receive_byte(uint8_t dummy);
void SPI_FLASH_SectorErase(uint32_t sectorAddress);
void SPI2_FLASH_Write_Enable(void);
void SPI_FLASH_WaitBusy(void);
void SPI2_FLASH_WriteByte(uint32_t address, uint8_t* data, uint32_t length);
uint32_t SPI2_FLASH_ReadID(void);
int SPI2_FLASH_ReadData(uint32_t address, uint8_t *buffer, uint32_t length);
void SPI2_FLASH_WriteBytes(uint32_t address, const uint8_t *data, uint32_t length);

#endif // SPI_H


