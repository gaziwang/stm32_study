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
extern uint8_t key_flag;
uint8_t RX_Buffer[1000]; // 接收缓冲区
uint8_t TX_Buffer[] = "Hello from USART3 DMA!\r\n"; // 发送缓冲区

int main(void)
{
    RCC_config(RCC_PLLMul_9);
    Delay_Init();       // Configure the system clock to 72MHz using PLL with HSE
    LED_Init();         // Initialize the LED
    BEEP_Init();        // Initialize the BEEP
    KEY_Init();         // Initialize the keys
    LIGHTSENSOR_Init(); // Initialize the light sensor
    // 1. 先初始化USART3
    USART3_Init(); // 确保传入波特率参数
    USART_SendString(USART3, "Testing DMA...\r\n");
    EXTI_key1_config();
    USART3_DMA_RX_Init(RX_Buffer, sizeof(RX_Buffer));

  while (1) {
        // 这里可以添加按键检测触发DMA发送
        if(key_flag) {
            key_flag = 0;
            
            // 准备要发送的数据
            uint32_t len = sizeof(TX_Buffer) - 1; // 减去字符串结束符
            
            // 使用DMA发送
            USART3_DMA_TX_Init(TX_Buffer, len); // 初始化并启动发送
            
            // 等待发送完成
            if(DMA_GetFlagStatus(DMA1_FLAG_TE2)) {
                USART_SendString(USART3, "DMA transfer error!\r\n");
                DMA_ClearFlag(DMA1_FLAG_TE2);
            }
            while(DMA_GetFlagStatus(DMA1_FLAG_TC2) == RESET);
            DMA_ClearFlag(DMA1_FLAG_TC2);
        }
        
        // 检查接收数据
        uint32_t received = sizeof(RX_Buffer) - DMA_GetCurrDataCounter(DMA1_Channel3);
        if(received > 0) {
            // 处理接收到的数据
            USART_SendString(USART3, "Received data: ");
            USART_SendData(USART3, RX_Buffer[0]); // 示例：发送第一个字节
        }
    }

}

