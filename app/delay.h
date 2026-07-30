/**
 * @file    delay.h
 * @brief   SysTick-based millisecond delay for STM32F103 (StdPeriph)
 *
 * 使用 SysTick 定时器实现毫秒级阻塞延时。
 * 需要在 main 初始化时调用 Delay_Init()，然后调用 Delay_ms()。
 */

#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

/* 初始化 SysTick 定时器（1ms 中断） */
void Delay_Init(void);

/* 毫秒级阻塞延时 */
void Delay_ms(uint32_t ms);

/* SysTick 中断服务函数（由 startup 文件的向量表调用） */
void SysTick_Handler(void);

#endif /* __DELAY_H */
