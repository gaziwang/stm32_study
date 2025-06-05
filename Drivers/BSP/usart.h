#ifndef _USART_H
#define _USART_H

#include "stm32f10x.h"
#include <stdio.h>
#include "stm32f10x_usart.h"
void USART1_Init(void); // Initialize USART1
void USART3_Init(void); // Initialize USART3
void USART_SendByte(USART_TypeDef* USARTx, uint8_t data); // Send a single byte via USART
void USART_SendString(USART_TypeDef* USARTx, char* str); // Send a string via USART
void USART_SendInt(USART_TypeDef* USARTx, int num); // Send an integer via USART
void USART_SendFloat(USART_TypeDef* USARTx, float num); // Send a float via USART
int fputc(int ch, FILE *f); // Redirect printf to USART3
static void NVIC_USART1Config(void); // Configure NVIC for USART1 interrupts
static void NVIC_USART3Config(void); // Configure NVIC for USART3 interrupts

#endif


