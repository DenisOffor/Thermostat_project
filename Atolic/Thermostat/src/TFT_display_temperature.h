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

#define NOT_ALL_PARCEL_HERE 0
#define ALL_PARCEL_HERE 1
#define AMOUNT_OF_PARCEL_FOR_GRAPH 10
uint8_t amount_of_got_parcel;
uint8_t parcel_state;
uint8_t previous_full_width_curr_temp;
uint8_t previous_full_width_aim_temp;

#define AMOUNT_OF_PARCEL 4
#define MAX_SIZE_OF_OUTPUT 7
#define START_ROW_CURR_TEMP 50
#define START_ROW_AIM_TEMP 120

#define Number_for_CELSIUM 10
#define Number_for_DOT 11
#define Number_for_MINUS 12

typedef enum TYPE_OF_TEMPERATURE {
	CURRENT_TEMP = 0,
	AIM_TEMP,
} TYPE_OF_TEMPERATURE;

typedef struct Symbols_Distribution
{
	uint8_t char_output[MAX_SIZE_OF_OUTPUT];
	uint8_t custom_width[MAX_SIZE_OF_OUTPUT];
	uint8_t amout_of_symbols;
}Symbols_Distribution;

Symbols_Distribution symbols_distribution;

void display_temperature(double temperature, TYPE_OF_TEMPERATURE type_of_temp);
void Symbol_Distribution_clear();
void Parse_temperature(double* temperature);
void Symbol_distribution_add_char(uint8_t width, uint8_t symbol);
void Choose_symbol_for_draw(uint8_t symbol, uint8_t start_row, uint8_t start_col);
void TFT_draw_symbol(uint16_t start_position_in_arr, uint16_t end_position_in_arr, uint8_t symbol_width,
		uint8_t symbol_height, uint8_t start_row, uint8_t start_col);
void TFT_reset_temperature();
void TFT_picture_Wrire_in_FLASH(uint8_t* buf);
void TFT_draw_plot();

#endif /* TFT_DISPLAY_TEMPERATURE_H_ */
