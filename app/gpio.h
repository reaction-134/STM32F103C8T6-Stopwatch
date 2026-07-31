/**
 * @file    gpio.h
 * @brief   GPIO 初始化与数码管显示函数声明
 *
 * 4 个独立共阴极七段数码管（LED1 / LED8 / LED9 / LED10），
 * 每个数码管由 7 个独立 GPIO 脚推挽驱动，高电平经 S8050 三极管点亮。
 *
 * GPIO 分配：
 *   LED1:  PA0~PA6  (A-G)
 *   LED8:  PA7~PA12, PA15  (A-G)
 *   LED9:  PB1, PB3~PB8  (A-G)
 *   LED10: PB9, PB12~PB15, PC13~PC15  (A-G + DP)
 */

#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f10x.h"

/* GPIO 初始化 */
void GPIO_Init_ALL(void);

/* 七段数码管数字显示函数 (num: 0~9) */
void LED1_Display(uint8_t num);   /* PA0~PA6 */
void LED8_Display(uint8_t num);   /* PA7~PA12, PA15 */
void LED9_Display(uint8_t num);   /* PB1, PB3~PB8 */
void LED10_Display(uint8_t num);  /* PB9, PB12~PB15, PC13~PC15, DP常亮 */

#endif /* __GPIO_H */
