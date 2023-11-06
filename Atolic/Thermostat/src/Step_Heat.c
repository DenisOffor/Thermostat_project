/*
 * Step_Heat.c
 *
 *  Created on: 6 но€б. 2023 г.
 *      Author: denlo
 */


#include "Step_Heat.h"

StepHeatVariables Step_heat_var;

void Calc_Intermediate_temperature() {
	//each 1 minute do step
	if((TIM2->CNT % 60) == 0) {
		float intermediate_temperature = temperatures.curr_temperature + Step_heat_var.step;
		Step_heat_var.intermediate_temperature = intermediate_temperature > Step_heat_var.aim_temperature ?
				Step_heat_var.aim_temperature : intermediate_temperature;
		TIM2->CNT++; // in order to this code not implement two times for one CNT
	}
}


void Init_TIM2_as_Sec_counter() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	//ITR1 -> slave of TIM15
	TIM2->SMCR |= TIM_SMCR_TS_0 | TIM_SMCR_SMS;

	TIM2->PSC = 1;
	TIM2->ARR = 60000;

	TIM2->CR1 |= TIM_CR1_CEN;
}
