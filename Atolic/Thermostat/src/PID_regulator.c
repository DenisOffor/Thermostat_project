/*
 * PID_regulator.c
 *
 *  Created on: 27 окт. 2023 г.
 *      Author: denlo
 */


#include "PID_regulator.h"



Pid_Coef pid_coef;
int pwmDutyCycle = 500;
float errorPrevious = 0;
float errorCurrent = 0;
float errorIntegral = 0;
float errorDifferential = 0;

void TIM3_IRQHandler(void) {
	TIM3->SR &= ~TIM_SR_UIF;
	TIM3->CCR1 = (uint16_t)pwmDutyCycle;
}

void init_TIM3_for_PWM() {
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER6_1;

	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	TIM3->ARR = 1000;
	TIM3->PSC = 8*FREQ_MULTIPLIER_COEF;

	TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	TIM3->CCER |= TIM_CCER_CC1E;
	TIM3->CCR1 = 500;

	TIM3->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM3_IRQn);
	NVIC_SetPriority(TIM3_IRQn,2);
}

void clear_Pid_Coef() {
	pid_coef.Kp = pid_coef.Ki = pid_coef.Kd = 0;
}

void set_Pid_Coef(uint16_t Kp, uint16_t Ki, uint16_t Kd) {
	pid_coef.Kp = Kp;
	pid_coef.Ki = Ki;
	pid_coef.Kd = Kd;
}
