#include "SPI.h"
#define PAGE_SIZE 256
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
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;                   // 时钟极性
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;                  // 时钟相位
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;                    // 软件NSS管理
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;         // 波特率预分频
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;                // MSB优先
    SPI_InitStructure.SPI_CRCPolynomial     = 0;                               // CRC多项式
    SPI_Init(SPI2, &SPI_InitStructure);                                        // 初始化SPI2
    SPI_Cmd(SPI2, ENABLE);                                                     // 使能SPI2
    GPIO_SetBits(GPIOB, GPIO_Pin_12);                                          // 设置NSS引脚为高电平
}
int SPI2_FLASH_Send_byte(uint8_t data)
{ // 检查并且等待发送缓冲区空
    volatile int i = 1000;
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) {
        if (i-- == 0) // 超时处理
        {
            return -1; // 返回-1表示发送失败
        }
    }
    SPI_I2S_SendData(SPI2, data); // 发送数据
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) {
        if (i-- == 0) // 超时处理
        {
            return -1; // 返回-1表示接收失败
        }
    }
    return SPI_I2S_ReceiveData(SPI2); // 返回接收到的数据
}
int SPI2_FLASH_Receive_byte(uint8_t dummy)
{
    return SPI2_FLASH_Send_byte(dummy); // 发送一个空字节以接收数据
}
void SPI2_FLASH_Write_Enable(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_12); // CS 拉低
    SPI2_FLASH_Send_byte(0x06);         // 发送写使能指令
    GPIO_SetBits(GPIOB, GPIO_Pin_12);   // CS 拉高
}
void SPI_FLASH_WaitBusy(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    SPI2_FLASH_Send_byte(0x05);                   // 读取状态寄存器命令
    while (SPI2_FLASH_Receive_byte(0xFF) & 0x01); // 等待BUSY清除
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
}
void SPI_FLASH_SectorErase(uint32_t sectorAddress)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);                 // CS 拉低
    SPI2_FLASH_Send_byte(0x20);                         // 发送扇区擦除指令
    SPI2_FLASH_Send_byte((sectorAddress >> 16) & 0xFF); // 发送高字节地址
    SPI2_FLASH_Send_byte((sectorAddress >> 8) & 0xFF);  // 发送中间字节地址
    SPI2_FLASH_Send_byte(sectorAddress & 0xFF);         // 发送低字节地址
    GPIO_SetBits(GPIOB, GPIO_Pin_12);                   // CS 拉高
    SPI_FLASH_WaitBusy();                               // 等待擦除完成
}
void SPI2_FLASH_WriteByte(uint32_t address, uint8_t *data, uint32_t length)
{
    SPI2_FLASH_Write_Enable();                    // 发送写使能指令
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);           // CS 拉低
    SPI2_FLASH_Send_byte(0x02);                   // 发送页编程指令
    SPI2_FLASH_Send_byte((address >> 16) & 0xFF); // 发送高字节地址
    SPI2_FLASH_Send_byte((address >> 8) & 0xFF);  // 发送中间字节地址
    SPI2_FLASH_Send_byte(address & 0xFF);         // 发送低字节地址

    for (uint32_t i = 0; i < length; i++) {
        SPI2_FLASH_Send_byte(data[i]); // 发送数据
    }
    GPIO_SetBits(GPIOB, GPIO_Pin_12); // CS 拉高
}
void SPI2_FLASH_WriteBytes(uint32_t address, const uint8_t *data, uint32_t length)
{
    while (length > 0) {
        uint32_t page_offset   = address % PAGE_SIZE;
        uint32_t space_in_page = PAGE_SIZE - page_offset;
        uint32_t chunk_size    = (length < space_in_page) ? length : space_in_page;

        SPI2_FLASH_Write_Enable();
        GPIO_ResetBits(GPIOB, GPIO_Pin_12); // CS 拉低

        SPI2_FLASH_Send_byte(0x02); // Page Program 指令
        SPI2_FLASH_Send_byte((address >> 16) & 0xFF);
        SPI2_FLASH_Send_byte((address >> 8) & 0xFF);
        SPI2_FLASH_Send_byte(address & 0xFF);

        for (uint32_t i = 0; i < chunk_size; i++) {
            SPI2_FLASH_Send_byte(data[i]);
        }
        GPIO_SetBits(GPIOB, GPIO_Pin_12); // CS 拉高
        SPI_FLASH_WaitBusy();             // 等待写完成
        // 更新指针和地址
        address += chunk_size;
        data += chunk_size;
        length -= chunk_size;
    }
}
uint32_t SPI2_FLASH_ReadID(void)
{
    uint8_t deviceID[3] = {0};

    SPI2_Init();
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);                            // CS 拉低
    SPI2_FLASH_Send_byte(0x9F);                                    // 发送 JEDEC ID 指令
    deviceID[0] = SPI2_FLASH_Receive_byte(0xFF);                   // Manufacturer ID
    deviceID[1] = SPI2_FLASH_Receive_byte(0xFF);                   // Memory Type
    deviceID[2] = SPI2_FLASH_Receive_byte(0xFF);                   // Capacity
    GPIO_SetBits(GPIOB, GPIO_Pin_12);                              // CS 拉高
    return (deviceID[0] << 16) | (deviceID[1] << 8) | deviceID[2]; // 返回ID
}
void SPI2_FLASH_PowerDown(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_12); // CS 拉低
    SPI2_FLASH_Send_byte(0xB9);         // 发送休眠指令
    GPIO_SetBits(GPIOB, GPIO_Pin_12);   // CS 拉高
}
void SPI2_FLASH_ReleasePowerDown(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_12); // CS 拉低
    SPI2_FLASH_Send_byte(0xAB);         // 发送唤醒指令
    GPIO_SetBits(GPIOB, GPIO_Pin_12);   // CS 拉高
}
int SPI2_FLASH_ReadData(uint32_t address, uint8_t *buffer, uint32_t length)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_12); // CS 拉低
    // 发送 READ DATA 指令 (0x03)
    SPI2_FLASH_Send_byte(0x03);
    // 发送地址 (24-bit)
    SPI2_FLASH_Send_byte((address >> 16) & 0xFF);
    SPI2_FLASH_Send_byte((address >> 8) & 0xFF);
    SPI2_FLASH_Send_byte(address & 0xFF);

    for (uint32_t i = 0; i < length; i++) {
        buffer[i] = SPI2_FLASH_Send_byte(0xFF); // 0xFF 是 Dummy Byte
    }
    GPIO_SetBits(GPIOB, GPIO_Pin_12); // CS 拉高
    return 0; // 返回0表示成功
}
