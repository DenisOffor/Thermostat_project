/*
 * init_TEC_throght_relay.h
 *
 *  Created on: 8 окт. 2023 г.
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
	PROCESS,
	WAIT_TEMPERATURE_SET,
} REGULATE_STATUS;
REGULATE_STATUS regulate_status;

typedef struct Constants_Relay {
	float room_temperature;
	float maintenance_coef;
	float heat_coef;
	float heat_for_1sec;
	float delta;
}Constants_Relay;
Constants_Relay constants_relay;

void Constants_Relay_set(double, double, double, double, double);

void TIM16_IRQHandler(void);
void TIM6_DAC_IRQHandler(void);

void Relay_regulating();
void init_GPIO_for_relay();
void init_periphery_relay_regulating();
void Relay_start();
void Relay_off();
void relay_on();
void relay_off();
void Relay_reset();

void init_TIM6_for_Regulate_Time();
void TIM6_set_heat_time(double seconds);

void init_TIM16_for_wait_temp_set();
void TIM16_set_wait_time(double seconds);

#endif /* INIT_TEC_THROGHT_RELAY_H_ */
