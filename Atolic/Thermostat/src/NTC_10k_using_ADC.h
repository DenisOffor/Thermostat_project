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

uint16_t ADC_value;
float Ntc_Tmp;
uint16_t Ntc_R;
uint8_t ADC_HAVE_DATA;

#define NTC_UP_R 10000.0f
#define A 0.001111f
#define B 0.000237987f
#define C 0.000000065f

void DMA1_Channel1_IRQHandler(void);
void NTC_measure_temperature();
void NTC_init_periphery();
double NTC_get_temperature();
void ADC_init();
void DMA_for_ADC_init();


#endif /* NTC_10K_USING_ADC_H_ */
