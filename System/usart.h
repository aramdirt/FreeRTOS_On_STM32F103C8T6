#ifndef __USARTX_H
#define __USARTX_H
#include "stm32f10x.h"  

void USART_User_Init(uint32_t BaudRate);
void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch);
void Usart_SendString(USART_TypeDef *pUSARTx, char *str);

#endif
