#ifndef __LIGHTSENSOR_H
#define __LIGHTSENSOR_H

#include "stm32f10x.h"
#include "Delay.h"
void LIGHTSENSOR_Init(void);
uint16_t Get_LightIntensity(void);

#endif
