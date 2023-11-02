/*
 * UART_for_PC.h
 *
 *  Created on: 13 окт. 2023 г.
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
#define SEND_TEMPERATURE 0x42
#define PWM_ADDRESS 0x45

#define UART_CMD_TURN_ON_OFF 0x10
#define UART_CMD_SET_AIM_TEMP 0x25
#define UART_CMD_HEAT_DURING 0x31
#define CMD_SENSOR_CHOOSE 0x32
#define UART_CMD_SET_RELAY_COEF 0x40
#define UART_CMD_SET_PID_COEF 0x41
#define UART_GET_GRAPH_ON_DISPLAY 0x43
#define CMD_DRAW_CHOOSE 0x44

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
void UART_send_temperature(uint8_t* char_output, uint8_t char_output_size, uint8_t address);


#endif /* UART_FOR_PC_H_ */
