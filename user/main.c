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
void RCC_config(uint32_t RCC_PLLMul_x);
void NVIC_config(void);
void EXTI_key_config(void);
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

    USART1_Init(); // Initialize USART1 for debugging or communication
    USART3_Init(); // Initialize USART3 if needed
    USART_SendString(USART1, "System Initialized\r\n");
    USART_SendFloat(USART3, 3.1415926f); // Send initialization message via USART3
    // while (1) {
    //  key_flag = Get_keypress(); // Check if key 0 is pressed
    //  if (key_flag == 1) {
    //      LED0_On(); // Turn on LED0
    //      Delay_ms(1000);
    //      LED0_Off(); // Turn off LED0
    //      Delay_ms(1000);
    //      LED0_Toggle(); // Wait for 1 second
    //  }
    //  if (key_flag == 2) {
    //      LED0_Off();     // Turn off LED0
    //      BEEP_On();      // Turn on BEEP
    //      Delay_ms(1000); // Wait for 1 second
    //      BEEP_Off();     // Turn off BEEP
    //  }
    //  if (key_flag == 3) {
    //      LED1_On(); // Turn on LED1
    //      Delay_ms(1000);
    //      LED1_Off();     // Turn off LED1
    //      Delay_ms(1000); // Wait for 1 second
    //  }
    //  if (Get_LightIntensity()) { // Check if the light intensity is below a threshold
    //      LED1_On(); // Turn on LED1 if the light intensity is low
    //      BEEP_Off(); // Turn off BEEP if the light intensity is low
    //  } else {
    //      LED1_Off();
    //      BEEP_On(); // Turn on BEEP if the light intensity is sufficient
    //  }
    //}
}
