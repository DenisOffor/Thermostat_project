/*
 * constant.h
 *
 *  Created on: 30 ����. 2023 �.
 *      Author: denlo
 */

#ifndef CONSTANT_H_
#define CONSTANT_H_

#include "stm32f0xx.h"
#include "buffer.h"
#include "FLASH.h"

#define FREQ_MULTIPLIER_COEF 5
#define RESET_TEMPERATURE 0xFF

//uint8_t mat_for_symbol1[1024];
//uint8_t mat_for_symbol2[1024];
//uint8_t mat_for_symbol3[1024];
//uint8_t mat_for_symbol4[256];

typedef struct Temperatures {
	double curr_temperature;
	int aim_temperature;
} Temperatures;

Temperatures temperatures;

#endif /* CONSTANT_H_ */