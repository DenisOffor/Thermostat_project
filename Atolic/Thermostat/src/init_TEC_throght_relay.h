/*
 * init_TEC_throght_relay.h
 *
 *  Created on: 8 ���. 2023 �.
 *      Author: denlo
 */

#ifndef INIT_TEC_THROGHT_RELAY_H_
#define INIT_TEC_THROGHT_RELAY_H_

#include "constant.h"

//#define MAINTENANCE_COEF 0.007f
//#define HEAT_COEF 0.006f
//#define ROOM_TEMPERATURE 27

typedef enum REGULATE_STATUS{
	WAITING,
	MAINTENANCE,
	HEATING,
	HEATING_DURING_TIME,
	PROCESS,
	WAIT_TEMPERATURE_SET,
} REGULATE_STATUS;
REGULATE_STATUS regulate_status;

typedef struct Constants_Relay {
	double room_temperature;
	double maintenance_coef;
	double heat_coef;
	double heat_for_1sec;
	double delta;
}Constants_Relay;

Constants_Relay constants_relay;
void Constatns_Relay_clear();
void Constants_Relay_set(double, double, double, double, double);

void TIM16_IRQHandler(void);
void TIM6_DAC_IRQHandler(void);

void init_GPIO_for_relay();
void init_periphery_relay_regulating();
void relay_on();
void relay_off();

void init_TIM6_for_Regulate_Time();
void TIM6_set_heat_time(double seconds);

void init_TIM16_for_wait_temp_set();
void TIM16_IRQHandler(void);
void TIM16_set_wait_time(double seconds);

#endif /* INIT_TEC_THROGHT_RELAY_H_ */