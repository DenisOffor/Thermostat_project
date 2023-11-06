/*
 * Step_Heat.h
 *
 *  Created on: 6 но€б. 2023 г.
 *      Author: denlo
 */

#ifndef STEP_AND_INTIME_HEAT_H_
#define STEP_AND_INTIME_HEAT_H_

#include "constant.h"
#include "PID_regulator.h"

typedef struct StepHeatVar{
	uint8_t aim_temperature;
	float intermediate_temperature;
	uint8_t tau_wait;
	uint8_t step;
} StepHeatVariables;
StepHeatVariables Step_heat_var;

typedef struct HeatInTimeVar{
	uint8_t aim_temperature;
	uint8_t tau_heat;
	float aim_sec_heat;
	float prev_temperature;
} HeatInTimeVar;
HeatInTimeVar Heat_in_time_var;


void Calc_Intermediate_temperature();
void Calc_aim_sec_heat();
void Calc_Begin_DutyCycle();

void Regulate_Duty_for_Heat_InTime();
void Init_TIM2_as_Sec_counter();

#endif /* STEP_AND_INTIME_HEAT_H_ */
