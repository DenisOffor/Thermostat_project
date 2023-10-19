/*
 * init_TEC_throght_relay.h
 *
 *  Created on: 8 ���. 2023 �.
 *      Author: denlo
 */

#ifndef INIT_TEC_THROGHT_RELAY_H_
#define INIT_TEC_THROGHT_RELAY_H_

#define FACTOR_UNDER_30 0.3
#define FACTOR_UNDER_40 0.25
#define FACTOR_ABOVE_40 0.2

#include "constant.h"

void TIM6_DAC_IRQHandler(void);
void init_GPIO();
void init_TIM6_for_Regulate_Time();
void Relay_regulating();
void relay_on();
void relay_off();

#endif /* INIT_TEC_THROGHT_RELAY_H_ */
