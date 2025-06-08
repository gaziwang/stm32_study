#ifndef SPI_H
#define SPI_H

#include "stm32f10x.h"
#include "Delay.h"

// Your SPI function declarations and definitions go here
void SPI2_Init(void);
uint8_t SPI2_FLASH_Send_byte(uint8_t data);
uint8_t SPI2_FLASH_Recive_byte(void);
#endif // SPI_H


