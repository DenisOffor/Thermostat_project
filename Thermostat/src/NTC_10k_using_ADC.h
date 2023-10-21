/*
 * NTC_10k_using_ADC.h
 *
 *  Created on: 19 окт. 2023 г.
 *      Author: denlo
 */

#ifndef NTC_10K_USING_ADC_H_
#define NTC_10K_USING_ADC_H_

#include "constant.h"
#include "math.h"
#include "TFT_display_temperature.h"

uint16_t ADC_Raw[1];
double Ntc_Tmp;
uint16_t Ntc_R;
uint8_t cycle_start;

#define NTC_UP_R 10000.0f
#define A 0.001111f
#define B 0.000237987f
#define C 0.000000065f

void DMA1_Channel1_IRQHandler(void);
void init_TIM15_as_TRGO();
void ADC_init();
void DMA_for_ADC_init();

#endif /* NTC_10K_USING_ADC_H_ */
