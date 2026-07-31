/**
 * @file    timer.h
 * @brief   TIM2 初始化声明 - 100us 中断周期
 *
 *   PSC = 71  -> 计数时钟 = 1 MHz
 *   ARR = 99  -> 更新周期 = 100 us
 */

#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h"

void TIM2_Init(void);

#endif /* __TIMER_H */
