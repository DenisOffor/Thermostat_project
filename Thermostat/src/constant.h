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

typedef enum PROGRAM_STATUS {
	STATUS_TURN_OFF,
	STATUS_START,
	STATUS_WAIT_ACTION,
	STATUS_HEATING,
	STATUS_MAINTENANCE
} PROGRAM_STATUS;

typedef enum DS18B20_CMD{
	TEMPERATURE_CONVERTING,
	WAITING_1SEC,
	TEMPERATURE_READING,
	TEMPERATURE_DISPLAYING,
} DS18B20_CMD;

typedef enum REGULATE_STATUS{
	WAITING,
	MAINTENANCE,
	HEATING,
	HEATING_DURING_TIME,
	PROCESS,
	WAIT_TEMPERATURE_SET,
} REGULATE_STATUS;

Temperatures temperatures;
DS18B20_CMD ds18b20_cmd;
REGULATE_STATUS regulate_status;
PROGRAM_STATUS program_status;

#endif /* CONSTANT_H_ */
