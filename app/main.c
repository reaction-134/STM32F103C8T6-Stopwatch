/**
 * @file    main.c
 * @brief   四位数码管逐一点亮测试程序 (StdPeriph)
 *
 * 程序启动后进入主循环，顺序执行以下步骤并无限循环：
 *
 *   1. LED1 (PA0-PA6) 全部点亮 -> 保持 2 秒 -> 关闭
 *   2. LED8 (PA7-PA12, PA15) 全部点亮 -> 保持 2 秒 -> 关闭
 *   3. LED9 (PB1, PB3-PB8) 全部点亮 -> 保持 2 秒 -> 关闭
 *   4. LED10 (PB9, PB12-PB15, PC13-PC15) 全部点亮 -> 保持 2 秒 -> 关闭
 *
 * 硬件平台: STM32F103C8T6
 * 库:       STM32F10x Standard Peripheral Library (StdPeriph)
 * 编译器:   Keil MDK-ARM V5 / ARMCC 或 AC6
 */

#include "stm32f10x.h"
#include "gpio.h"
#include "delay.h"

/**
 * @brief  主函数
 * @note   初始化 GPIO 和 SysTick 延时后，进入无限循环。
 *         每次点亮一个数码管的所有段，保持 2 秒后关闭，
 *         依次切换为下一个数码管。
 */
int main(void)
{
    /* ---------- 初始化硬件 ---------- */

    /* 初始化所有数码管驱动 GPIO（推挽输出 50MHz） */
    GPIO_Init_ALL();

    /* 初始化 SysTick 定时器（1ms 中断），用于 Delay_ms() */
    Delay_Init();

    /* ---------- 主循环：逐一点亮数码管 ---------- */

    while (1)
    {
        /* ======== 第一步：LED1 全亮 2 秒 ======== */
        /* PA0=1, PA1=1, PA2=1, PA3=1, PA4=1, PA5=1, PA6=1 */
        LED1_ON();
        Delay_ms(2000);
        /* PA0~PA6 全部置 0 */
        LED1_OFF();

        /* ======== 第二步：LED8 全亮 2 秒 ======== */
        /* PA7=1, PA8=1, PA9=1, PA10=1, PA11=1, PA12=1, PA15=1 */
        LED8_ON();
        Delay_ms(2000);
        LED8_OFF();

        /* ======== 第三步：LED9 全亮 2 秒 ======== */
        /* PB1=1, PB3=1, PB4=1, PB5=1, PB6=1, PB7=1, PB8=1 */
        LED9_ON();
        Delay_ms(2000);
        LED9_OFF();

        /* ======== 第四步：LED10 全亮 2 秒 ======== */
        /* PB9=1, PB12=1, PB13=1, PB14=1, PB15=1,
         * PC13=1, PC14=1, PC15=1 */
        LED10_ON();
        Delay_ms(2000);
        LED10_OFF();
    }
}
