/*
 * constant.h
 *
 *  Created on: 30 сент. 2023 г.
 *      Author: denlo
 */

#ifndef CONSTANT_H_
#define CONSTANT_H_

#include "stm32f0xx.h"
#include "buffer.h"
#include "FLASH.h"

#define FREQ_MULTIPLIER_COEF 5

//uint8_t mat_for_symbol1[1024];
//uint8_t mat_for_symbol2[1024];
//uint8_t mat_for_symbol3[1024];
//uint8_t mat_for_symbol4[256];

uint16_t color_mat[2048];
uint8_t mat_for_symbol[FLASH_PAGE_SIZE];
uint8_t buf_tx[BUFFER_SIZE];
uint8_t UART_tx_buf[BUFFER_SIZE];
uint8_t UART_rx_buf[2];

double temperature;
double temperature_aim;

typedef enum PROGRAM_TASK{
	WAITING = 0,
	TEMPERATURE_CONVERTING,
	TEMPERATURE_READING,
	TEMPERATURE_DISPLAYING
} PROGRAM_TASK;

PROGRAM_TASK program_task;


#endif /* CONSTANT_H_ */
