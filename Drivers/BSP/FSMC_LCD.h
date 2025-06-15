#ifndef _FSMC_LCD_H
#define _FSMC_LCD_H

#include "stm32f10x.h"
#include "Delay.h"
void LCD_GPIO_Init(void);
void LCD_FSMC_Config(void);
void LCD_WriteCmd(uint16_t cmd);
uint16_t LCD_ReadData(void);
uint32_t ILI9341_ReadID(void);
uint16_t ILI9341_Read_Display_Pixel_Format(void);
#endif /* _FSMC_H */

