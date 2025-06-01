#include "key.h"

static void NVIC_Key1config(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
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
void EXTI_key1_config(void)
{ // KEY1
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;       // Assuming the key is connected to pin 3
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;    // Input Pull-Up mode
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; // Speed can be adjusted as needed
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);        // Enable clock for GPIOE
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3); // Configure the EXTI line for pin 3
    NVIC_Key1config();

    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line    = EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

