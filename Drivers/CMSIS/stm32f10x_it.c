/**
 ******************************************************************************
 * @file    EXTI/EXTI_Config/stm32f10x_it.c
 * @author  MCD Application Team
 * @version V3.5.0
 * @date    08-April-2011
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and peripherals
 *          interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x_it.h"
extern uint32_t SysTick_Counter; // Declare the SysTick_Counter variable from Delay.c
uint8_t key_flag = 0;            // Declare a flag to indicate key press
#define DEBOUNCE_TIME 20         // 消抖时间 20ms
volatile uint32_t last_key_time = 0;
/** @addtogroup STM32F10x_StdPeriph_Examples
 * @{
 */

/** @addtogroup EXTI_Config
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1) {
    }
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1) {
    }
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1) {
    }
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1) {
    }
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void)
{
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void)
{
}

/**
 * @brief  This function handles PendSV_Handler exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void)
{
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */

void SysTick_Handler(void)
{
    SysTick_Counter++; // 每次1ms中断，计数器加1
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
 * @brief  This function handles External line 0 interrupt request.
 * @param  None
 * @retval None
 */
void EXTI3_IRQHandler(void)
{

    if (EXTI_GetITStatus(EXTI_Line3) != RESET) {
        uint32_t now = millis();
        if ((now - last_key_time) > DEBOUNCE_TIME) {
            last_key_time = now;
            key_flag      = 1;
            LED0_Toggle();
            printf("Key1 pressed!\r\n");
        }

        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}
void EXTI4_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line4) != RESET) {
        uint32_t now = millis();
        if ((now - last_key_time) > DEBOUNCE_TIME) {
            last_key_time = now;
        }
        EXTI_ClearITPendingBit(EXTI_Line4); // 清除中断挂起标志
    }
}

void USART3_IRQHandler(void)
{
    // Check if the RXNE (Receive Data Register Not Empty) interrupt is triggered
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
        uint16_t data = USART_ReceiveData(USART3); // Read received data
        while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET) {
        }
        USART_SendData(USART3, data);                   // Echo back the received data
        USART_ClearITPendingBit(USART3, USART_IT_RXNE); // Clear the RXNE interrupt pending bit
    }
}

/**
 * @brief  This function handles External lines 9 to 5 interrupt request.
 * @param  None
 * @retval None
 */
void EXTI9_5_IRQHandler(void)
{
#if defined(STM32F10X_HD_VL) || defined(STM32F10X_HD) || defined(STM32F10X_XL)
    if (EXTI_GetITStatus(EXTI_Line8) != RESET) {
    }
#else
    if (EXTI_GetITStatus(EXTI_Line9) != RESET) {
    }
#endif
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
 * @brief  This function handles PPP interrupt request.
 * @param  None
 * @retval None
 */
/*void PPP_IRQHandler(void)
{
}*/

/**
 * @}
 */

/**
 * @}
 */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
