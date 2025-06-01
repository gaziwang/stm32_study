#include "lightsensor.h"

void LIGHTSENSOR_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE); // Enable GPIOF clock
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; // Assuming the light sensor is connected to PF8
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // Floating input mode
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; // Speed can be adjusted as needed
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}

uint16_t Get_LightIntensity(void)
{
    uint16_t light_intensity = 0;
    GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_8); // Read the input data to clear any previous state
    //值为1时 光线较暗    为零时  光线较亮
    return GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_8);
}
