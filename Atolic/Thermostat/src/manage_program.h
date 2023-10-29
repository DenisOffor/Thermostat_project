/*
 * manage_program.h
 *
 *  Created on: 18 окт. 2023 г.
 *      Author: denlo
 */

#ifndef MANAGE_PROGRAM_H_
#define MANAGE_PROGRAM_H_

#include "TFT_display_temperature.h"
#include "ds18b20.h"
#include "init_TEC_throght_relay.h"
#include "UART_for_PC.h"
#include "NTC_10k_using_ADC.h"
#include "PID_regulator.h"

#define DISPLAY_TEMPERATURE 0
#define DISPLAY_GRAPH 1
uint8_t display_status;

typedef enum PROGRAM_STATUS {
	STATUS_TURN_OFF,
	STATUS_START,
	STATUS_WAIT_ACTION,
	STATUS_HEATING,
	STATUS_MAINTENANCE
} PROGRAM_STATUS;
PROGRAM_STATUS program_status;

typedef struct Sensors_State{
	uint8_t main_sensor;          //  Main sensor:
	uint8_t DS_as_add_sensor;	  // 1 - DS,
	uint8_t NTC_as_add_sensor;	  // 2 - NTC,
	uint8_t AHT_as_add_sensor;    // 3 - AHT
} Sensors_State;                  // In rest of uint8_t stored state of sensors (turn on or turn off)
Sensors_State sensors_state;

void Clear_sensors_state();
void Set_sensors_state(uint8_t main_sensor, uint8_t DS_as_add_sensor, uint8_t NTC_as_add_sensor, uint8_t AHT_as_add_sensor);

void init_periphery();
void init_clock();
void check_UART_cmd();
void Measure_temperature();
void Display_data();
void DS18B20_measure_temperature();
void NTC_measure_temperature();
void Relay_regulating();
void PID_regulation();

void reset_all_var();


#endif /* MANAGE_PROGRAM_H_ */
