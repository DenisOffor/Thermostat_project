/*
 * manage_program.h
 *
 *  Created on: 18 окт. 2023 г.
 *      Author: denlo
 */

#ifndef MANAGE_PROGRAM_H_
#define MANAGE_PROGRAM_H_

#include "TFT_display_temperature.h"
#include "ds18b20.h"
#include "init_TEC_throght_relay.h"
#include "UART_for_PC.h"
#include "NTC_10k_using_ADC.h"

void init_clock();
void process_cmd();
void init_periphery();
void check_UART_cmd();


#endif /* MANAGE_PROGRAM_H_ */
