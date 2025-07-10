#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "debug.h"

void USARTx_SendByte(USART_TypeDef *USARTx, uint8_t data);
void USARTx_SendString(USART_TypeDef *USARTx, char *str);

void System_Init(void);
void System_Run(void);
void System_Loop(void);

#endif
