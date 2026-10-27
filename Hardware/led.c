//led.c
#include "led.h"
#include "delay.h"

/* 记录 LED 当前状态，0=灭  1=亮 */
static uint8_t led_state = 0;

/**
 * @brief  初始化 LED 引脚
 */
void LED_Init(void)
{
    GPIO_InitTypeDef gpio;

    /* 1. 打开时钟 */
    RCC_AHB1PeriphClockCmd(LED_CLK, ENABLE);

	/* ===== 校验：如果时钟没开，下面会卡死 ===== */
    while (!(RCC->AHB1ENR & LED_CLK));   // 等 bit0 变 1
	
    /* 2. 配置引脚：推挽输出 */
    gpio.GPIO_Pin   = LED_PIN;
    gpio.GPIO_Mode  = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(LED_PORT, &gpio);

    /* 3. 关灯 */
    LED_Off();
}

/**
 * @brief  点亮 LED
 */
void LED_On(void)
{
    GPIO_SetBits(LED_PORT, LED_PIN);
    led_state = 1;
}

/**
 * @brief  熄灭 LED
 */
void LED_Off(void)
{
    GPIO_ResetBits(LED_PORT, LED_PIN);
    led_state = 0;
}

/**
 * @brief  翻转 LED 状态
 */
void LED_Toggle(void)
{
    if (led_state == 0){LED_On();}
    else			   {LED_Off();}
}

void LED_Blink_1Hz(void)
{
    LED_On();               // 点亮 500 ms
    Delay_ms(500);
    LED_Off();              // 熄灭 500 ms
    Delay_ms(500);
}
