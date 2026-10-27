#include "ppm.h"

volatile uint32_t ppm_last_capture = 0;
volatile uint32_t ppm_channel_values[8] = {0};
volatile uint8_t ppm_channel_index = 0;
volatile uint8_t ppm_frame_ready = 0;

void PPM_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
        
    // 配置PA1为输入模式（TIM2_CH2，用于PPM信号输入）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        // 复用模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        // 上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 将PA0连接到TIM2
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
}

//PPM信号捕获定时器配置
void TIM2_PPM_Configuration(void)
{
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 时基配置
    // 定时器时钟 = 84MHz，计数频率 = 84MHz / 84 = 1MHz (1us分辨率)
    TIM_TimeBaseStructure.TIM_Prescaler = 83;           // 84分频
    TIM_TimeBaseStructure.TIM_Period = 65535;           // 最大计数值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    // 输入捕获配置
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;  // 上升沿捕获
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;        // 无分频
    TIM_ICInitStructure.TIM_ICFilter = 0x0;              // 无滤波
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
    
    // 使能捕获/比较1中断
    TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
    
    // 配置NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 启动TIM2
    TIM_Cmd(TIM2, ENABLE);
}

//中断服务函数
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
    {
        uint32_t current_capture = TIM_GetCapture2(TIM2);
        
        // 计算脉冲宽度
        uint32_t pulse_width = 0;
        if (current_capture > ppm_last_capture)
        {
            pulse_width = current_capture - ppm_last_capture;
        }
        else
        {
            pulse_width = (65535 - ppm_last_capture) + current_capture;
        }
        
        // 检测同步脉冲（通常 > 2ms）
        if (pulse_width > 2000)  // 2ms阈值
        {
            // 同步脉冲，开始新帧
            ppm_channel_index = 0;
            ppm_frame_ready = 1;
        }
        else if (pulse_width > 800 && pulse_width < 2200)  // 有效通道脉冲范围
        {
            // 存储通道值
            if (ppm_channel_index < 8)
            {
                ppm_channel_values[ppm_channel_index] = pulse_width;
                ppm_channel_index++;
            }
        }
        
        ppm_last_capture = current_capture;
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
    }
}

uint16_t PPM_GetChannel(uint8_t channel)
{
    if (channel < 8)
    {
        return ppm_channel_values[channel];
    }
    return 0;
}

uint8_t PPM_IsFrameReady(void)
{
    if (ppm_frame_ready)
    {
        ppm_frame_ready = 0;
        return 1;
    }
    return 0;
}
