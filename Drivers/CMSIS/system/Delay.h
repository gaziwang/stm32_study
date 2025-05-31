#ifndef _DELAY_H
#define _DELAY_H

#include "stm32f10x.h"

void Delay_Init(void);
void Delay_ms(uint32_t ms);
void Delay_us(uint32_t us);

#endif

