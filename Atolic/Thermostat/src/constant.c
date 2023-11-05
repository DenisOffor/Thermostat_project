/*
 * constant.c
 *
 *  Created on: 30 сент. 2023 г.
 *      Author: denlo
 */

#include "constant.h"

Temperatures temperatures;
Sensors_State sensors_state;
PROGRAM_STATUS program_status = STATUS_TURN_OFF;
uint8_t display_status = DISPLAY_TEMPERATURE;

void Reset_temperatures() {
	temperatures.curr_temperature = temperatures.cur_temperature_DS
			= temperatures.cur_temperature_NTC = temperatures.cur_temperature_AHT = temperatures.aim_temperature = RESET_TEMPERATURE;
}
