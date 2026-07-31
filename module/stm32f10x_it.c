/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

/* ========== 秒表计时变量（volatile，中断与主循环共享）========== */
volatile uint32_t tick_100us = 0;   /* 100us 累计，到 10 表示 1ms */
volatile uint32_t total_ms   = 0;   /* 毫秒累计，主循环据此计算显示 */

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

void NMI_Handler(void) {}
void HardFault_Handler(void) { while (1); }
void MemManage_Handler(void) { while (1); }
void BusFault_Handler(void)  { while (1); }
void UsageFault_Handler(void){ while (1); }
void SVC_Handler(void)       {}
void DebugMon_Handler(void)  {}
void PendSV_Handler(void)    {}

/* SysTick_Handler 定义在 delay.c */

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/******************************************************************************/

/**
 * @brief  TIM2 中断服务函数 - 每 100us 触发一次
 *
 *         1. 清除更新中断标志
 *         2. tick_100us++（100us 计数）
 *         3. 满 10 次（1ms）：tick_100us=0, total_ms++
 *
 *         中断中不做 GPIO 操作，仅更新计时变量。
 */
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

        tick_100us++;

        if (tick_100us >= 10)
        {
            tick_100us = 0;
            total_ms++;
        }
    }
}
