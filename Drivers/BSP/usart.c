#include "usart.h"

static void NVIC_USART1Config(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
static void NVIC_USART3Config(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
static void NVIC_UART4Config(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void USART1_Init(void)
{   //打开GPIOA和USART1的时钟
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); // Enable USART1 and GPIOA clock

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9; // PA9 (TX)
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;          // Alternate function push-pull
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // Configure PA10 (RX) as input floating
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10; // PA10 (RX)
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING; // Input floating
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    USART_DeInit(USART1); // Reset USART1 to default state
    USART_InitTypeDef USART1_InitStructure;
    USART1_InitStructure.USART_BaudRate            = 115200;
    USART1_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART1_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART1_InitStructure.USART_Parity              = USART_Parity_No;
    USART1_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
    USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART1_InitStructure);

    NVIC_USART1Config(); // Configure NVIC for USART1 interrupts
    USART_Cmd(USART1, ENABLE);
}

void USART3_Init(void)
{   //打开GPIOC和USART3的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10; // PB10 (TX)
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;          // Alternate function push-pull
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Configure PB11 (RX) as input floating
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11; // PB11 (RX)
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING; // Input floating
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    USART_DeInit(USART3); // Reset USART3 to default state
    USART_InitTypeDef USART3_InitStructure;
    USART3_InitStructure.USART_BaudRate            = 115200;
    USART3_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART3_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART3_InitStructure.USART_Parity              = USART_Parity_No;
    USART3_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
    USART3_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART3, &USART3_InitStructure);

    NVIC_USART3Config(); // Configure NVIC for USART3 interrupts
    USART_Cmd(USART3, ENABLE);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // Enable RXNE interrupt for USART3
}
void USART4_Init(void)
{
    //打开GPIOC和USART4的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10; // PC10 (TX)
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;          // Alternate function push-pull
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // Configure PC11 (RX) as input floating
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11; // PC11 (RX)
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING; // Input floating
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    USART_DeInit(UART4); // Reset UART4 to default state
    USART_InitTypeDef UART4_InitStructure;
    UART4_InitStructure.USART_BaudRate            = 115200;
    UART4_InitStructure.USART_WordLength          = USART_WordLength_8b;
    UART4_InitStructure.USART_StopBits            = USART_StopBits_1;
    UART4_InitStructure.USART_Parity              = USART_Parity_No;
    UART4_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
    UART4_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(UART4, &UART4_InitStructure);

    NVIC_UART4Config(); // Configure NVIC for UART4 interrupts
    USART_Cmd(UART4, ENABLE);
}
/**
 * @brief 串口单字节发送（基础函数）
 * @param USARTx: 串口外设（如 USART1/USART2）
 * @param data: 要发送的单字节数据（uint16_t 兼容 ASCII/汉字字节）
 */
void USART_SendByte(USART_TypeDef* USARTx, uint8_t data)
{
    // 等待“发送缓冲区空”标志（TXE）置位
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);  
    USART_SendData(USARTx, data); // 发送单字节
}

/**
 * @brief 串口发送字符串（支持 ASCII、汉字、数字转字符串后的数据）
 * @param USARTx: 串口外设（如 USART1/USART2）
 * @param str: 要发送的字符串指针（以 '\0' 结尾）
 * @note 汉字需保证编码一致（如 UTF-8/GBK），串口助手需对应配置编码
 */
void USART_SendString(USART_TypeDef* USARTx, char* str)
{
    while (*str != '\0') // 遍历到字符串结束符 '\0'
    {
        // 等待“发送缓冲区空”标志（TXE）置位
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);

        USART_SendByte(USARTx, (uint16_t)*str); // 发送当前字符（强转为 uint16_t 匹配函数参数）
        str++; // 指向下一个字符（汉字占2-4字节时，会逐字节发送）
    }
    USART_SendByte(USARTx, '\r'); // 发送回车符
    USART_SendByte(USARTx, '\n'); // 发送换行符
}

/**
 * @brief 串口发送整数（先转字符串再发送）
 * @param USARTx: 串口外设
 * @param num: 要发送的整数
 */
void USART_SendInt(USART_TypeDef* USARTx, int num)
{
    // 等待“发送缓冲区空”标志（TXE）置位
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    char buf[20]; // 足够存储整数的字符串缓冲区
    sprintf(buf, "%d", num); // 整数转字符串（如 123 → "123"）
    USART_SendString(USARTx, buf);
}

/**
 * @brief 串口发送浮点数（先转字符串再发送，保留2位小数）
 * @param USARTx: 串口外设
 * @param num: 要发送的浮点数
 */
void USART_SendFloat(USART_TypeDef* USARTx, float num)
{
    char buf[20];
    // 等待“发送缓冲区空”标志（TXE）置位
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    sprintf(buf, "%.5f", num); // 浮点数转字符串（如 3.14 → "3.14"）
    USART_SendString(USARTx, buf);
}

int fputc(int ch, FILE *f)
{
    // 等待“发送缓冲区空”标志（TXE）置位
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);  
    // 发送当前字符（强转为 uint8_t）
    USART_SendData(USART3, (uint8_t)ch);
    return ch; // 必须返回字符，否则 printf 会异常
}

