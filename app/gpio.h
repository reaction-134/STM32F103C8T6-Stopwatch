/**
 * @file    gpio.h
 * @brief   GPIO 初始化与 4 个独立数码管控制函数声明 (StdPeriph)
 *
 * 4 个独立共阴极七段数码管 (LED1 / LED8 / LED9 / LED10)，
 * 每个数码管由独立 7 (或 8) 个 GPIO 引脚推挽驱动。
 * 高电平经过 NPN 三极管驱动段点亮。
 *
 * GPIO 连接分配：
 *   LED1: PA0~PA6  (A-G)
 *   LED8: PA7~PA12, PA15  (A-G)
 *   LED9: PB1, PB3~PB8  (A-G)
 *   LED10: PB9, PB12~PB15, PC13~PC15  (A-G + DP)
 */

#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f10x.h"

/* ---------- GPIO 初始化（使能时钟、配置推挽输出）---------- */
void GPIO_Init_ALL(void);

/* ---------- LED1 控制 (PA0-PA6) ---------- */
void LED1_ON(void);     /* PA0~PA6 全部置 1 */
void LED1_OFF(void);    /* PA0~PA6 全部置 0 */

/* ---------- LED8 控制 (PA7-PA12, PA15) ---------- */
void LED8_ON(void);     /* PA7~PA12, PA15 全部置 1 */
void LED8_OFF(void);    /* PA7~PA12, PA15 全部置 0 */

/* ---------- LED9 控制 (PB1, PB3-PB8) ---------- */
void LED9_ON(void);     /* PB1, PB3~PB8 全部置 1 */
void LED9_OFF(void);    /* PB1, PB3~PB8 全部置 0 */

/* ---------- LED10 控制 (PB9, PB12-PB15, PC13-PC15) ---------- */
void LED10_ON(void);    /* PB9, PB12~PB15, PC13~PC15 全部置 1 */
void LED10_OFF(void);   /* PB9, PB12~PB15, PC13~PC15 全部置 0 */

#endif /* __GPIO_H */
