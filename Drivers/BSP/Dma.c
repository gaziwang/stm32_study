#include "Dma.h"

/*
    DMA_InitTypeDef DMA_InitStructure;
// 源地址: 数据从哪里来 (内存地址 或 外设寄存器地址)
DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(Periph_Data_Register); // 外设作为源/目标
// 目标地址: 数据搬到哪里去 (内存地址 或 外设寄存器地址)
DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)Memory_Buffer_Address;   // 内存作为源/目标
// 传输方向 (二选一):
DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // 外设 -> 内存 (如 ADC 采集)
DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; // 内存 -> 外设 (如 串口发送)
// 或 (仅用于内存->内存)
// DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // 对于 M2M, 源内存地址其实填在 PeripheralBaseAddr
// DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; // 目标内存地址填在 MemoryBaseAddr (需要配合 M2M=Enable)
// 要传输的数据量 (单位是 "数据项" 的个数)
DMA_InitStructure.DMA_BufferSize = NumberOfDataItems; // 例如 100, 1000
// 外设地址是否递增? (通常是 不递增, 因为外设寄存器地址固定)
DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // 固定地址 (如 USART->DR)
// 或 DMA_PeripheralInc_Enable; // 少数外设有多个连续数据寄存器可能需要递增
// 内存地址是否递增? (通常是 递增, 因为我们操作的是数组/缓冲区)
DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // 递增 (操作数组)
// 或 DMA_MemoryInc_Disable; // 固定地址 (操作单个变量)
// 外设数据宽度 (数据项的大小): 与外设寄存器匹配
DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  // 8位
// 或 DMA_PeripheralDataSize_HalfWord; // 16位
// 或 DMA_PeripheralDataSize_Word;    // 32位
// 内存数据宽度 (数据项的大小): 与内存缓冲区类型匹配
DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  // uint8_t buffer[]
// 或 DMA_MemoryDataSize_HalfWord; // uint16_t buffer[]
// 或 DMA_MemoryDataSize_Word;    // uint32_t buffer[]
// 重要: PeripheralDataSize 和 MemoryDataSize 通常要一致! 除非有特殊需求并清楚后果。
// 传输模式:
DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;   // 普通模式: 传输指定次数后停止
// 或 DMA_Mode_Circular; // 循环模式: 传输完指定次数后自动从头开始(计数器/地址重置), 用于连续采集/发送
// 通道优先级 (当多个DMA通道同时请求时, 谁先服务)
DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
// 或 DMA_Priority_Medium;
// 或 DMA_Priority_High;
// 或 DMA_Priority_VeryHigh;
// 内存到内存模式 (M2M) 开关: 仅当源和目标都是内存时才启用!
DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;    // 外设<->内存模式 (默认)
// 或 DMA_M2M_Enable;     // 内存->内存模式 (源地址填在 PeripheralBaseAddr!)
// 初始化指定的 DMA 通道 (通道号必须查手册确定!)
DMA_Init(DMAy_Channelx, &DMA_InitStructure); // 例如 DMA1_Channel4 用于 USART1_TX
*/

void DMA_M2M_Init(uint32_t *source, uint32_t *destination, uint32_t BufferSize)
{
    // Enable DMA1 clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_DeInit(DMA1_Channel1); // Reset DMA1 Channel 1 to default state
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)source;            // Source address (as PeripheralBaseAddr in M2M)
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)destination;       // Destination address (as MemoryBaseAddr in M2M)
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;       // Direction: source to destination (for M2M)
    DMA_InitStructure.DMA_BufferSize         = BufferSize;                  // Buffer size in data units
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Enable;    // Source address incremented
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;        // Destination address incremented
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; // Source data size: Word
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Word;     // Destination data size: Word
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;             // Normal mode
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;           // High priority
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Enable;              // Enable memory-to-memory transfer
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);                            // Initialize DMA1 Channel 1
    DMA_Cmd(DMA1_Channel1, ENABLE);                                         // Enable DMA1 Channel 1
}

void USART3_DMA_TX_Init(uint8_t *source, uint32_t BufferSize)
{  
    DMA_Cmd(DMA1_Channel2, DISABLE); // 1. 禁用 DMA，确保可配置
    DMA_ClearFlag(DMA1_FLAG_TC2 | DMA1_FLAG_TE2); // 2. 清除上次状态
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                      // 使能 DMA1 时钟
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);     // 外设地址: USART3 数据寄存器
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)source;            // 内存地址 (源数据)
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;       // 方向: 内存 -> 外设 (发送数据)
    DMA_InitStructure.DMA_BufferSize         = BufferSize;                  // 传输数据量
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;   // 外设地址不递增
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;        // 内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设数据大小: 字节
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;     // 内存数据大小: 字节
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;             // 普通模式
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;           // 高优先级
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;             // 禁用内存到内存模式
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);                            // 初始化 DMA1 通道 2
    USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);                          // 使能 USART3 的 DMA 发送请求
    DMA_Cmd(DMA1_Channel2, ENABLE);                                         // 使能 DMA1 通道 2
}
void USART3_DMA_RX_Init(uint8_t *destination, uint32_t BufferSize)
{
    // 1. 使能 DMA1 时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    // 2. 复位 DMA1 通道 3 (USART3_RX 使用通道 3)
    DMA_DeInit(DMA1_Channel3);
    // 3. 配置 DMA 参数
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);     // 外设地址: USART3 数据寄存器
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)destination;       // 内存地址 (目标缓冲区)
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;       // 方向: 外设到内存
    DMA_InitStructure.DMA_BufferSize         = BufferSize;                  // 传输数据量
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;   // 外设地址不递增
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;        // 内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设数据宽度: 字节
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;     // 内存数据宽度: 字节
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;           // 循环模式 (持续接收)
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;           // 高优先级
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;             // 禁用内存到内存模式

    DMA_Init(DMA1_Channel3, &DMA_InitStructure);
    // 4. 使能 USART3 的 DMA 接收请求
    USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
    // 5. 使能 DMA 通道
    DMA_Cmd(DMA1_Channel3, ENABLE);
}
