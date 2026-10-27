/*============================================================
 * main.c —— F401 裸机模板（FreeRTOS 关卡 1 之前的地基验证）
 * 现象：
 *   1. LED (PA5) 每 1s 翻转一次（1Hz 方波）
 *   2. USART1 (9600-8N1) 每秒打印 tick 计数
 * 说明：
 *   - 时钟链：startup -> SystemInit -> RCC_Configuration()(pwm.c 内, 8M HSE -> 84M)
 *   - delay.c 用 SysTick 轮询实现，不开 SysTick 中断，
 *     后面移植 FreeRTOS 时 SysTick 会被内核接管，互不冲突
 *============================================================*/
#include "stm32f4xx.h"
#include "pwm.h"      /* RCC_Configuration */
#include "led.h"
#include "usart1.h"
#include "delay.h"

int main(void)
{
    uint32_t tick = 0;

    RCC_Configuration();   /* 先配时钟：84MHz */
    LED_Init();
    USART1_Init(9600);     /* 与旧工程一致；想快点可改 115200 */
    Delay_Init();          /* SysTick 轮询延时，之后 Delay_ms 可用 */

    USART1_SendString("=== F401 bare-metal template OK ===\r\n");
    USART1_SendString("LED(PA5) 1Hz blink, tick print @1s\r\n");

    while (1)
    {
        LED_Toggle();
        tick++;
        USART1_SendString("tick=");
        USART1_SendNumber((int32_t)tick);
        USART1_SendString("\r\n");
        Delay_ms(1000);
    }
}
