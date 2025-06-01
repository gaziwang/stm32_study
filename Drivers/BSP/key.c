#include "key.h"
void KEY_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;    // Assuming the key is connected to PA0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // Input with pull-down
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3 | GPIO_Pin_4; // Assuming the keys are connected to PA3 and PA4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;           // Input with pull-up
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}
uint8_t Get_keypress(void)
{
    uint8_t key_status = 0;
    //KEY0
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0) // Check if PA0 is pressed
    {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0){}
        Delay_ms(20);
        key_status = 1; // Key is pressed
    }
    //KEY1
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 0) // Check if PA0 is pressed
    {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 0){}
        Delay_ms(20);
        key_status = 2; // Key is pressed
    }
    //KEYUP
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1) // Check if PA0 is pressed
    {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1){}
        Delay_ms(20);
        key_status = 3; // Key is pressed
    }
    return key_status;
}


