/*
 * NTC_10k_using_ADC.c
 *
 *  Created on: 19 окт. 2023 г.
 *      Author: denlo
 */


#include "NTC_10k_using_ADC.h"

uint8_t cycle_start = 0;

void DMA1_Channel1_IRQHandler(void) {
	DMA1->IFCR |= DMA_IFCR_CTCIF1;
	cycle_start = 1;
}

void NTC_measurment() {
	if(cycle_start == 1){
		Ntc_R = ((NTC_UP_R)/((4095.0/ADC_Raw[0]) - 1));
		double Ntc_Ln = log(Ntc_R);
		Ntc_Tmp = (1.0/(A + B*Ntc_Ln + C*Ntc_Ln*Ntc_Ln*Ntc_Ln)) - 273.15;
		display_temperature(Ntc_Tmp, NTC_TEMP);
		cycle_start = 0;
	}
}


void ADC_init() {
	//RCC on for GPIOA
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	//analog func
	GPIOA->MODER |= GPIO_MODER_MODER0;

	//turn on RCC
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	//turn on 14 MHz osc
	RCC->CR2 |= RCC_CR2_HSI14ON;
	//wait while is starting
	while((RCC->CR2 & RCC_CR2_HSI14RDY) != RCC_CR2_HSI14RDY);

	//calibration
	ADC1->CR |= ADC_CR_ADCAL;
	//wait until calibration is done
	while(ADC1->CR & ADC_CR_ADCAL);

	//239.5 cycles
	ADC1->SMPR |= ADC_SMPR_SMP;
	//continuous mode
	//PA0 select
	ADC1->CHSELR |= ADC_CHSELR_CHSEL0;

	ADC1->CFGR1 |= ADC_CFGR1_DMACFG | ADC_CFGR1_CONT;
	//DMA en
	ADC1->CFGR1 |= ADC_CFGR1_DMAEN | ADC_CFGR1_OVRMOD;

	DMA_for_ADC_init();

	//ADC enable
	ADC1->CR |= ADC_CR_ADEN;
	//wait until ADC is ready
	while( (ADC1->ISR & ADC_ISR_ADRDY) != ADC_ISR_ADRDY);

	ADC1->CR |= ADC_CR_ADSTART;
}

void DMA_for_ADC_init() {
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;

	DMA1_Channel1->CCR |= DMA_CCR_CIRC | DMA_CCR_PSIZE_0 | DMA_CCR_MSIZE_0;
	DMA1_Channel1->CNDTR = 1;
	DMA1_Channel1->CMAR = (uint32_t)(&ADC_Raw[0]);
	DMA1_Channel1->CPAR = (uint32_t)(&ADC1->DR);

	DMA1_Channel1->CCR |= DMA_CCR_TCIE;
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	NVIC_SetPriority(DMA1_Channel1_IRQn, 5);

	DMA1_Channel1->CCR |= DMA_CCR_EN;

}
