/**
 * @file    gpio.c
 * @brief   GPIO 初始化与七段数码管显示函数实现
 *
 * 七段码 bit 映射（以 LED1 为例）：
 *   bit0(PA0)=A, bit1(PA1)=B, bit2(PA2)=C,
 *   bit3(PA3)=D, bit4(PA4)=E, bit5(PA5)=F, bit6(PA6)=G
 *
 *    AAA
 *   F   B
 *    GGG
 *   E   C
 *    DDD   DP
 */

#include "gpio.h"

/* ===================================================================
 * 七段数码管字形码（共阴极，1=段亮）
 * ===================================================================*/
static const uint8_t SEG_CODE[10] =
{
    0x3F,  /* 0: abcdef   */
    0x06,  /* 1: bc       */
    0x5B,  /* 2: abdeg    */
    0x4F,  /* 3: abcdg    */
    0x66,  /* 4: bcfg     */
    0x6D,  /* 5: acdfg    */
    0x7D,  /* 6: acdefg   */
    0x07,  /* 7: abc      */
    0x7F,  /* 8: abcdefg  */
    0x6F   /* 9: abcdfg   */
};

/* ===================================================================
 * GPIO 初始化
 * ===================================================================*/
void GPIO_Init_ALL(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能 GPIOA / GPIOB / GPIOC 时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC, ENABLE);

    /* 使能 AFIO，禁用 JTAG 以释放 PA15 / PB3 / PB4 作为 GPIO */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    /* 推挽输出 50MHz */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /* GPIOA: PA0~PA12, PA15 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3  |
                                  GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_6  | GPIO_Pin_7  |
                                  GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11 |
                                  GPIO_Pin_12 | GPIO_Pin_15;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* GPIOB: PB1, PB3~PB9, PB12~PB15 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1  | GPIO_Pin_3  | GPIO_Pin_4  | GPIO_Pin_5  |
                                  GPIO_Pin_6  | GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9  |
                                  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* GPIOC: PC13~PC15 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/* ===================================================================
 * LED1 显示 (PA0~PA6)
 *   PA0=A  PA1=B  PA2=C  PA3=D  PA4=E  PA5=F  PA6=G
 * ===================================================================*/
void LED1_Display(uint8_t num)
{
    uint8_t  code;
    uint16_t set_mask = 0;
    uint16_t rst_mask = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
                        GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;

    code = SEG_CODE[num & 0x0F];

    if (code & 0x01) set_mask |= GPIO_Pin_0;   /* A */
    if (code & 0x02) set_mask |= GPIO_Pin_1;   /* B */
    if (code & 0x04) set_mask |= GPIO_Pin_2;   /* C */
    if (code & 0x08) set_mask |= GPIO_Pin_3;   /* D */
    if (code & 0x10) set_mask |= GPIO_Pin_4;   /* E */
    if (code & 0x20) set_mask |= GPIO_Pin_5;   /* F */
    if (code & 0x40) set_mask |= GPIO_Pin_6;   /* G */

    GPIO_ResetBits(GPIOA, rst_mask);
    GPIO_SetBits(GPIOA, set_mask);
}

/* ===================================================================
 * LED8 显示 (PA7~PA12, PA15)
 *   PA7=A  PA8=B  PA9=C  PA10=D  PA11=E  PA12=F  PA15=G
 * ===================================================================*/
void LED8_Display(uint8_t num)
{
    uint8_t  code;
    uint16_t set_mask = 0;
    uint16_t rst_mask = GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 |
                        GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15;

    code = SEG_CODE[num & 0x0F];

    if (code & 0x01) set_mask |= GPIO_Pin_7;   /* A */
    if (code & 0x02) set_mask |= GPIO_Pin_8;   /* B */
    if (code & 0x04) set_mask |= GPIO_Pin_9;   /* C */
    if (code & 0x08) set_mask |= GPIO_Pin_10;  /* D */
    if (code & 0x10) set_mask |= GPIO_Pin_11;  /* E */
    if (code & 0x20) set_mask |= GPIO_Pin_12;  /* F */
    if (code & 0x40) set_mask |= GPIO_Pin_15;  /* G */

    GPIO_ResetBits(GPIOA, rst_mask);
    GPIO_SetBits(GPIOA, set_mask);
}

/* ===================================================================
 * LED9 显示 (PB1, PB3~PB8)
 *   PB1=A  PB3=B  PB4=C  PB5=D  PB6=E  PB7=F  PB8=G
 * ===================================================================*/
void LED9_Display(uint8_t num)
{
    uint8_t  code;
    uint16_t set_mask = 0;
    uint16_t rst_mask = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |
                        GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;

    code = SEG_CODE[num & 0x0F];

    if (code & 0x01) set_mask |= GPIO_Pin_1;   /* A */
    if (code & 0x02) set_mask |= GPIO_Pin_3;   /* B */
    if (code & 0x04) set_mask |= GPIO_Pin_4;   /* C */
    if (code & 0x08) set_mask |= GPIO_Pin_5;   /* D */
    if (code & 0x10) set_mask |= GPIO_Pin_6;   /* E */
    if (code & 0x20) set_mask |= GPIO_Pin_7;   /* F */
    if (code & 0x40) set_mask |= GPIO_Pin_8;   /* G */

    GPIO_ResetBits(GPIOB, rst_mask);
    GPIO_SetBits(GPIOB, set_mask);
}

/* ===================================================================
 * LED10 显示 (PB9, PB12~PB15, PC13~PC15)
 *   PB9=A  PB12=B  PB13=C  PB14=D  PB15=E
 *   PC13=F  PC14=G  PC15=DP（小数点常亮）
 *
 *   实际硬件修正：PCB 上 PC13 <-> PC14 接反，
 *   代码已对调：F 用 PC14，G 用 PC13
 * ===================================================================*/
void LED10_Display(uint8_t num)
{
    uint8_t  code;
    uint16_t set_mask_b = 0;
    uint16_t rst_mask_b = GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_13 |
                          GPIO_Pin_14 | GPIO_Pin_15;
    uint16_t set_mask_c = GPIO_Pin_15;   /* DP 常亮 */
    uint16_t rst_mask_c = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

    code = SEG_CODE[num & 0x0F];

    if (code & 0x01) set_mask_b |= GPIO_Pin_9;   /* A -> PB9  */
    if (code & 0x02) set_mask_c |= GPIO_Pin_13;  /* B -> PC13 */
    if (code & 0x04) set_mask_b |= GPIO_Pin_13;  /* C -> PB13 */
    if (code & 0x08) set_mask_b |= GPIO_Pin_14;  /* D -> PB14 */
    if (code & 0x10) set_mask_b |= GPIO_Pin_15;  /* E -> PB15 */
    if (code & 0x20) set_mask_c |= GPIO_Pin_14;  /* F -> PC14 */
    if (code & 0x40) set_mask_b |= GPIO_Pin_12;  /* G -> PB12 */

    GPIO_ResetBits(GPIOB, rst_mask_b);
    GPIO_ResetBits(GPIOC, rst_mask_c);
    GPIO_SetBits(GPIOB, set_mask_b);
    GPIO_SetBits(GPIOC, set_mask_c);
}
