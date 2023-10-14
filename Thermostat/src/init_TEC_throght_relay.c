/*
 * init_TEC_throght_relay.c
 *
 *  Created on: 8 окт. 2023 г.
 *      Author: denlo
 */

#include "init_TEC_throght_relay.h"

//void TIM6_DAC_IRQHandler(void) {
//	TIM6->SR &= ~TIM_SR_UIF;
//	TIM6->CR1 &= ~TIM_CR1_CEN;
//	//if the timer has counted up to 25ms then disable rattle check
//	check = 1;
//}
//
//void EXTI0_1_IRQHandler(void) {
//	if( (EXTI->PR & EXTI_PR_PIF0) == EXTI_PR_PIF0) {
//		EXTI->PR |= EXTI_PR_PIF0;
//		if(check == 1) {
//			check = 0;
//			GPIOC->ODR ^= GPIO_ODR_8;
//			GPIOB->ODR ^= GPIO_ODR_7;
//			TIM6->CR1 |= TIM_CR1_CEN;
//		}
//	}
//}

void init_GPIO() {
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;
	GPIOB->MODER |= GPIO_MODER_MODER7_0;
	GPIOB->OTYPER |= GPIO_OTYPER_OT_7;
	GPIOC->MODER |= GPIO_MODER_MODER8_0;
	GPIOB->ODR &= ~GPIO_ODR_7;
}

void Relay_regulating() {
	if(temperature < 33)
		GPIOB->ODR |= GPIO_ODR_7;
	if(temperature > 33)
		GPIOB->ODR &= ~GPIO_ODR_7;
}

//void init_GPIO_for_Button() {
//	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN;
//	//input mode on PA0
//	GPIOA->MODER &= ~GPIO_MODER_MODER0;
//	GPIOC->MODER |= GPIO_MODER_MODER8_0;
//	GPIOC->ODR &= ~GPIO_ODR_8;
//	GPIOC->PUPDR |= GPIO_PUPDR_PUPDR8_1;
//
//	EXTI->IMR |= EXTI_IMR_IM0;	 //interrupt mask register
//	EXTI->RTSR |= EXTI_RTSR_RT0; //rising trigger selection
//
//	NVIC_EnableIRQ(EXTI0_1_IRQn);//tune NVIC for EXTI0
//	NVIC_SetPriority(EXTI0_1_IRQn, 5);
//}
//
//void init_TIM6_for_rattle_eliminating () {
//	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
//	TIM6->ARR = 4000; //tune TIM6 for 25ms
//	TIM6->PSC = 250;
//	//interrupt on
//	TIM6->DIER |= TIM_DIER_UIE;
//	NVIC_EnableIRQ(TIM6_DAC_IRQn);
//	NVIC_SetPriority(TIM6_DAC_IRQn, 5);
//	TIM6->CR1 |= TIM_CR1_CEN;
//}
