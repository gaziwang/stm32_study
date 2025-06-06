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
#include "i2c.h"
extern uint8_t key_flag;
uint8_t RX_Buffer[256];               // 接收缓冲区
uint8_t TX_Buffer[] = "Test666!\r\n"; // Send buffer (ASCII only)

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
    // I2C1_Init();
    // EEPROM_WritePage(0x00, (uint8_t *)"Hellowb", 12); // 写入数据到EEPROM
    // uint8_t read_buffer[20];
    // EEPROM_ReadBytes(0x00, read_buffer, 13); // 从EEPROM读取数据
    
    SoftI2C_Init(); // 初始化模拟I2C
    SoftI2C_WriteByte(0xA0, 0x00, 0xAA); // 写入数据到EEPROM
    uint8_t read_data = SoftI2C_ReadByte(0xA0, 0x00); // 从EEPROM读取数据
    printf("Read data: 0x%02X\r\n", read_data); // 打印读取的数据

    while (1) {
        if (key_flag) {
            key_flag = 0;
            // 准备要发送的数据
            uint32_t len = sizeof(TX_Buffer) - 1; // 减去字符串结束符
            // 使用DMA发送
            USART3_DMA_TX_Init(TX_Buffer, len); // 初始化并启动发送

            // 等待发送完成
            if (DMA_GetFlagStatus(DMA1_FLAG_TE2)) {
                USART_SendString(USART3, "DMA transfer error!\r");
                DMA_ClearFlag(DMA1_FLAG_TE2);
            }
            while (DMA_GetFlagStatus(DMA1_FLAG_TC2) == RESET);
            DMA_ClearFlag(DMA1_FLAG_TC2);
            USART_SendString(USART3, "DMA transfer complete!\n");
        }
    }
}
