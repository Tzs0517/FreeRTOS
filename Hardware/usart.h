//usart6.h
#ifndef __USART_H
#define __USART_H

#include "stm32f4xx.h"

void USART_Init(uint32_t baudrate);
void USART_SendChar(uint8_t ch);
void USART_SendString(char *str);

#endif
