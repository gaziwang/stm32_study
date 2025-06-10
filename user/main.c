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
#include "SPI.h"
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
    EXTI_key0_config();
    USART3_DMA_RX_Init(RX_Buffer, sizeof(RX_Buffer));
    // I2C1_Init();
    // EEPROM_WritePage(0x00, (uint8_t *)"Hellowb", 12); // 写入数据到EEPROM
    // uint8_t read_buffer[20];
    // EEPROM_ReadBytes(0x00, read_buffer, 13); // 从EEPROM读取数据

    SoftI2C_Init();                      // 初始化模拟I2C
    SoftI2C_WriteByte(0x00, 0x22);                  // 写入数据到EEPROM
    uint8_t read_data = SoftI2C_ReadByteFrom(0x00); // 从EEPROM读取数据
    printf("Read data: 0x%02X\r\n", read_data);     // 打印读取的数据
 


    SPI2_Init();
    printf("SPI Flash ID: 0x%06X\r\n ", SPI2_FLASH_ReadID()); // 打印Flash ID

    Delay_ms(2000); // 延时1秒
    uint8_t status[16] = {0};
	SPI2_FLASH_Write_Enable();
    SPI_FLASH_SectorErase(0x000000); // 擦除扇区0
    Delay_ms(100); // 等待擦除完成
    printf("Erasing sector 0...擦除成功\r\n");
    SPI2_FLASH_Write_Enable();
    GPIO_ResetBits(GPIOB, GPIO_Pin_12); // CS 拉低
    SPI2_FLASH_Send_byte(0x03);
    SPI2_FLASH_Recive_byte(0x00); 
    SPI2_FLASH_Recive_byte(0x00); 
    SPI2_FLASH_Recive_byte(0x00); 
    for (int i = 0; i < 3; i++) {
        status[i] = SPI2_FLASH_Recive_byte(0xFF);  // 接收数据
        printf("%02X",status[i]);
    }
    printf("\r\n");
    GPIO_SetBits(GPIOB, GPIO_Pin_12); // CS 拉高

    SPI2_FLASH_WriteByte(0x000000, (uint8_t *)"Hello, SPI Flash!", 18); // 写入数据到Flash
    Delay_ms(100); // 等待写入完成
    SPI2_FLASH_Write_Enable(); // 发送写使能指令
    GPIO_ResetBits(GPIOB, GPIO_Pin_12); // CS 拉低
    SPI2_FLASH_Send_byte(0x03); 
    SPI2_FLASH_Recive_byte(0x00); 
    SPI2_FLASH_Recive_byte(0x00); 
    SPI2_FLASH_Recive_byte(0x00); 
    for (int i = 0; i < 17; i++) {
        status[i] = SPI2_FLASH_Recive_byte(0xFF);  // 接收数据
    }
    printf("Status after write:%.*s\r\n", 17, status);
    for (int i = 0; i < 17; i++) {
        printf("%c", status[i]);
    }
    printf("\r\n");
    GPIO_SetBits(GPIOB, GPIO_Pin_12); // CS 拉高

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
