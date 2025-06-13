#include "FSMC_LCD.h"

/*LCD控制配置*/

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
{   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);
    FSMC_NORSRAMTimingInitTypeDef ReadWriteTiming;
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
    FSMC_NORSRAMInitStruct.FSMC_ExtendedMode       = FSMC_ExtendedMode_Disable;             // 禁用扩展模式（除非需要独立的读写时序）
    FSMC_NORSRAMInitStruct.FSMC_WriteBurst         = FSMC_WriteBurst_Disable;               // 禁用写突发（仅PSRAM需要）

    ReadWriteTiming.FSMC_AddressSetupTime      = 1;                 // 地址建立时间（HCLK周期数）
    ReadWriteTiming.FSMC_AddressHoldTime       = 0;                 // 地址保持时间（HCLK周期数）
    ReadWriteTiming.FSMC_DataSetupTime         = 2;                 // 数据建立时间（HCLK周期数）
    ReadWriteTiming.FSMC_BusTurnAroundDuration = 0;                 // 总线周转时间（HCLK周期数）
    ReadWriteTiming.FSMC_CLKDivision           = 0;                 // 时钟分频（仅同步模式使用）
    ReadWriteTiming.FSMC_DataLatency           = 0;                 // 数据延迟（仅同步模式使用）
    ReadWriteTiming.FSMC_AccessMode            = FSMC_AccessMode_A; // 访问模式A（根据存储器类型选择）

    FSMC_NORSRAMInitStruct.FSMC_ReadWriteTimingStruct = &ReadWriteTiming;

    // 3. 扩展模式时序（如果启用扩展模式）
    FSMC_NORSRAMTimingInitTypeDef writeTiming;                    // 可单独配置写时序（可选）
    FSMC_NORSRAMInitStruct.FSMC_WriteTimingStruct = &writeTiming; // 如果禁用扩展模式，设为NULL

    // 4. 调用初始化函数
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStruct);

    // 5. 使能 FSMC Bank
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);
}
