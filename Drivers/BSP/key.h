#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"
#include "Delay.h"
void KEY_Init(void);
uint8_t Get_keypress(void);
void EXTI_key1_config(void);       // Configure the external interrupt for key 1
static void NVIC_Key1config(void); // Configure the NVIC for key 1 interrupts

#endif
