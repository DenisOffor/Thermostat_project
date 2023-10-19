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
#define RESET_TEMPERATURE 0xFF

//uint8_t mat_for_symbol1[1024];
//uint8_t mat_for_symbol2[1024];
//uint8_t mat_for_symbol3[1024];
//uint8_t mat_for_symbol4[256];

typedef struct Temperatures {
	double curr_temperature;
	int aim_temperature;
} Temperatures;

typedef enum PROGRAM_TASK{
	TURN_OFF = 0,
	START,
	TEMPERATURE_CONVERTING,
	TEMPERATURE_READING,
	TEMPERATURE_DISPLAYING,
} PROGRAM_TASK;

typedef enum REGULATE_STATUS{
	WAITING,
	HEATING,
	HEATING_DURING_TIME,
	COOLING,
	COOLING_DURING_TIME
} REGULATE_STATUS;

Temperatures temperatures;
PROGRAM_TASK program_task;
REGULATE_STATUS regulate_status;

uint16_t color_mat[1024];
uint8_t mat_for_symbol[FLASH_PAGE_SIZE];
uint8_t buf_tx[BUFFER_SIZE];
uint8_t UART_tx_buf[BUFFER_SIZE];
uint8_t UART_rx_buf[2];

#endif /* CONSTANT_H_ */
