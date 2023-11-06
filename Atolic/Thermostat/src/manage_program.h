/*
 * manage_program.h
 *
 *  Created on: 18 окт. 2023 г.
 *      Author: denlo
 */



/*
 TIM1 used for us DS18B20
 TIM17 used for delay 1sec for DS18B20 measurement
 TIM6 used as heat time for relay and free control
 TIM16 used as unit of delay for relay
 TIM15 used as TRGO for analog sensor and time 1 sec for data transfer
 TIM3 used as PWM for PID regulation
 TIM14 used as delay unit for PID regulation

 TIM2 slave of TIM15 for second count
 */


#ifndef MANAGE_PROGRAM_H_
#define MANAGE_PROGRAM_H_

#include <Step_and_InTime_Heat.h>
#include "TFT_display_temperature.h"
#include "ds18b20.h"
#include "NTC_10k_using_ADC.h"
#include "AHT20.h"
#include "init_TEC_throght_relay.h"
#include "UART_for_PC.h"
#include "PID_regulator.h"
#include "Step_and_InTime_Heat.h"
#include "string.h"

uint8_t Second_pass_switcherFirst;
uint8_t Second_pass_switcherSecond;

void Clear_sensors_state();
void Set_sensors_state(uint8_t main_sensor, uint8_t DS_as_add_sensor, uint8_t NTC_as_add_sensor, uint8_t AHT_as_add_sensor);

void init_periphery();
void init_clock();
void check_UART_cmd();
void Measure_temperature();
void Display_data();
void TemperatureRegulating();

void Form_temperature_parcel(uint8_t* parcel);
void reset_all_var();

void init_TIM15_as_TRGO_for_ADC_and_1sec_timer();

#endif /* MANAGE_PROGRAM_H_ */
