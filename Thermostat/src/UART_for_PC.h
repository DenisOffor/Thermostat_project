/*
 * UART_for_PC.h
 *
 *  Created on: 13 окт. 2023 г.
 *      Author: denlo
 */

#ifndef UART_FOR_PC_H_
#define UART_FOR_PC_H_

#include "constant.h"

#define USART USART1
#define PORT_USART GPIOA
#define RCC_GPIO_USART RCC_AHBENR_GPIOAEN
#define RCC_USART RCC_APB2ENR_USART1EN
#define GPIO_USART_TX GPIO_MODER_MODER9_1
#define GPIO_USART_RX GPIO_MODER_MODER10_1


void DMA1_Channel4_5_IRQHandler(void);
void init_GPIO_for_USART();
void init_USART();
void init_DMA_for_USART();

#endif /* UART_FOR_PC_H_ */
