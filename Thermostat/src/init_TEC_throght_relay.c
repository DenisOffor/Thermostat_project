/*
 * init_TEC_throght_relay.c
 *
 *  Created on: 8 окт. 2023 г.
 *      Author: denlo
 */

#include "init_TEC_throght_relay.h"

void TIM6_DAC_IRQHandler(void) {
	TIM6->SR &= ~TIM_SR_UIF;
	TIM6->CR1 &= ~TIM_CR1_CEN;
	relay_off();
	if(temperatures.aim_temperature == RESET_TEMPERATURE)
		regulate_status = WAITING;
	else
		regulate_status = MAINTENANCE;
}

void Relay_regulating() {
	if(program_task == TURN_OFF)
		return;

	float time_heat = 0;

	switch(regulate_status) {
		case WAITING:
			relay_off();
			break;
		case MAINTENANCE:
			if(temperatures.curr_temperature < (temperatures.aim_temperature - 0.3)) {
				TIM6->ARR = 1000 * 2;
				relay_on();
				TIM6->CR1 |= TIM_CR1_CEN;
			}
			break;
		case HEATING:
			if(temperatures.curr_temperature < 30)
				time_heat += (30 - temperatures.curr_temperature) / FACTOR_UNDER_30;

			if(temperatures.curr_temperature > 30 && temperatures.aim_temperature > 40)
				time_heat += (40 - temperatures.curr_temperature) / FACTOR_UNDER_40;

			if(temperatures.curr_temperature < 30 && temperatures.aim_temperature > 40)
				time_heat += (40 - 30) / FACTOR_UNDER_40;

			if(temperatures.curr_temperature > 30 && temperatures.aim_temperature < 40)
				time_heat += (temperatures.aim_temperature - temperatures.curr_temperature) / FACTOR_UNDER_40;

			if(temperatures.curr_temperature < 30 && temperatures.aim_temperature < 40)
						time_heat += (temperatures.aim_temperature - 30) / FACTOR_UNDER_40;

			if(temperatures.aim_temperature > 40)
				time_heat += (temperatures.aim_temperature - 40) / FACTOR_ABOVE_40;

			TIM6->ARR = 1000 * time_heat;
			relay_on();
			TIM6->CR1 |= TIM_CR1_CEN;

			break;
		case HEATING_DURING_TIME:
			TIM6->CR1 |= TIM_CR1_CEN;
			relay_on();
			break;
		case COOLING:
			break;
		case COOLING_DURING_TIME:
			break;
	}
}

void relay_on() {
	GPIOB->ODR |= GPIO_ODR_7;
}

void relay_off() {
	GPIOB->ODR &= ~GPIO_ODR_7;
}

void init_GPIO() {
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	GPIOB->MODER |= GPIO_MODER_MODER7_0;
	//GPIOB->OTYPER |= GPIO_OTYPER_OT_7;
	//GPIOB->PUPDR |= GPIO_PUPDR_PUPDR7_0;
	GPIOB->ODR &= ~GPIO_ODR_7;

	init_TIM6_for_Regulate_Time();
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
