#include "stm32f10x.h"
#include "Delay.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_it.h"
#include "misc.h"
#include "LED.h"
#include "Beep.h"
#include "key.h"
#include "lightsensor.h"
void RCC_config(uint32_t RCC_PLLMul_x);
void NVIC_config(void);
void EXTI_key_config(void);
uint8_t key_flag = 0; // Flag to indicate if the key was pressed
int main(void)
{

    RCC_config(RCC_PLLMul_9);
    Delay_Init();      // Configure the system clock to 72MHz using PLL with HSE
    LED_Init();        // Initialize the LED
    BEEP_Init();       // Initialize the BEEP
    KEY_Init();        // Initialize the keys
    LIGHTSENSOR_Init(); // Initialize the light sensor
    EXTI_key_config(); // Configure the external interrupt for the key
    while (1) {
        // key_flag = Get_keypress(); // Check if key 0 is pressed
        // if (key_flag == 1) {
        //     LED0_On(); // Turn on LED0
        //     Delay_ms(1000);
        //     LED0_Off(); // Turn off LED0
        //     Delay_ms(1000);
        //     LED0_Toggle(); // Wait for 1 second
        // }
        // if (key_flag == 2) {
        //     LED0_Off();     // Turn off LED0
        //     BEEP_On();      // Turn on BEEP
        //     Delay_ms(1000); // Wait for 1 second
        //     BEEP_Off();     // Turn off BEEP
        // }
        // if (key_flag == 3) {
        //     LED1_On(); // Turn on LED1
        //     Delay_ms(1000);
        //     LED1_Off();     // Turn off LED1
        //     Delay_ms(1000); // Wait for 1 second
        // }
        if (Get_LightIntensity()) { // Check if the light intensity is below a threshold
            LED1_On(); // Turn on LED1 if the light intensity is low
            BEEP_Off(); // Turn off BEEP if the light intensity is low
        } else {
            LED1_Off();
            BEEP_On(); // Turn on BEEP if the light intensity is sufficient
        }
    }
}
void RCC_config(uint32_t RCC_PLLMul_x)
{
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);                    // Enable HSE
    ErrorStatus status = RCC_WaitForHSEStartUp(); // Wait for HSE to be ready
    if (status == SUCCESS) {
        // Flash性能优化（高频必配）
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        FLASH_SetLatency(FLASH_Latency_2);

        // 配置PLL（HSE→PLL→72MHz）
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_x);
        RCC_PLLCmd(ENABLE);
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); // 等待PLL锁定

        // 总线分频（AHB=72MHz, APB1=36MHz, APB2=72MHz）
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        RCC_PCLK1Config(RCC_HCLK_Div2);
        RCC_PCLK2Config(RCC_HCLK_Div1);

        // 切换系统时钟到PLL（关键：先切时钟，再关HSI）
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        SystemCoreClockUpdate(); // 更新全局时钟变量

        RCC_HSICmd(DISABLE); // 系统稳定后，关闭未使用的HSI
    } else {
        RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        RCC_PCLK1Config(RCC_HCLK_Div1);
        RCC_PCLK2Config(RCC_HCLK_Div1);
        SystemCoreClockUpdate();
        while (1); // 死循环提示错误（或自定义处理）
    }
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); // Enable clock for GPIOE
}
void NVIC_config(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void EXTI_key_config(void)
{ // KEY1
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;       // Assuming the key is connected to pin 3
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;    // Input Pull-Up mode
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; // Speed can be adjusted as needed
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);        // Enable clock for GPIOE
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3); // Configure the EXTI line for pin 3
    NVIC_config();

    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line    = EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}
