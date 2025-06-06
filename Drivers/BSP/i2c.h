#ifndef _I2C_H
#define _I2C_H

#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#include "Delay.h"
void I2C1_Init(void);
void EEprom_SendByte(uint8_t address, uint8_t data);
void EEprom_ReadByte(uint8_t address, uint8_t *data);
void EEPROM_WritePage(uint8_t addr, uint8_t* data, uint8_t len);
void EEPROM_ReadBytes(uint8_t addr, uint8_t* buffer, uint16_t len);
#endif



