#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include "debug.h"

extern volatile uint8_t usart2_cmd_received;
extern volatile uint8_t rec_finish;

void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void UART4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void Delay_NonBlocking(uint64_t delay_us);
uint64_t get_sys_time(void);

#endif
