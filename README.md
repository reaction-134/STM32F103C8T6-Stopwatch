# STM32F103C8T6 高精度电子秒表

基于 STM32F103C8T6 与标准外设库（StdPeriph）实现的高精度电子秒表，使用 4 个独立共阴极七段数码管显示「秒.毫秒」格式，时间基准由 TIM2 定时器提供 100us 中断。

## 功能特性

- TIM2 定时器 **100us** 更新中断，每秒 10000 次中断
- 时间基准：`tick_100us` → `total_ms` 两级累加
- 显示格式：**秒.毫秒**（如 12.345s）
- 4 个数码管独立 GPIO 驱动，无动态扫描、无位选
- LED10 小数点常亮
- 中断内仅更新计时变量，显示刷新在主循环完成

## 显示布局

```
LED10 . LED9  LED8  LED1
 秒      100ms  10ms  1ms
```

| 数码管 | 显示内容 | 变化周期 |
|--------|---------|---------|
| LED10 | 秒个位 `(total_ms/1000)%10` | 1s |
| LED9  | 百毫秒位 `(total_ms/100)%10` | 100ms |
| LED8  | 十毫秒位 `(total_ms/10)%10` | 10ms |
| LED1  | 毫秒个位 `(total_ms/1)%10` | 1ms |

## 硬件连接

4 个独立共阴极七段数码管，GPIO 输出高电平经 S8050 三极管驱动点亮。

### LED1（PA0~PA6）

| 段 | A | B | C | D | E | F | G |
|----|---|---|---|---|---|---|---|
| GPIO | PA0 | PA1 | PA2 | PA3 | PA4 | PA5 | PA6 |

### LED8（PA7~PA12, PA15）

| 段 | A | B | C | D | E | F | G |
|----|---|---|---|---|---|---|---|
| GPIO | PA7 | PA8 | PA9 | PA10 | PA11 | PA12 | PA15 |

### LED9（PB1, PB3~PB8）

| 段 | A | B | C | D | E | F | G |
|----|---|---|---|---|---|---|---|
| GPIO | PB1 | PB3 | PB4 | PB5 | PB6 | PB7 | PB8 |

### LED10（PB9, PB12~PB15, PC13~PC15）

| 段 | A | B | C | D | E | F | G | DP |
|----|---|---|---|---|---|---|---|---|
| GPIO | PB9 | PC13 | PB13 | PB14 | PB15 | PC14 | PB12 | PC15 |

> 注意：LED10 引脚映射经硬件实测修正，与原理图原始标注不同（B→PC13、F→PC14、G→PB12）。

## 工程结构

```
STM32F103C8T6_ProjectTemplate/
├── app/
│   ├── main.c          # 主循环：原子读取 total_ms，刷新四位数码管
│   ├── gpio.c          # GPIO 初始化 + 七段显示函数
│   ├── gpio.h
│   ├── timer.c         # TIM2 100us 中断配置（PSC=71, ARR=99）
│   ├── timer.h
│   ├── delay.c         # SysTick 延时（备用）
│   └── delay.h
├── module/
│   ├── stm32f10x_it.c  # TIM2_IRQHandler + 计时变量
│   └── stm32f10x_conf.h
├── board/              # 开发板初始化
├── bsp/                # 外设驱动
├── libraries/          # 标准外设库 + CMSIS
├── project/MDK(V5)/    # Keil MDK 工程
├── 开发任务书.md        # 项目需求文档
└── 开发报告.md          # 开发总结文档
```

## 开发环境

- **IDE**: Keil MDK-ARM V5
- **库**: STM32F10x Standard Peripheral Library (StdPeriph)
- **编译器**: ARMCC (V5.06)
- **禁止**: HAL 库、CubeMX 生成代码、动态扫描

## TIM2 配置

| 参数 | 值 | 计算 |
|------|----|------|
| TIM2 时钟 | 72 MHz | APB1(36MHz) × 2 |
| 预分频 PSC | 71 | 72MHz / 72 = 1MHz |
| 自动重装 ARR | 99 | (99+1) × 1us = 100us |
| 中断频率 | 10 kHz | 每秒 10000 次 |

## 使用说明

1. 用 Keil MDK 打开 `project/MDK(V5)/Project.uvprojx`
2. 编译并下载到 STM32F103C8T6
3. 上电后秒表自动开始计时

## 文档

- [开发任务书](开发任务书.md)
- [开发报告](开发报告.md)