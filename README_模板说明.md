# f401 裸机模板（整理后）

> 原 `term1_code (2)` 工程整理而来。目标：作为 FreeRTOS 四关卡实验的干净地基。
> 原始工程完整备份：`..\term1_code_backup.zip`（含被删掉的一切，随时可回滚）。

## 现在打开就能用

1. 双击 `f401_combination.uvprojx`（Keil MDK，AC5 编译器）
2. `F7` 编译 → 烧录 → 现象：**LED(PA5) 每秒翻转 + USART1(9600) 每秒打印 `tick=N`**
3. 看到串口输出 = 工具链、时钟(84MHz)、GPIO、USART、SysTick 延时全部打通

## 目录结构（整理后）

```
User\        main.c(模板) / main_sensor_demo.c.bak(你原来的传感器+PPM+电机demo，不参与编译)
             stm32f4xx_it.c(异常入口) / stm32f4xx_conf.h(已精简)
Start\       启动文件 + CMSIS 头 + system_stm32f4xx.c
System\      delay.c(SysTick轮询延时，不占中断，FreeRTOS 接管 SysTick 后不冲突)
Hardware\    led / usart1 / i2c / gy86 / ppm / pwm / key 驱动（关卡实验直接用）
Library\     SPL 标准库，只保留 14 个模块：misc gpio rcc usart tim i2c exti
             syscfg dma spi pwr iwdg wwdg dbgmcu
DebugConfig\ JLinkSettings.ini
```

## 删掉了什么（都在备份 zip 里）

| 类别 | 内容 |
|---|---|
| 编译产物 | `Objects\`(30.9MB)、`Listings\`（重新编译自动再生） |
| 未引用残留 | `RTE\`(RTX 配置，工程没用)、`test\test.s`(汇编课作业，工程引用已从 uvprojx 移除) |
| 重复/被遮蔽 | `System\usart1.*`、`System\usart6.*`（include 路径里 `Hardware\` 优先，这份永远编不进去） |
| 半成品/草稿 | `Hardware\GY86-2.*`(空文件)、`I2C-2.*`、`I2C_sysnit.s`、`User\Clock.s`、`User\I2C_Init.s`、`User\main.s`、根目录 `change1`、根目录 `system_stm32f4xx.c` |
| 别人的 IDE 缓存 | `.uvguix.17558/.34874/.34874.bak/.taozh`、`JLinkLog.txt`、空的 `Hardware\usart6.*` |
| 用不到的库 | SPL 里 adc/can/cryp/dac/fmc/fsmc/hash/ltdc/qspi/rtc/sai/sdio 等 40+ 文件 |

## 接 FreeRTOS 关卡 1 之前要做的

1. 下载 `FreeRTOS-Kernel`（GitHub FreeRTOS 仓库，V10.x+ 均可），拷入 `FreeRTOS\` 目录：
   `include\ portable\ARM\ Cortex-M4F\ portable\MemMang\` + `FreeRTOSConfig.h`
2. Keil 里新建 Group 加入 `tasks.c queue.c list.c timers.c event_groups.c stream_buffer.c`、
   `port.c`、`heap_4.c`（关卡 4 再换 `xTaskCreateStatic` 时加 `heap_5` 或静态方案）
3. include 路径加 `.\FreeRTOS\include;.\FreeRTOS\portable\ARM\Cortex-M4F`
   （你这套是 **AC5 编译器**：建议直接下载 FreeRTOS **V10.4.6** 源码包，用其中
   `portable\ARM\Cortex-M4F` 的 port.c——V11+ 新内核配 AC5 会麻烦，AC6 才走 `portable\GCC\ARM_CM4F`）
4. 中断向量：把 `FreeRTOSConfig.h` 里 `vPortSetupTimerInterrupt` 相关配置好后，
   在 `stm32f4xx_it.c` 里删掉/改名 `SVC_Handler`、`PendSV_Handler`、`SysTick_Handler`
   三个函数（port.c 里有同名实现，否则链接冲突）——**这一步是新手第一大坑**
5. `configMAX_SYSCALL_INTERRUPT_PRIORITY` 记得设（如 5<<4），你 µC/OS-II 那套 BASEPRI 思想直接复用

## 编译宏（已确认，不用改）

`STM32F401xx, USE_STDPERIPH_DRIVER, HSE_VALUE=8000000`，C99 开启，AC5。
