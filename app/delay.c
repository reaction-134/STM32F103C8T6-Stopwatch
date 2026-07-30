/**
 * @file    delay.c
 * @brief   SysTick-based millisecond delay implementation (StdPeriph)
 *
 * 利用 SysTick 定时器的中断功能，实现毫秒级阻塞延时。
 * SystemCoreClock 由 system_stm32f10x.c 自动定义：
 *   - 默认 HSI (8 MHz) 时 = 8000000
 *   - 若配置 PLL 至 72 MHz 则 = 72000000
 * 该值影响 SysTick 的重装载值，请根据实际系统时钟确认。
 */

#include "delay.h"

/* SysTick 递减计数器，由中断服务函数每个 1ms 减 1 */
static __IO uint32_t s_ulDelayCounter = 0;

/**
 * @brief  初始化 SysTick 定时器
 * @note   配置 SysTick 每 1ms 触发一次中断
 *         SysTick_Config() 会自动设置重装载值并启动定时器
 */
void Delay_Init(void)
{
    /* SysTick 重装载值 = SystemCoreClock / 1000
     * 例如：72 MHz -> 72000，8 MHz -> 8000
     * 如果配置失败（传入 0 或超出范围），进入死循环 */
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        /* SysTick 配置失败，死循环等待 */
        while (1);
    }

    /* SysTick 优先级设置为最低（NVIC 优先级分组已在 system_stm32f10x 中配置） */
    NVIC_SetPriority(SysTick_IRQn, 0x0F);
}

/**
 * @brief  毫秒级阻塞延时
 * @param  ms  延时毫秒数
 * @note   SysTick_Handler 每 1ms 将 s_ulDelayCounter 减 1，
 *         本函数忙等待直到计数器归零。
 */
void Delay_ms(uint32_t ms)
{
    /* 设置需要等待的毫秒数 */
    s_ulDelayCounter = ms;

    /* 忙等待，直到计数器减到 0 */
    while (s_ulDelayCounter != 0);
}

/**
 * @brief  SysTick 中断服务函数
 * @note   每 1ms 自动调用一次，递减延时计数器。
 *         函数名已在 startup 文件的异常向量表中注册。
 */
void SysTick_Handler(void)
{
    if (s_ulDelayCounter != 0)
    {
        s_ulDelayCounter--;
    }
}
