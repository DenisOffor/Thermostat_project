/*
 * constant.h
 *
 *  Created on: 30 сент. 2023 г.
 *      Author: denlo
 */

#ifndef CONSTANT_H_
#define CONSTANT_H_

#include "stm32f0xx.h"
#include "FLASH.h"

#define FREQ_MULTIPLIER_COEF 5
#define RESET_TEMPERATURE 255

#define DISPLAY_TEMPERATURE 0
#define DISPLAY_GRAPH 1
uint8_t display_status;

typedef struct Temperatures {
	float curr_temperature;
	float cur_temperature_DS;
	float cur_temperature_NTC;
	float cur_temperature_AHT;
	int aim_temperature;
} Temperatures;
Temperatures temperatures;

typedef struct Sensors_State{
	uint8_t main_sensor;          //  Main sensor:
	uint8_t DS_as_add_sensor;	  // 1 - DS,
	uint8_t NTC_as_add_sensor;	  // 2 - NTC,
	uint8_t AHT_as_add_sensor;    // 3 - AHT
} Sensors_State;                  // In rest of uint8_t stored state of sensors (turn on or turn off)
Sensors_State sensors_state;

typedef enum PROGRAM_STATUS {
	STATUS_TURN_OFF,
	STATUS_START,
	STATUS_WAIT_ACTION,
	STATUS_HEATING,
	STATUS_HEATING_DURING_TIME,
	STATUS_MAINTENANCE
} PROGRAM_STATUS;
PROGRAM_STATUS program_status;

typedef enum RegulateMode {
	FREE_CONTROL,
	RELAY,
	PID
} RegulateMode;
RegulateMode regulate_mode;

void Reset_temperatures();

#endif /* CONSTANT_H_ */
