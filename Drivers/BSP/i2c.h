#ifndef _I2C_H
#define _I2C_H

#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#include "Delay.h"

void HardI2C_Init(void);
void HardI2C_WriteByte(uint8_t memAddr, uint8_t data);
uint8_t HardI2C_ReadByte(uint8_t memAddr);
void HardI2C_WritePage(uint8_t memAddr, const uint8_t *data, uint8_t len);
void HardI2C_ReadBytes(uint8_t memAddr, uint8_t *data, uint16_t len);
void SoftI2C_Init(void);
void SoftI2C_Start(void);
void SoftI2C_Stop(void);
void SoftI2C_SendByte(uint8_t byte);
uint8_t SoftI2C_ReadByte(uint8_t ack);
void SoftI2C_WriteByte(uint8_t memAddr, uint8_t data);
uint8_t SoftI2C_ReadByteFrom(uint8_t memAddr);
void SoftI2C_WritePage(uint8_t memAddr, const uint8_t *data, uint8_t len);
void SoftI2C_WriteBytes(uint8_t memAddr, const uint8_t *data, uint16_t len);
void SoftI2C_ReadBytes(uint8_t memAddr, uint8_t *data, uint16_t len);

#endif




