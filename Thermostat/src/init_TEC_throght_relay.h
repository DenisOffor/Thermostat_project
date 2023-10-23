/*
 * init_TEC_throght_relay.h
 *
 *  Created on: 8 окт. 2023 г.
 *      Author: denlo
 */

#ifndef INIT_TEC_THROGHT_RELAY_H_
#define INIT_TEC_THROGHT_RELAY_H_

#define MAINTENANCE_COEF 0.006f
#define HEAT_COEF 0.006f
#define ROOM_TEMPERATURE 27

#include "constant.h"

void TIM16_IRQHandler(void);
void TIM6_DAC_IRQHandler(void);

void init_GPIO_for_relay();
void init_periphery_relay_regulating();
void Relay_regulating();
void relay_on();
void relay_off();

void init_TIM6_for_Regulate_Time();
void TIM6_set_heat_time(double seconds);

void init_TIM16_for_wait_temp_set();
void TIM16_IRQHandler(void);
void TIM16_set_wait_time(double seconds);

#endif /* INIT_TEC_THROGHT_RELAY_H_ */
