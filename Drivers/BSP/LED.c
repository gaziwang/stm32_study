#include "LED.h"


void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // Enable clock for GPIOE
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    // Configure GPIOB pin 5 as output push-pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; // Assuming the LED is connected to pin 5
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_5); // Set pin 5 high to turn off the LED
    GPIO_SetBits(GPIOE, GPIO_Pin_5); // Set pin 5 high to turn off the LED
}
void LED0_On(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_5); // Set pin 5 high to turn on the LED
}
void LED0_Off(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_5); // Set pin 5 low to turn off the LED
}
void LED1_On(void)
{
    GPIO_ResetBits(GPIOE, GPIO_Pin_5); // Set pin 5 high to turn on the LED
}
void LED1_Off(void)
{
    GPIO_SetBits(GPIOE, GPIO_Pin_5); // Set pin 5 low to turn off the LED
}
void LED0_Toggle(void)
{
    GPIOB->ODR ^= GPIO_Pin_5; // Toggle pin 5 state
}
void LED1_Toggle(void)
{
    GPIOE->ODR ^= GPIO_Pin_5; // Toggle pin 5 state
}

