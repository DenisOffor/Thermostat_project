/*
 * init_TEC_throght_relay.c
 *
 *  Created on: 8 ���. 2023 �.
 *      Author: denlo
 */

#include "init_TEC_throght_relay.h"

Constants_Relay constants_relay;
REGULATE_STATUS regulate_status = WAITING;

void TIM16_IRQHandler(void) {
	TIM16->SR &= ~TIM_SR_UIF;
	TIM16->CR1 &= ~TIM_CR1_CEN;
	if(regulate_status == WAIT_TEMPERATURE_SET) {
		if(temperatures.aim_temperature - temperatures.curr_temperature > 1) {
			regulate_status = HEATING;
			return;
		}
		regulate_status = MAINTENANCE;
	}
}

void TIM6_DAC_IRQHandler(void) {
	TIM6->SR &= ~TIM_SR_UIF;
	TIM6->CR1 &= ~TIM_CR1_CEN;
	relay_off();
	//if it was heat during time -> just off relay and wait for new action
	if(program_status == STATUS_HEATING_DURING_TIME)
		program_status = STATUS_WAIT_ACTION;

	//if it was heat using relay -> set unit of delay and wait
	if(regulate_status == PROCESS){
		regulate_status = WAIT_TEMPERATURE_SET;
		TIM16->CR1 |= TIM_CR1_CEN;
	}
}

void Relay_regulating() {
	if(program_status == STATUS_TURN_OFF)
		return;

	double time_heat = 0;

	switch(regulate_status) {
		case WAITING:
			relay_off();
			break;
		case MAINTENANCE:
			if(temperatures.curr_temperature < (temperatures.aim_temperature -
						(constants_relay.delta - (temperatures.aim_temperature - constants_relay.room_temperature)*constants_relay.maintenance_coef) )) {

				TIM6->ARR = 1000 * (1 + (temperatures.aim_temperature - constants_relay.room_temperature)*5*constants_relay.maintenance_coef);
				relay_on();
				TIM6->CR1 |= TIM_CR1_CEN;
				regulate_status = PROCESS;
			}
			break;
		case HEATING:
			time_heat = (temperatures.aim_temperature - temperatures.curr_temperature)
					/ (constants_relay.heat_for_1sec - (temperatures.aim_temperature - constants_relay.room_temperature)*constants_relay.heat_coef);

			TIM6_set_heat_time(time_heat);
			relay_on();
			TIM6->CR1 |= TIM_CR1_CEN;
			regulate_status = PROCESS;
			break;

		default:
			break;
	}
}

void Relay_start() {
	regulate_status = HEATING;
}

void Relay_off() {
	relay_off();
	regulate_status = WAITING;
	TIM6->CNT = 0;
	TIM6->CR1 &= ~TIM_CR1_CEN;
	TIM16->CNT = 0;
	TIM16->CR1 &= ~TIM_CR1_CEN;
}

void relay_on() {
	GPIOB->ODR |= GPIO_ODR_7;
}

void relay_off() {
	GPIOB->ODR &= ~GPIO_ODR_7;
}

void init_periphery_relay_regulating() {
	init_GPIO_for_relay();
	init_TIM6_for_Regulate_Time();
	init_TIM16_for_wait_temp_set();
	Constants_Relay_set(27,0.006,0.006,0.65,0.25);
}

void init_GPIO_for_relay() {
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	GPIOB->MODER |= GPIO_MODER_MODER7_0;
	GPIOB->ODR &= ~GPIO_ODR_7;
}

void Relay_reset() {
	Constants_Relay_set(27,0.006,0.006,0.65,0.25);
	regulate_status = WAITING;
	TIM6->CNT = 0;
	TIM6->CR1 &= ~TIM_CR1_CEN;
	TIM16->CNT = 0;
	TIM16->CR1 &= ~TIM_CR1_CEN;
}

void init_TIM6_for_Regulate_Time() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

	TIM6->PSC = 8000 * FREQ_MULTIPLIER_COEF;
	TIM6->ARR = 1000;

	TIM6->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM6_DAC_IRQn);
	NVIC_SetPriority(TIM6_DAC_IRQn, 1);

	TIM6->CR1 |= TIM_CR1_CEN;
}

void TIM6_set_heat_time(double seconds) {
	TIM6->ARR = 1000 * seconds;
}

void init_TIM16_for_wait_temp_set() {
	RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;

	//4.5 sec
	TIM16->PSC = 8000 * FREQ_MULTIPLIER_COEF;
	TIM16->ARR = 4500;

	TIM16->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM16_IRQn);
	NVIC_SetPriority(TIM16_IRQn, 1);

	TIM16->CR1 |= TIM_CR1_CEN;
}

void TIM16_set_wait_time(double seconds) {
	TIM16->ARR = 1000 * seconds;
}

void Constants_Relay_set(double room_temperature, double maintenance_coef, double heat_coef, double heat_for_1sec, double delta) {
	constants_relay.room_temperature = room_temperature;
	constants_relay.maintenance_coef = maintenance_coef;
	constants_relay.heat_coef = heat_coef;
	constants_relay.heat_for_1sec = heat_for_1sec;
	constants_relay.delta = delta;
}
