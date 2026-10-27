#ifndef __ppm_h
#define __ppm_h

#include "stm32f4xx.h"                  // Device header

// PPM相关变量


void PPM_GPIO_Configuration(void);
void TIM2_PPM_Configuration(void);
void TIM2_IRQHandler(void);
uint16_t PPM_GetChannel(uint8_t channel);
uint8_t PPM_IsFrameReady(void);

extern volatile uint32_t ppm_channel_values[8];
extern volatile uint32_t ppm_last_capture;
extern volatile uint8_t ppm_channel_index;
extern volatile uint8_t ppm_frame_ready;

#endif
