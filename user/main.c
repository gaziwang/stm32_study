#include "stm32f10x.h"
#include "Delay.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "LED.h"
#include "Beep.h"
void RCC_config(uint32_t RCC_PLLMul_x);
void NVIC_config(void);
void EXTI_key_config(void);

int main()
{
    //    Delay_Init();
    //    RCC_config(RCC_PLLMul_9); // Configure the system clock to 72MHz using PLL with HSE
    LED_Init();  // Initialize the LEDs
    BEEP_Init(); // Initialize the BEEP
    LED0_On();    // Turn on LED0
    LED1_Off();  // Turn off LED1

    BEEP_On(); // Turn on the BEEP
    Delay_ms(100); // Delay for 100 milliseconds to allow system stabilization
    BEEP_Off(); // Turn off the BEEP

    LED0_Off(); // Turn off LED0
    LED1_On();  // Turn on LED1

    // while (1) {
    //     LED0_On();     // Turn on LED0
    //     LED1_Off();    // Turn off LED1
    //     Delay_ms(100); // Delay for 1 second to allow system stabilization
    //     BEEP_On();     // Turn on the BEEP
    //     Delay_ms(100);
    //     BEEP_Off();    // Turn off the BEEP
    //     Delay_ms(100); // Wait for 1 second
    //     LED0_Off();    // Turn off LED0
    //     LED1_On();     // Turn on LED1
    //     Delay_ms(100); // Delay for 1 second
    // }
}
void RCC_config(uint32_t RCC_PLLMul_x)
{
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);                    // Enable HSE
    ErrorStatus status = RCC_WaitForHSEStartUp(); // Wait for HSE to be ready
    if (status == SUCCESS) {
        // Enable the FLASH prefetch buffer to improve performance by reducing wait states during memory access
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        FLASH_SetLatency(FLASH_Latency_2);                   // Set Flash latency to 2 wait states
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_x); // Configure PLL with HSE as source and multiplier of x
        RCC_PLLCmd(ENABLE);                                  // Enable PLL
        RCC_HCLKConfig(RCC_SYSCLK_Div1);                     // AHB = 72MHz
        RCC_PCLK1Config(RCC_HCLK_Div2);                      // APB1 = 36MHz
        RCC_PCLK2Config(RCC_HCLK_Div1);                      // APB2 = 72MHz
        RCC_HSICmd(DISABLE);
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
        }
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); // Set PLL as system clock source
    } else {
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
{
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
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}
