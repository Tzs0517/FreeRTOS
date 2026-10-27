#include "pwm.h"

void RCC_Configuration(void)
{
    // 开启HSE（外部高速时钟）
    RCC_HSEConfig(RCC_HSE_ON);
    
    // 等待HSE稳定
    while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
    
    // 配置PLL，目标频率84MHz
    // 根据您的实际晶振频率调整这些参数
    RCC_PLLConfig(RCC_PLLSource_HSE, 8, 168, 2, 7); // 8MHz晶振时
    RCC_PLLCmd(ENABLE);
    
    // 等待PLL就绪
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    
    // 设置系统时钟源为PLL
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    
    // 开启GPIOA和TIM1时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
}

void PWM_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 配置PA6为复用功能（TIM3_CH1）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        // 复用模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  // 高速
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      // 推挽输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        // 上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 将PA6连接到TIM3
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
}

void TIM3_PWM_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    // 时基配置
    // 定时器时钟 = 84MHz，PWM频率 = 84MHz / (Prescaler + 1) / (Period + 1)
    TIM_TimeBaseStructure.TIM_Prescaler = 83;        // 预分频，84MHz/84 = 1MHz
    TIM_TimeBaseStructure.TIM_Period = 19999;         // 自动重装载值，1MHz/20000 = 50Hz PWM频率
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    // PWM输出配置
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;      // PWM模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  // 输出使能
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; // 互补输出禁用
    TIM_OCInitStructure.TIM_Pulse = 0;                    // 初始占空比
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 输出极性
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
    
    // 配置通道3
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    
    // 使能TIM1预装载寄存器
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    
    // 使能TIM1主输出
		//TIM_CtrlPWMOutputs(TIM3, ENABLE);
    
    // 启动TIM1
    TIM_Cmd(TIM3, ENABLE);
}

void Motor_SetPulse(uint16_t pulse_us)
{
    // duty_cycle范围：0-1999，对应0-100%占空比
    if(pulse_us > 1999) {
        pulse_us = 1999;
    }
    TIM_SetCompare1(TIM3, pulse_us);
}
