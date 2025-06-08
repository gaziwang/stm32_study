#include "SPI.h"

void SPI2_Init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能GPIOB时钟
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;      // PB13 (SCK), PB14 (MISO), PB15 (MOSI)
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;  // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 设置GPIO速度
    GPIO_Init(GPIOB, &GPIO_InitStructure);            // 初始化GPIOB

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;           // PB13 (SCK), PB14 (MISO), PB15 (MOSI)
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING; // 浮动输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      // 设置GPIO速度
    GPIO_Init(GPIOB, &GPIO_InitStructure);                 // 初始化GPIOB

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;      // PB13 (SCK), PB14 (MISO), PB15 (MOSI)
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;  // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 设置GPIO速度
    GPIO_Init(GPIOB, &GPIO_InitStructure);            // 初始化GPIOB

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;      // PB12 (NSS)
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 设置GPIO速度
    GPIO_Init(GPIOB, &GPIO_InitStructure);            // 初始化GPIOB

    SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex; // 双线全双工
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;                 // 主模式
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;                 // 8位数据帧
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;                    // 时钟极性
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;                  // 时钟相位
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;                    // 软件NSS管理
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;        // 波特率预分频
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;                // MSB优先
    SPI_InitStructure.SPI_CRCPolynomial     = 0;                               // CRC多项式
    SPI_Init(SPI2, &SPI_InitStructure);                                        // 初始化SPI2
    SPI_Cmd(SPI2, ENABLE);                                                     // 使能SPI2
    GPIO_SetBits(GPIOB, GPIO_Pin_12);                                          // 设置NSS引脚为高电平
}

uint8_t SPI2_FLASH_Send_byte(uint8_t data)
{   //检查并且等待发送缓冲区空
    
    volatile int i = 1000;
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
    {
        if(i-- == 0) // 超时处理
        {
            return 0; // 返回0表示发送失败
        }

    }
    SPI_I2S_SendData(SPI2, data); // 发送数据
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
    {
        if(i-- == 0) // 超时处理
        {
            return 0; // 返回0表示接收失败
        }
    }
    return SPI_I2S_ReceiveData(SPI2); // 返回接收到的数据
}
uint8_t SPI2_FLASH_Recive_byte(void)
{
    return SPI2_FLASH_Send_byte(0xFF); // 发送一个空字节以接收数据
}


