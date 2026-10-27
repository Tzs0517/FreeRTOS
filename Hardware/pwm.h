#ifndef __pwm_h
#define __pwm_h

#include "stm32f4xx.h"

void RCC_Configuration(void);
void PWM_GPIO_Configuration(void);
void TIM3_PWM_Configuration(void);
void Motor_SetPulse(uint16_t pulse_us);

#endif
