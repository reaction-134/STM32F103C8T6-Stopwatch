/**
 * @file    main.c
 * @brief   高精度电子秒表 — TIM2 100us 中断，四位数码管显示
 *
 * 显示布局：
 *   LED10 . LED9  LED8  LED1
 *    秒      100ms  10ms  1ms
 *
 * 显示计算（基于 total_ms）：
 *   LED1:  (total_ms / 1)   % 10  — 毫秒个位，每 1ms   变化
 *   LED8:  (total_ms / 10)  % 10  — 十毫秒位，每 10ms  变化
 *   LED9:  (total_ms / 100) % 10  — 百毫秒位，每 100ms 变化
 *   LED10: (total_ms / 1000)% 10  — 秒个位，  每 1s    变化
 *
 * LED10 DP (PC15) 常亮。
 */

#include "stm32f10x.h"
#include "gpio.h"
#include "timer.h"

/* 中断中更新的计时变量（定义在 stm32f10x_it.c） */
extern volatile uint32_t total_ms;

int main(void)
{
    uint32_t disp_total;

    /* 1. 初始化所有数码管 GPIO */
    GPIO_Init_ALL();

    /* 2. 初始化 TIM2（100us 中断） */
    TIM2_Init();

    /* 3. LED10 小数点 (PC15) 常亮 */
    GPIO_SetBits(GPIOC, GPIO_Pin_15);

    /* 4. 主循环 — 持续刷新四位数码管 */
    while (1)
    {
        /* 关中断，原子读取 total_ms（uint32_t 非原子） */
        __disable_irq();
        disp_total = total_ms;
        __enable_irq();

        /* 四位显示 */
        LED1_Display((disp_total / 1)   % 10);   /* 毫秒个位 */
        LED8_Display((disp_total / 10)  % 10);   /* 十毫秒位 */
        LED9_Display((disp_total / 100) % 10);   /* 百毫秒位 */
        LED10_Display((disp_total / 1000) % 10); /* 秒个位 */
        /* LED10 DP 已在 LED10_Display 中常亮 */
    }
}
