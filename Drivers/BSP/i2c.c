#include "i2c.h"

void I2C1_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能GPIOB时钟

    // 配置I2C1的SDA和SCL引脚
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7; // PB6（SCL）和PB7（SDA）
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;         // 复用开漏输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 使能I2C外设时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    // 配置I2C外设
    I2C_InitTypeDef I2C1_InitStructure;
    I2C1_InitStructure.I2C_ClockSpeed = 400000; // 400kHz
    // I2C_StructInit(&I2C1_InitStructure);
    I2C1_InitStructure.I2C_Mode                = I2C_Mode_I2C;                 // I2C模式
    I2C1_InitStructure.I2C_DutyCycle           = I2C_DutyCycle_2;              // 快速模式占空比
    I2C1_InitStructure.I2C_OwnAddress1         = 0x00;                         // 主模式下本机地址无关
    I2C1_InitStructure.I2C_Ack                 = I2C_Ack_Enable;               // 使能应答
    I2C1_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // 7位地址模式
    I2C_Init(I2C1, &I2C1_InitStructure);

    // 使能I2C外设
    I2C_Cmd(I2C1, ENABLE);
}

void EEprom_SendByte(uint8_t address, uint8_t data)
{
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); // 等待总线空闲

    I2C_GenerateSTART(I2C1, ENABLE); // 发送起始信号
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1, 0xA0, I2C_Direction_Transmitter); // 发送器件地址
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C1, address); // 发送内存地址
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_SendData(I2C1, data); // 发送数据
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTOP(I2C1, ENABLE); // 发送停止信号
    // EEPROM写入需要时间，延迟确保完成（如5ms）
    Delay_Init();
    Delay_ms(5); // 等待写入完成
}
void EEprom_ReadByte(uint8_t address, uint8_t *data)
{
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1, 0xA0, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C1, address); // 设置地址
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTART(I2C1, ENABLE);                                        // 重启
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));            // 等待起始信号发送完成
    I2C_Send7bitAddress(I2C1, address, I2C_Direction_Receiver);             // 发送设备地址
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)); // 等待设备地址发送完成
    *data = I2C_ReceiveData(I2C1);                                          // 接收数据
    I2C_GenerateSTOP(I2C1, ENABLE);                                         // 发送停止信号

    Delay_Init();
    Delay_ms(5); // 等待数据稳定
}
void EEPROM_WritePage(uint8_t addr, uint8_t *data, uint8_t len)
{
    if (len > 8) len = 8; // 防止超过页大小

    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1, 0xA0, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C1, addr);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    for (uint8_t i = 0; i < len; i++) {
        I2C_SendData(I2C1, data[i]);
        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }

    I2C_GenerateSTOP(I2C1, ENABLE);
    Delay_Init();
    Delay_ms(5); // 等待写入完成
}
void EEPROM_ReadBytes(uint8_t addr, uint8_t *buffer, uint16_t len)
{
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

    // 设置读取起始地址
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1, 0xA0, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C1, addr);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // 重新启动，开始读取
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1, 0xA0, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    // 读取 N - 1 字节，发 ACK
    for (uint16_t i = 0; i < len - 1; i++) {
        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
        buffer[i] = I2C_ReceiveData(I2C1);
    }

    // 最后一个字节，发 NACK + STOP
    I2C_AcknowledgeConfig(I2C1, DISABLE);
    I2C_GenerateSTOP(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
    buffer[len - 1] = I2C_ReceiveData(I2C1);
    I2C_AcknowledgeConfig(I2C1, ENABLE); // 恢复ACK
}
