#include "HSE_Set.h"

void RCC_config(uint32_t RCC_PLLMul_x)
{
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);                    // Enable HSE
    ErrorStatus status = RCC_WaitForHSEStartUp(); // Wait for HSE to be ready
    if (status == SUCCESS) {
        // Flash性能优化（高频必配）
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        FLASH_SetLatency(FLASH_Latency_2);

        // 配置PLL（HSE→PLL→72MHz）
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_x);
        RCC_PLLCmd(ENABLE);
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); // 等待PLL锁定

        // 总线分频（AHB=72MHz, APB1=36MHz, APB2=72MHz）
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        RCC_PCLK1Config(RCC_HCLK_Div2);
        RCC_PCLK2Config(RCC_HCLK_Div1);

        // 切换系统时钟到PLL（关键：先切时钟，再关HSI）
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        SystemCoreClockUpdate(); // 更新全局时钟变量

        RCC_HSICmd(DISABLE); // 系统稳定后，关闭未使用的HSI
    } else {
        RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        RCC_PCLK1Config(RCC_HCLK_Div1);
        RCC_PCLK2Config(RCC_HCLK_Div1);
        SystemCoreClockUpdate();
        while (1); // 死循环提示错误（或自定义处理）
    }
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); // Enable clock for GPIOE
}
