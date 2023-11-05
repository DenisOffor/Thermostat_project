/*
 * PID_regulator.c
 *
 *  Created on: 27 окт. 2023 г.
 *      Author: denlo
 */


#include "PID_regulator.h"



Pid_Coef pid_coef;
uint8_t pid_state = PID_OFF;
int pwmDutyCycle = 500;
float errorPrevious = 0;
float errorCurrent = 0;
float errorIntegral = 0;
float errorDifferential = 0;

void TIM14_IRQHandler(void) {
	TIM14->SR &= ~TIM_SR_UIF;
	TIM14->CR1 &= ~TIM_CR1_CEN;
	pid_state = PID_ON;
}

void TIM3_IRQHandler(void) {
	TIM3->SR &= ~TIM_SR_UIF;
	TIM3->CCR1 = 1000 - (uint16_t)pwmDutyCycle;
}

void init_PID_regulation() {
	init_TIM3_for_PWM();
	init_tim14_as_delay_unit();
	set_Pid_Coef(300, 4, 10);
}

void init_TIM3_for_PWM() {
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER6_1;
	//GPIOC->MODER |= GPIO_OTYPER_OT_6;
	GPIOC->PUPDR |= GPIO_PUPDR_PUPDR6_1;

	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	TIM3->ARR = 1000;
	TIM3->PSC = 8*FREQ_MULTIPLIER_COEF;

	TIM3->CCMR1 |= TIM_CCMR1_OC1M;
	TIM3->CCER |= TIM_CCER_CC1E;
	TIM3->CCR1 = 500;

	TIM3->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM3_IRQn);
	NVIC_SetPriority(TIM3_IRQn,2);
}

void init_tim14_as_delay_unit() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;

	TIM14->ARR = 8000 * FREQ_MULTIPLIER_COEF;
	TIM14->PSC = 3000;

	TIM14->DIER |= TIM_DIER_UIE;

	NVIC_EnableIRQ(TIM14_IRQn);
	NVIC_SetPriority(TIM14_IRQn,5);

	TIM14->CR1 |= TIM_CR1_CEN;
}

void set_Pid_Coef(uint16_t Kp, uint16_t Ki, uint16_t Kd) {
	pid_coef.Kp = Kp;
	pid_coef.Ki = Ki;
	pid_coef.Kd = Kd;
}

void PID_reset() {
	TIM3->CNT = 0;
	TIM3->CR1 &= ~TIM_CR1_CEN;
	TIM14->CNT = 0;
	TIM14->CR1 &= ~TIM_CR1_CEN;
	pid_state = PID_OFF;
	set_Pid_Coef(300, 4, 10);
}
