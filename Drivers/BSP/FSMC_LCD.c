#include "FSMC_LCD.h"

/*LCD控制配置*/
#define LCD_BASE_ADDR  0x6C000000
#define LCD_CMD_ADDR     LCD_BASE_ADDR
#define LCD_DATA_ADDR    0X6C000800
#define LCD_CMD   *((volatile uint16_t*)LCD_CMD_ADDR)
#define LCD_DATA  *((volatile uint16_t*)LCD_DATA_ADDR)
#define ILI9341_READ_ID4    0xD3    // 读取ID命令
void LCD_GPIO_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE);
    /*CS引脚*/
    GPIO_InitTypeDef GPIO_InitStructure;
    /*CS\FSMC_NE4*/
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12; // CS引脚
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
    /*RS\FSMC_A10*/
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0; // RS引脚
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
    /*WR\FSMC_NWE*/
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5; // WR引脚
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    /*RD\FSMC_NOE*/
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4; // RS引脚
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    /*背光引脚*/
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0; // BL引脚
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*FSMC引脚*/
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_15 | GPIO_Pin_0 | GPIO_Pin_14 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}
void LCD_FSMC_Config(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    FSMC_NORSRAMTimingInitTypeDef ReadTiming;
    FSMC_NORSRAMTimingInitTypeDef WriteTiming;
    FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStruct;
    FSMC_NORSRAMInitStruct.FSMC_Bank               = FSMC_Bank1_NORSRAM4;                   // 使用 Bank1 的第一个 NOR/SRAM 区域
    FSMC_NORSRAMInitStruct.FSMC_DataAddressMux     = FSMC_DataAddressMux_Disable;           // 禁用数据/地址复用
    FSMC_NORSRAMInitStruct.FSMC_MemoryType         = FSMC_MemoryType_SRAM;                  // 存储器类型为 SRAM（或 FSMC_MemoryType_NOR）
    FSMC_NORSRAMInitStruct.FSMC_MemoryDataWidth    = FSMC_MemoryDataWidth_16b;              // 16位数据宽度（或 FSMC_MemoryDataWidth_8b）
    FSMC_NORSRAMInitStruct.FSMC_BurstAccessMode    = FSMC_BurstAccessMode_Disable;          // 禁用突发访问（仅PSRAM需要）
    FSMC_NORSRAMInitStruct.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;           // 等待信号低电平有效
    FSMC_NORSRAMInitStruct.FSMC_WrapMode           = FSMC_WrapMode_Disable;                 // 禁用包装模式（仅PSRAM需要）
    FSMC_NORSRAMInitStruct.FSMC_WaitSignalActive   = FSMC_WaitSignalActive_BeforeWaitState; // 等待信号在等待状态前有效
    FSMC_NORSRAMInitStruct.FSMC_WriteOperation     = FSMC_WriteOperation_Enable;            // 使能写操作
    FSMC_NORSRAMInitStruct.FSMC_WaitSignal         = FSMC_WaitSignal_Disable;               // 禁用等待信号（通常禁用）
    FSMC_NORSRAMInitStruct.FSMC_ExtendedMode       = FSMC_ExtendedMode_Enable;             // 禁用扩展模式（除非需要独立的读写时序）
    FSMC_NORSRAMInitStruct.FSMC_WriteBurst         = FSMC_WriteBurst_Disable;               // 禁用写突发（仅PSRAM需要）

    ReadTiming.FSMC_AddressSetupTime                  = 0;                 // 地址建立时间（HCLK周期数）
    ReadTiming.FSMC_AddressHoldTime                   = 0;                 // 地址保持时间（HCLK周期数）
    ReadTiming.FSMC_DataSetupTime                     = 15;                // 数据建立时间（HCLK周期数）
    ReadTiming.FSMC_BusTurnAroundDuration             = 0;                 // 总线周转时间（HCLK周期数）
    ReadTiming.FSMC_CLKDivision                       = 0;                 // 时钟分频（仅同步模式使用）
    ReadTiming.FSMC_DataLatency                       = 0;                 // 数据延迟（仅同步模式使用）
    ReadTiming.FSMC_AccessMode                        = FSMC_AccessMode_A; // 访问模式A（根据存储器类型选择）
    FSMC_NORSRAMInitStruct.FSMC_ReadWriteTimingStruct = &ReadTiming; 

    WriteTiming.FSMC_AddressSetupTime                 = 0;
    WriteTiming.FSMC_AddressHoldTime                  = 0;
    WriteTiming.FSMC_DataSetupTime                    = 1;
    WriteTiming.FSMC_AccessMode                       = FSMC_AccessMode_A;
    FSMC_NORSRAMInitStruct.FSMC_WriteTimingStruct = &WriteTiming;
    // 4. 调用初始化函数
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStruct);
    // 5. 使能 FSMC Bank
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);
}

void LCD_WriteCmd(uint16_t cmd)
{
    LCD_CMD = cmd;
    Delay_us(1);  // 短暂延时确保命令写入
}

/**
 * @brief 从LCD读取数据
 * @return 读取到的数据
 */
uint16_t LCD_ReadData(void)
{
    Delay_us(1);  // 短暂延时确保数据稳定
    return LCD_DATA;
}

/**
 * @brief 读取ILI9341的ID
 * @return 32位ID值，格式为0x00XXYYZZ，其中XX YY ZZ为ID的三个字节
 */
uint32_t ILI9341_ReadID(void)
{
    uint32_t id = 0;
    uint16_t temp;
    
    // 发送读ID命令
    LCD_WriteCmd(ILI9341_READ_ID4);
    
    // 读取4个字节的ID数据
    // 第一个字节通常是虚拟数据，丢弃
    temp = LCD_ReadData();  // 虚拟读取
    
    // 读取实际的ID数据（3个字节）
    temp = LCD_ReadData();  // ID1
    id = (uint32_t)(temp & 0xFF) << 16;
    
    temp = LCD_ReadData();  // ID2  
    id |= (uint32_t)(temp & 0xFF) << 8;
    
    temp = LCD_ReadData();  // ID3
    id |= (uint32_t)(temp & 0xFF);
    
    return id;
}
uint16_t ILI9341_Read_Display_Pixel_Format(void)
{
    
    uint16_t temp;
    // 发送读ID命令
    LCD_WriteCmd(0x0C);    // 读取4个字节的ID数据
    // 第一个字节通常是虚拟数据，丢弃
    temp = LCD_ReadData();  // 虚拟读取
    temp = LCD_ReadData();  // ID2  
    return temp;
}
