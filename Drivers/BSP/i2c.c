#include "i2c.h"
#define DEV_ADDR  0xA0
#define PAGE_SIZE        8  // EEPROM 每页 8 字节，需按具体芯片手册修改

// ========== 硬件 I2C（I2C1）配置 ==========
void HardI2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // GPIOB 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);   // I2C1 时钟

    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;     // PB6(SCL), PB7(SDA)
    gpio.GPIO_Mode  = GPIO_Mode_AF_OD;             // 复用开漏
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio);

    I2C_InitTypeDef i2c;
    i2c.I2C_ClockSpeed          = 400000; // 400kHz
    i2c.I2C_Mode                = I2C_Mode_I2C;
    i2c.I2C_DutyCycle           = I2C_DutyCycle_2;
    i2c.I2C_OwnAddress1         = 0x00;
    i2c.I2C_Ack                 = I2C_Ack_Enable;
    i2c.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &i2c);
    I2C_Cmd(I2C1, ENABLE);
}

void HardI2C_WriteByte(uint8_t memAddr, uint8_t data)
{
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1, 0xA0, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C1, memAddr);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_SendData(I2C1, data);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTOP(I2C1, ENABLE);
    Delay_us(150);  // EEPRO200延迟
}

uint8_t HardI2C_ReadByte(uint8_t memAddr)
{
    uint8_t data;
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, 0xA0, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(I2C1, memAddr);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTART(I2C1, ENABLE);  // 重启
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, 0xA0, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    I2C_AcknowledgeConfig(I2C1, DISABLE);  // 最后1字节不应答
    data = I2C_ReceiveData(I2C1);
    I2C_GenerateSTOP(I2C1, ENABLE);
    I2C_AcknowledgeConfig(I2C1, ENABLE);
    Delay_us(200);
    return data;
}
// ========== 硬件 I2C 页写 ==========
void HardI2C_WritePage(uint8_t memAddr, const uint8_t *data, uint8_t len)
{
    if (len > PAGE_SIZE) len = PAGE_SIZE;

    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, DEV_ADDR, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C1, memAddr);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    for (uint8_t i = 0; i < len; i++) {
        I2C_SendData(I2C1, data[i]);
        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }

    I2C_GenerateSTOP(I2C1, ENABLE);
    Delay_us(500);  // 写入延迟
}

void HardI2C_WriteBytes(uint8_t memAddr, const uint8_t *data, uint16_t len)
{
    while (len > 0) {
        uint8_t page_offset = memAddr % PAGE_SIZE;
        uint8_t write_len = PAGE_SIZE - page_offset;
        if (write_len > len) write_len = len;

        HardI2C_WritePage(memAddr, data, write_len);

        memAddr += write_len;
        data += write_len;
        len  -= write_len;
    }
}

void HardI2C_ReadBytes(uint8_t memAddr, uint8_t *data, uint16_t len)
{
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, DEV_ADDR, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(I2C1, memAddr);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, DEV_ADDR, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    while (len--) {
        if (len == 0)
            I2C_AcknowledgeConfig(I2C1, DISABLE);
        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
        *data++ = I2C_ReceiveData(I2C1);
    }
    I2C_GenerateSTOP(I2C1, ENABLE);
    I2C_AcknowledgeConfig(I2C1, ENABLE);
    Delay_us(200);
}
// ========== 软件模拟 I2C 配置 ==========
#define I2C_PORT       GPIOB
#define I2C_SCL_PIN    GPIO_Pin_6
#define I2C_SDA_PIN    GPIO_Pin_7

#define SDA_HIGH()     GPIO_SetBits(I2C_PORT, I2C_SDA_PIN)
#define SDA_LOW()      GPIO_ResetBits(I2C_PORT, I2C_SDA_PIN)
#define SCL_HIGH()     GPIO_SetBits(I2C_PORT, I2C_SCL_PIN)
#define SCL_LOW()      GPIO_ResetBits(I2C_PORT, I2C_SCL_PIN)
#define READ_SDA()     GPIO_ReadInputDataBit(I2C_PORT, I2C_SDA_PIN)

static void SDA_IN(void)
{
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin = I2C_SDA_PIN;
    gpio.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(I2C_PORT, &gpio);
}

static void SDA_OUT(void)
{
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin = I2C_SDA_PIN;
    gpio.GPIO_Mode = GPIO_Mode_Out_OD;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_PORT, &gpio);
}

void SoftI2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
    gpio.GPIO_Mode = GPIO_Mode_Out_OD;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_PORT, &gpio);

    SDA_HIGH();
    SCL_HIGH();
}

void SoftI2C_Start(void)
{
    SDA_OUT();
    SDA_HIGH(); SCL_HIGH(); Delay_us(5);
    SDA_LOW();  Delay_us(5);
    SCL_LOW();
}

void SoftI2C_Stop(void)
{
    SDA_OUT();
    SCL_LOW(); SDA_LOW(); Delay_us(5);
    SCL_HIGH(); Delay_us(5);
    SDA_HIGH(); Delay_us(5);
}

void SoftI2C_SendByte(uint8_t byte)
{
    SDA_OUT();
    for (int i = 0; i < 8; i++) {
        (byte & 0x80) ? SDA_HIGH() : SDA_LOW();
        Delay_us(2);
        SCL_HIGH(); Delay_us(2);
        SCL_LOW();  Delay_us(2);
        byte <<= 1;
    }
}

uint8_t SoftI2C_ReadByte(uint8_t ack)
{
    uint8_t data = 0;
    SDA_IN();
    for (int i = 0; i < 8; i++) {
        data <<= 1;
        SCL_HIGH(); Delay_us(2);
        if (READ_SDA()) data |= 0x01;
        SCL_LOW(); Delay_us(2);
    }
    SDA_OUT();
    (ack) ? SDA_LOW() : SDA_HIGH();
    SCL_HIGH(); Delay_us(2);
    SCL_LOW(); SDA_HIGH();
    return data;
}

uint8_t SoftI2C_WaitAck(void)
{
    uint8_t ack;
    SDA_IN();
    SCL_HIGH(); Delay_us(2);
    ack = READ_SDA();
    SCL_LOW();
    SDA_OUT();
    return ack;
}

void SoftI2C_WriteByte(uint8_t memAddr, uint8_t data)
{
    SoftI2C_Start();
    SoftI2C_SendByte(0xA0); SoftI2C_WaitAck();
    SoftI2C_SendByte(memAddr); SoftI2C_WaitAck();
    SoftI2C_SendByte(data); SoftI2C_WaitAck();
    SoftI2C_Stop();
    Delay_us(200);
}

uint8_t SoftI2C_ReadByteFrom(uint8_t memAddr)
{
    uint8_t data;
    SoftI2C_Start();
    SoftI2C_SendByte(0xA0); SoftI2C_WaitAck();
    SoftI2C_SendByte(memAddr); SoftI2C_WaitAck();
    SoftI2C_Start();
    SoftI2C_SendByte(0xA1); SoftI2C_WaitAck();
    data = SoftI2C_ReadByte(0);
    SoftI2C_Stop();
    Delay_us(200);
    return data;
}
// ========== 软件 I2C 页写 ==========
void SoftI2C_WritePage(uint8_t memAddr, const uint8_t *data, uint8_t len)
{
    if (len > PAGE_SIZE) len = PAGE_SIZE;

    SoftI2C_Start();
    SoftI2C_SendByte(DEV_ADDR); SoftI2C_WaitAck();
    SoftI2C_SendByte(memAddr);         SoftI2C_WaitAck();

    for (uint8_t i = 0; i < len; i++) {
        SoftI2C_SendByte(data[i]); SoftI2C_WaitAck();
    }
    SoftI2C_Stop();
    Delay_us(500);
}

void SoftI2C_WriteBytes(uint8_t memAddr, const uint8_t *data, uint16_t len)
{
    while (len > 0) {
        uint8_t page_offset = memAddr % PAGE_SIZE;
        uint8_t write_len = PAGE_SIZE - page_offset;
        if (write_len > len) write_len = len;

        SoftI2C_WritePage(memAddr, data, write_len);

        memAddr += write_len;
        data += write_len;
        len  -= write_len;
    }
}

void SoftI2C_ReadBytes(uint8_t memAddr, uint8_t *data, uint16_t len)
{
    SoftI2C_Start();
    SoftI2C_SendByte(DEV_ADDR); SoftI2C_WaitAck();
    SoftI2C_SendByte(memAddr);         SoftI2C_WaitAck();

    SoftI2C_Start();
    SoftI2C_SendByte(DEV_ADDR | 0x01); SoftI2C_WaitAck();

    while (len--) {
        *data++ = SoftI2C_ReadByte(len != 0);
    }
    SoftI2C_Stop();
    Delay_us(200);
}

