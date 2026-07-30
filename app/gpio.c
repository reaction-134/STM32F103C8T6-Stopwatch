/**
 * @file    gpio.c
 * @brief   GPIO 初始化与 4 个独立数码管控制函数实现 (StdPeriph)
 *
 * 硬件连接：
 *   - 每个数码管独立由 7 个 GPIO 引脚控制（LED10 额外包含 DP 小数点）
 *   - GPIO 输出高电平 -> NPN 三极管 -> LED 段点亮（共阴极）
 *   - 段码 a/b/c/d/e/f/g 对应标准七段数码管排列
 *
 * 特别注意：
 *   1) PA15 (JTDI)、PB3 (JTDO)、PB4 (NJTRST) 默认被 JTAG 占用。
 *      初始化时会调用 GPIO_PinRemapConfig() 禁用 JTAG，
 *      保留 SWD (PA13/SWDIO, PA14/SWCLK) 以便调试。
 *   2) PC13~PC15 属于备份域引脚，在 F103C8T6 中可正常作为 GPIO 输出，
 *      无需额外配置备份域时钟。
 */

#include "gpio.h"

/**
 * @brief  初始化所有用于驱动数码管的 GPIO 引脚
 * @note   该函数完成以下操作：
 *         1. 开启 GPIOA / GPIOB / GPIOC 时钟
 *         2. 开启 AFIO 时钟，禁用 JTAG 以释放 PA15/PB3/PB4
 *         3. 将所有相关引脚配置为推挽输出 (GPIO_Mode_Out_PP)，50 MHz
 */
void GPIO_Init_ALL(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ========== 1. 使能 GPIO 时钟 ========== */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC, ENABLE);

    /* ========== 2. AFIO 时钟 & JTAG 释放 ========== */
    /* 使能 AFIO 时钟，才能使用 GPIO_PinRemapConfig() */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    /* 禁用 JTAG (PA15/JTDI, PB3/JTDO, PB4/NJTRST)，
     * 保留 SWD (PA13/SWDIO, PA14/SWCLK) 用于调试 */
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    /* ========== 3. 配置 GPIO 模式 ========== */
    /* 所有引脚均为推挽输出，50 MHz 速度 */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /* ---------- GPIOA: PA0 ~ PA12, PA15 ---------- */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3  |
                                  GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_6  | GPIO_Pin_7  |
                                  GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11 |
                                  GPIO_Pin_12 | GPIO_Pin_15;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* ---------- GPIOB: PB1, PB3 ~ PB9, PB12 ~ PB15 ---------- */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1  | GPIO_Pin_3  | GPIO_Pin_4  | GPIO_Pin_5  |
                                  GPIO_Pin_6  | GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9  |
                                  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* ---------- GPIOC: PC13 ~ PC15 ---------- */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* 初始状态下，所有数码管段全部关闭 */
    LED1_OFF();
    LED8_OFF();
    LED9_OFF();
    LED10_OFF();
}

/* ===================================================================
 * LED1 控制 (PA0-PA6)
 *   PA0 -> A段    PA1 -> B段    PA2 -> C段
 *   PA3 -> D段    PA4 -> E段    PA5 -> F段
 *   PA6 -> G段
 * ===================================================================*/

/**
 * @brief  LED1 所有段同时点亮
 * @note   PA0~PA6 输出高电平，经 NPN 三极管驱动共阴极数码管段
 */
void LED1_ON(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
                        GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
}

/**
 * @brief  LED1 所有段同时关闭
 * @note   PA0~PA6 输出低电平
 */
void LED1_OFF(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
                          GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
}

/* ===================================================================
 * LED8 控制 (PA7-PA12, PA15)
 *   PA7  -> A段    PA8  -> B段    PA9  -> C段
 *   PA10 -> D段    PA11 -> E段    PA12 -> F段
 *   PA15 -> G段
 * ===================================================================*/

/**
 * @brief  LED8 所有段同时点亮
 * @note   PA7~PA12, PA15 输出高电平
 */
void LED8_ON(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 |
                        GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15);
}

/**
 * @brief  LED8 所有段同时关闭
 */
void LED8_OFF(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 |
                          GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15);
}

/* ===================================================================
 * LED9 控制 (PB1, PB3-PB8)
 *   PB1 -> A段    PB3 -> B段    PB4 -> C段
 *   PB5 -> D段    PB6 -> E段    PB7 -> F段
 *   PB8 -> G段
 * ===================================================================*/

/**
 * @brief  LED9 所有段同时点亮
 * @note   PB1, PB3~PB8 输出高电平
 */
void LED9_ON(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |
                        GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8);
}

/**
 * @brief  LED9 所有段同时关闭
 */
void LED9_OFF(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |
                          GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8);
}

/* ===================================================================
 * LED10 控制 (PB9, PB12-PB15, PC13-PC15)
 *   PB9  -> A段    PB12 -> B段    PB13 -> C段
 *   PB14 -> D段    PB15 -> E段    PC13 -> F段
 *   PC14 -> G段    PC15 -> DP（小数点）
 * ===================================================================*/

/**
 * @brief  LED10 所有段 + 小数点同时点亮
 * @note   PB9, PB12~PB15, PC13~PC15 输出高电平
 */
void LED10_ON(void)
{
    /* 端口 B: PB9, PB12 ~ PB15 */
    GPIO_SetBits(GPIOB, GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_13 |
                        GPIO_Pin_14 | GPIO_Pin_15);
    /* 端口 C: PC13(F段), PC14(G段), PC15(DP) */
    GPIO_SetBits(GPIOC, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
}

/**
 * @brief  LED10 所有段 + 小数点同时关闭
 */
void LED10_OFF(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_13 |
                          GPIO_Pin_14 | GPIO_Pin_15);
    GPIO_ResetBits(GPIOC, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
}
