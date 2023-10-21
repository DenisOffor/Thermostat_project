/*
 * UART_for_PC.h
 *
 *  Created on: 13 окт. 2023 г.
 *      Author: denlo
 */

#ifndef UART_FOR_PC_H_
#define UART_FOR_PC_H_

#include "constant.h"

#define DATA_WAITING 0
#define DATA_IN_BUF 1

#define START_BYTE 0x01
#define END_BYTE 0x09
#define DS18B20_ADDRESS 0x43
#define NTC_ADDRESS 0x44


#define UART_CMD_TURN_OFF 0x10
#define UART_CMD_TURN_ON 0x11
#define UART_CMD_SET_AIM_TEMP 0x25
#define UART_CMD_HEAT_DURING 0x31

uint8_t rx_data_state;

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
void UART_send_temperature(uint8_t* char_output, uint8_t char__output_size, uint8_t address);


#endif /* UART_FOR_PC_H_ */
