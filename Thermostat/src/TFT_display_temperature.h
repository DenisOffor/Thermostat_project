/*
 * TFT_display_temperature.h
 *
 *  Created on: 7 окт. 2023 г.
 *      Author: denlo
 */

#ifndef TFT_DISPLAY_TEMPERATURE_H_
#define TFT_DISPLAY_TEMPERATURE_H_

#include "TFT_through_SPI.h"
#include <stdlib.h>
#include <math.h>

#define AMOUNT_OF_PARCEL 4
#define MAX_SIZE_OF_OUTPUT 7
#define SPACE_BEETWEN_SYMBOLS 2
#define START_ROW 128

#define Number_for_CELSIUM 10
#define Number_for_DOT 11
#define Number_for_MINUS 12

typedef struct Symbol_Distribution
{
	uint8_t consequense_of_output[MAX_SIZE_OF_OUTPUT];
	uint16_t custom_width[MAX_SIZE_OF_OUTPUT];
	uint8_t amout_of_symbols;
	uint8_t amout_of_0;
	uint8_t amout_of_1;
	uint8_t amout_of_2;
	uint8_t amout_of_3;
	uint8_t amout_of_4;
	uint8_t amout_of_5;
	uint8_t amout_of_6;
	uint8_t amout_of_7;
	uint8_t amout_of_8;
	uint8_t amout_of_9;
	uint8_t amout_of_c;
	uint8_t amout_of_dot;
	uint8_t amout_of_minus;
}Symbol_Distribution;

Symbol_Distribution symbol_distribution;

void display_temperature(double temperature);
void Symbol_Distribution_clear();
void Parse_temperature(double* temperature);
void Choose_symbol_for_draw(uint8_t symbol, uint8_t start_col);
void TFT_draw_symbol(uint16_t start_position, uint16_t end_position, uint8_t symbol_width,
		uint8_t symbol_height, uint8_t start_row, uint8_t start_col);

#endif /* TFT_DISPLAY_TEMPERATURE_H_ */
