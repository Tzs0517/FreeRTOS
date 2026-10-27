#include "stm32f4xx.h"                  // F401设备头文件
#include "delay.h"

void Key_Init(void)
{
    // 1. 使能GPIOB时钟（F401的GPIO挂在AHB1总线，需用AHB1使能函数）
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    
    // 2. 配置GPIO参数（上拉输入模式）
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;  // 保持PB0、PB1不变
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;            // F4的输入模式统一为GPIO_Mode_IN
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            // F4单独配置上拉（对应F1的GPIO_Mode_IPU）
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t Key_GetNum0(void)
{
    uint8_t KeyNum = 0;
    
    // 检测PB0是否被按下（上拉输入，按下为低电平）
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
    {
        Delay_ms(20);                  // 消抖
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0);  // 等待按键松开
        Delay_ms(20);                  // 消抖
        KeyNum = 1;
    }
    return KeyNum;
}

uint8_t Key_GetNum1(void)
{
    uint8_t KeyNum = 0;
    
    // 检测PB1是否被按下（上拉输入，按下为低电平）
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
    {
        Delay_ms(20);                  // 消抖
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);  // 等待按键松开
        Delay_ms(20);                  // 消抖
        KeyNum = 1;
    }
    return KeyNum;
}
