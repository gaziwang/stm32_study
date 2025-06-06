#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h" // 标准库核心头文件

void Delay_Init(void);   // SysTick初始化（配置1ms中断）
void Delay_ms(uint32_t ms); // 毫秒延时
void Delay_us(uint32_t us); // 微秒延时
uint32_t millis(void); // 获取当前毫秒计数
#endif

