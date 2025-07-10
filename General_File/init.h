/*
 * init.h
 *
 *  Created on: 2025Äê7ÔÂ1ÈÕ
 *      Author: Rock
 */

#ifndef GENERAL_FILE_INIT_H_
#define GENERAL_FILE_INIT_H_

#include "debug.h"

void RCC_Configuration(void);
void GPIO_Init_Single(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, GPIOSpeed_TypeDef GPIO_Speed);
void USART_Init_Single(USART_TypeDef* USARTx, uint32_t BaudRate, GPIO_TypeDef* GPIO_TxPort, uint16_t GPIO_TxPin, GPIO_TypeDef* GPIO_RxPort, uint16_t GPIO_RxPin);
void I2C_Init_Single(I2C_TypeDef* I2Cx, uint32_t ClockSpeed, GPIO_TypeDef* GPIO_SCLPort, uint16_t GPIO_SCLPin, GPIO_TypeDef* GPIO_SDAPort, uint16_t GPIO_SDAPin);
void TIM_Base_Init(TIM_TypeDef* TIMx, uint16_t psc, uint32_t arr);

#endif /* GENERAL_FILE_INIT_H_ */
