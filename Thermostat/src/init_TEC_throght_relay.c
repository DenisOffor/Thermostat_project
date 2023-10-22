/*
 * init_TEC_throght_relay.c
 *
 *  Created on: 8 ���. 2023 �.
 *      Author: denlo
 */

#include "init_TEC_throght_relay.h"

void TIM16_IRQHandler(void) {
	TIM16->SR &= ~TIM_SR_UIF;
	TIM16->CR1 &= ~TIM_CR1_CEN;
	if(regulate_status == WAIT_TEMPERATURE_SET) {
		if(temperatures.aim_temperature - temperatures.curr_temperature > 0.7) {
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
	if(temperatures.aim_temperature == 255)
		regulate_status = WAITING;
	if(regulate_status == PROCESS){
		regulate_status = WAIT_TEMPERATURE_SET;
		TIM16->CR1 |= TIM_CR1_CEN;
	}
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
			if(temperatures.curr_temperature < (temperatures.aim_temperature - (0.25-(temperatures.aim_temperature - 27)*0.008) )) {//0.25-(t-t)*0.008;
				TIM6->ARR = 1000 * 1;
				relay_on();
				TIM6->CR1 |= TIM_CR1_CEN;
				regulate_status = PROCESS;
			}
			break;
		case HEATING:
			time_heat = (temperatures.aim_temperature - temperatures.curr_temperature) / (0.65 - (temperatures.aim_temperature - 27)*0.004); //0.65-(t-t)*0.004

			TIM6->ARR = 1000 * time_heat;
			relay_on();
			TIM6->CR1 |= TIM_CR1_CEN;
			regulate_status = PROCESS;

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
	init_TIM16_for_wait_temp_set();
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

void init_TIM16_for_wait_temp_set() {
	RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;

	//10 sec
	TIM16->PSC = 8000 * FREQ_MULTIPLIER_COEF;
	TIM16->ARR = 9000;


	TIM16->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM16_IRQn);
	NVIC_SetPriority(TIM16_IRQn, 1);

	TIM16->CR1 |= TIM_CR1_CEN;
}
