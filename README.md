# STM32F401 FreeRTOS Lab

> 基于 STM32F401RE 的嵌入式实时系统学习工程：从裸机模板出发，逐关引入 FreeRTOS 内核，
> 每个阶段都带**可测量的指标**（中断延迟、周期抖动、栈水位、CPU 占用），而不只是"跑起来就行"。
>
> English: A bare-metal → FreeRTOS learning lab on STM32F401RE, driven by measurable numbers
> (interrupt latency, period jitter, stack watermark, CPU load) instead of "it blinks".

[![MCU](https://img.shields.io/badge/MCU-STM32F401RE-blue)]() [![Kernel](https://img.shields.io/badge/RTOS-FreeRTOS-green)]() [![Toolchain](https://img.shields.io/badge/IDE-Keil%20MDK--5-orange)]() [![Language](https://img.shields.io/badge/Language-C-lightgrey)]()

## ✨ 当前状态

- [x] 裸机模板：时钟树 / GPIO / USART / SysTick 轮询延时，编译 0 warning
- [ ] 关卡 1：FreeRTOS 跑通 + 任务模型（3 任务、不同优先级与栈）
- [ ] 关卡 2：队列 + 中断（ISR→任务延迟实测，µs 级）
- [ ] 关卡 3：互斥量 + 优先级反转（复现 → 继承缓解，记录前后时间差）
- [ ] 关卡 4：内存 / CPU / 可靠性（静态创建、栈水位、看门狗、tickless）
- [ ] 终极目标：将任务架构迁移至四轴飞控应用（见 [相关项目](#-相关项目)）

## 🎯 为什么做这个仓库

本科阶段做过一个完整的四轴飞行器项目（µC/OS-II 内核移植 + 姿态解算 + 双闭环 PID，
见个人主页）。这个仓库解决另一个问题：**用工程师的方式学 RTOS**——

每个关卡必须交付三样东西：

1. **一个能跑的 demo**（现象可复现）
2. **一组实测数据**（延迟 / 抖动 / 水位 / 占用，来自 DWT 周期计数器或 `uxTaskGetStackHighWaterMark` 等内核统计）
3. **一段"踩坑记录"**（现象 → 根因 → 修复，写在对应 issue 或本文档末尾）

## 🛠 环境与硬件

| 项 | 说明 |
|---|---|
| 开发板 | STM32F401RE（Cortex-M4F @ 84MHz，HSE 8MHz → PLL） |
| 编译器 | Keil MDK-ARM 5.24a（Arm Compiler 5，C99） |
| 库 | STM32F4 StdPeriph（精简至 14 个实际使用的模块） |
| 烧录 | ST-LINK / J-Link，`F7` 一键编译下载 |
| 串口 | USART1 PA9/PA10，9600-8N1（TODO: 换 115200） |
| 内核 | FreeRTOS V10.4.6（AC5 用 `portable/ARM/Cortex-M4F`） |

## 📁 工程结构

```
User/        main.c · stm32f4xx_it.c · stm32f4xx_conf.h
Start/       startup_stm32f401xx.s · system_stm32f4xx.c · CMSIS 头
System/      delay.c —— SysTick 轮询延时（不占用中断，与内核 SysTick 无冲突）
Hardware/    led · usart1 · i2c · gy86 · ppm · pwm · key 驱动
Library/     SPL 标准外设库（已裁剪）
FreeRTOS/    (TODO) include · portable/ARM/Cortex-M4F · heap_4
```

## 🚀 快速开始

```bash
git clone <repo-url>
# Keil 打开 f401_combination.uvprojx → F7 编译 → F8 下载
```

预期现象：

```
=== F401 bare-metal template OK ===
LED(PA5) 1Hz blink, tick print @1s
tick=1
tick=2
...
```

## 📏 实测数据

> 每过一关在这里追加一行，数据说话。

| 阶段 | 指标 | 实测值 | 测试方法 |
|---|---|---|---|
| 裸机 | 编译产物 | Code=2504 B, ZI=1060 B | Keil 构建日志 |
| 关卡 2 | 中断→任务唤醒延迟 | TODO µs | DWT CYCCNT |
| 关卡 3 | 反转缓解前后差 | TODO ms | 二值信号量 vs Mutex |
| 关卡 4 | 各任务栈余量 | TODO words | HighWaterMark |

## ⚠️ 踩坑记录

- **`stm32f4xx_it.c` 里的 `SVC/PendSV/SysTick_Handler` 必须在引入 port.c 前删除**，否则链接期重复定义——新手接 FreeRTOS 的第一号坑，本人亲踩。
- （TODO：随关卡追加）

## 🔗 相关项目

- **四轴飞行器飞控**（STM32F401 + µC/OS-II 手工移植 + Madgwick 姿态融合 + 双闭环 PID）：`<TODO 仓库地址>`

## 📄 License

MIT（TODO：加 LICENSE 文件）

---

<sub>作者：陶镇松 · 电子科技大学 软件工程（嵌入式方向） · 2131379472@qq.com</sub>
