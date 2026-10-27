//usart1.h
#ifndef __USART1_H
#define __USART1_H

#include "stm32f4xx.h"

void USART1_Init(uint32_t baudrate);
void USART1_SendChar(uint8_t ch);
void USART1_SendString(char *str);
void USART1_SendNumber(int32_t num); 


#endif
