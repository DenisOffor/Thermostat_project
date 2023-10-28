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

typedef enum PROGRAM_STATUS {
	STATUS_TURN_OFF,
	STATUS_START,
	STATUS_WAIT_ACTION,
	STATUS_HEATING,
	STATUS_MAINTENANCE
} PROGRAM_STATUS;

PROGRAM_STATUS program_status;

void PID_regulation();
void init_clock();
void DS18B20_measure_temperature();
void NTC_measure_temperature();
void init_periphery();
void check_UART_cmd();
void Relay_regulating();
void reset_all_var();


#endif /* MANAGE_PROGRAM_H_ */
