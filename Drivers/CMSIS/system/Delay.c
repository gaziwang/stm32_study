#include "Delay.h"

// 全局变量：记录SysTick中断次数（用于毫秒延时）
volatile uint32_t SysTick_Counter = 0;

/**
 * @brief 初始化SysTick定时器（配置为1ms中断）s
 * @note 依赖系统时钟（SystemCoreClock），需先配置系统时钟为72MHz
 */
void Delay_Init(void)
{
    // SysTick_Config函数功能：
    // 1. 设置重装载值 = SystemCoreClock / 1000 - 1 （对应1ms中断）
    // 2. 使能SysTick定时器
    // 3. 选择时钟源为HCLK（系统时钟）
    // 4. 使能SysTick中断
    if (SysTick_Config(SystemCoreClock / 1000)) 
    {
        // 配置失败时进入死循环（如系统时钟未正确配置）
        while (1); 
    }
}

/**
 * @brief SysTick中断服务函数（自动递增计数器）
 * @note 需在启动文件（如startup_stm32f10x_hd.s）中映射中断向量
 */

/**
 * @brief 毫秒延时函数（轮询SysTick计数器）
 * @param ms: 延时毫秒数（范围：0~2^32-1）
 */
void Delay_ms(uint32_t ms)
{
    uint32_t start = SysTick_Counter; // 记录初始计数
    while ((SysTick_Counter - start) < ms); // 等待计数达到目标
}

/**
 * @brief 微秒延时函数（空指令循环）
 * @param us: 延时微秒数（范围：0~2^32-1，实际受循环效率限制）
 * @note 72MHz系统时钟下，1us ≈ 72个空指令周期（需根据实际时钟调整）
 */
void Delay_us(uint32_t us)
{
    // 72MHz系统时钟下，1个空指令（__NOP()）耗时约 1/72 ≈ 0.0139us
    // 因此 1us ≈ 72 个空指令周期
    us *= 72; 

    while (us--)
    {
        __NOP(); // 空操作指令（编译器不会优化掉循环）
    }
}

/**
 * @brief 获取当前系统运行的毫秒数
 * @return 当前毫秒计数（从上电开始）
 */
uint32_t millis(void)
{
    return SysTick_Counter;
}




