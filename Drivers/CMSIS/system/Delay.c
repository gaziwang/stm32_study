#include "Delay.h"

// 全局变量，用于记录延时时间
volatile uint32_t SysTickCounter = 0;
// Systick 初始化（1ms 中断）
void Delay_Init(void) {
    // 设置 Systick 时钟源为 HCLK（72MHz），重装载值为 72000-1（1ms）
    if (SysTick_Config(SystemCoreClock / 1000)) {
        // 如果失败，进入错误处理
        while (1);
    }
}

// Systick 中断服务函数（全局变量自增）
void SysTick_Handler(void) {
    SysTickCounter++;
}

// 毫秒延时函数
void Delay_ms(uint32_t ms) {
    uint32_t current = SysTickCounter;
    while ((SysTickCounter - current) < ms);
}

// 微秒延时函数（适用于短时间延时）
void Delay_us(uint32_t us) {
    // 72MHz 下，每循环约 1/72 ≈ 0.0139us，需调整参数
    us *= 72; // 1us ≈ 72 cycles
    while (us--) {
        __NOP(); // 空操作，确保编译器不优化掉循环
    }
}

