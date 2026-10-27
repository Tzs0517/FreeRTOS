//led.h
#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"
#include "delay.h"          // 引用延时

/* ========= 用户可改：LED 接在哪个引脚 ========= */
#define LED_PORT     GPIOA
#define LED_PIN      GPIO_Pin_5
#define LED_CLK      RCC_AHB1Periph_GPIOA
/* ========================================== */

void LED_Init(void);
void LED_On(void);
void LED_Off(void);
void LED_Toggle(void);
void LED_Blink_1Hz(void);   // 每秒闪一次，阻塞版

#endif
