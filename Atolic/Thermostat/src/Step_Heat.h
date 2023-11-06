/*
 * Step_Heat.h
 *
 *  Created on: 6 но€б. 2023 г.
 *      Author: denlo
 */

#ifndef STEP_HEAT_H_
#define STEP_HEAT_H_

#include "constant.h"

typedef struct StepHeatVar{
	uint8_t aim_temperature;
	float intermediate_temperature;
	uint8_t step;
} StepHeatVariables;
StepHeatVariables Step_heat_var;

void Calc_Intermediate_temperature();
void Init_TIM2_as_Sec_counter();

#endif /* STEP_HEAT_H_ */
