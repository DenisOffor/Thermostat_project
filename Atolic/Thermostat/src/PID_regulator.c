/*
 * PID_regulator.c
 *
 *  Created on: 27 окт. 2023 г.
 *      Author: denlo
 */


#include "PID_regulator.h"

void init_TIM3_for_PWM() {
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER6_1;

	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	TIM3->ARR = 1000;
	TIM3->PSC = 8*FREQ_MULTIPLIER_COEF;

	TIM3->CCMR1 |= TIM_CCMR1_OC1M;
	TIM3->CCER |= TIM_CCER_CC1E;
	TIM3->CCR1 = 500;

	//	TIM3->DIER |= TIM_DIER_UIE;
	//	NVIC_EnableIRQ(TIM3_IRQn);
	//	NVIC_SetPriority(TIM3_IRQn,2);

	TIM3->CR1 |= TIM_CR1_CEN;
}
