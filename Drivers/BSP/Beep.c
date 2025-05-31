#include "Beep.h"
void BEEP_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // Enable clock for GPIOB
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // Configure GPIOB pin 8 as output push-pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; // Assuming the BEEP is connected to pin 8
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
 

}
void BEEP_On(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_8); // Set pin 8 high to turn on the BEEP
}
void BEEP_Off(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_8); // Set pin 8 low to turn off the BEEP
}
void BEEP_Toggle(void)
{
    GPIOB->ODR ^= GPIO_Pin_8; // Toggle pin 8 state
}
