#include "stm32f10x.h"
#include "Delay.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_it.h"
#include "misc.h"
#include "LED.h"
#include "Beep.h"
#include "key.h"
#include "lightsensor.h"
#include "usart.h"
#include "HSE_Set.h"
#include "Dma.h"
uint8_t key_flag = 0; // Flag to indicate if the key was pressed
int main(void)
{
    RCC_config(RCC_PLLMul_9);
    Delay_Init();       // Configure the system clock to 72MHz using PLL with HSE
    LED_Init();         // Initialize the LED
    BEEP_Init();        // Initialize the BEEP
    KEY_Init();         // Initialize the keys
    LIGHTSENSOR_Init(); // Initialize the light sensor
    EXTI_key1_config(); // Configure the external interrupt for the key

    USART3_Init(); // Initialize USART3 if needed
    USART_SendString(USART3, "wangmengzhao666\r\n");
    USART_SendFloat(USART3, 3.1415926f);          // Send initialization message via USART3
    printf("wangdawdawdawdawdwadawdawdwadwaa\n"); // Print a message to the console
    printf("wangdawdawdawda\n");                  // Print another message to the console

    while (1) {
    }
}
