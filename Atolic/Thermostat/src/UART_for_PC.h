/*
 * UART_for_PC.h
 *
 *  Created on: 13 ���. 2023 �.
 *      Author: denlo
 */

#ifndef UART_FOR_PC_H_
#define UART_FOR_PC_H_

#include "constant.h"

#define UART_DATA_WAITING 0
#define UART_CMD_RECEIVED 1
#define UART_DATA_IN_BUF 2

#define START_BYTE 0x01
#define END_BYTE 0x09
#define DS18B20_ADDRESS 0x43
#define NTC_ADDRESS 0x44
#define PWM_ADDRESS 0x45

#define UART_CMD_TURN_OFF 0x10
#define UART_CMD_TURN_ON 0x11
#define UART_CMD_SET_AIM_TEMP 0x25
#define UART_CMD_HEAT_DURING 0x31
#define UART_CMD_SET_PID_COEF 0x41
#define UART_DRAW_GRAPH_ON_DISPLAY 0x43


uint8_t rx_data_state;
uint8_t rx_received_cmd;
uint16_t size_of_parcel;

uint8_t UART_tx_buf[24];
uint8_t UART_data_buf[1000];

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
