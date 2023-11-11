/*
 * Step_Heat.c
 *
 *  Created on: 6 но€б. 2023 г.
 *      Author: denlo
 */


#include <Step_and_InTime_Heat.h>

StepHeatVariables Step_heat_var;
HeatInTimeVar Heat_in_time_var;

void Calc_Intermediate_temperature() {
	//each 1 minute do step
	if((TIM2->CNT % (Step_heat_var.tau_wait * 60) ) == 0) {
		float intermediate_temperature = Step_heat_var.prev_temperature  + Step_heat_var.step;
		Step_heat_var.intermediate_temperature = intermediate_temperature > Step_heat_var.aim_temperature ?
				Step_heat_var.aim_temperature : intermediate_temperature;
		TIM2->CNT++; // in order to this code not implement two times for one CNT
		Step_heat_var.prev_temperature = intermediate_temperature;
		errorIntegral = 0;
		errorPrevious = 0;
	}
}

void Calc_aim_sec_heat() {
	Heat_in_time_var.aim_sec_heat = (Heat_in_time_var.aim_temperature - temperatures.curr_temperature) / (60 * Heat_in_time_var.tau_heat);
}

void Calc_Begin_DutyCycle() {
	pwmDutyCycle =  (Heat_in_time_var.aim_sec_heat / 0.4) * 1000;
	TIM3->CCR1 = 1000 - (uint16_t)pwmDutyCycle;
	TIM3->CR1 |= TIM_CR1_CEN;
}

void Heat_InTime_reset() {
	TIM3->CR1 &= ~TIM_CR1_CEN;
	Heat_in_time_var.prev_temperature = 0;
}

void Regulate_Duty_for_Heat_InTime() {
	if(temperatures.curr_temperature - Heat_in_time_var.prev_temperature > Heat_in_time_var.aim_sec_heat)
		pwmDutyCycle -= 5;
	if(temperatures.curr_temperature - Heat_in_time_var.prev_temperature < Heat_in_time_var.aim_sec_heat)
		pwmDutyCycle += 5;

	Heat_in_time_var.prev_temperature = temperatures.curr_temperature;
}

void Init_TIM2_as_Sec_counter() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	//ITR1 -> slave of TIM15
	TIM2->SMCR |= TIM_SMCR_TS_0 | TIM_SMCR_SMS;

	TIM2->PSC = 1;
	TIM2->ARR = 60000;

	TIM2->CR1 |= TIM_CR1_CEN;
}
