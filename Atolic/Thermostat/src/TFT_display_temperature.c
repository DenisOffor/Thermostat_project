/*
 * TFT_display_temperature.c
 *
 *  Created on: 7 окт. 2023 г.
 *      Author: denlo
 */

#ifndef TFT_DISPLAY_TEMPERATURE_C_
#define TFT_DISPLAY_TEMPERATURE_C_

#include "TFT_display_temperature.h"

Symbols_Distribution symbols_distribution;
uint8_t previous_full_width_curr_temp = 0;
uint8_t previous_full_width_aim_temp = 0;

void display_temperature(double temperature, TYPE_OF_TEMPERATURE type_of_temp) {
	Symbol_Distribution_clear();
	Parse_temperature(&temperature);

	uint8_t full_width = 0;
	for(uint8_t i = 0; i < symbols_distribution.amout_of_symbols; i++)
		full_width += symbols_distribution.custom_width[i];

	uint8_t start_row = 0;
	switch(type_of_temp) {
		case CURRENT_TEMP:
			if(previous_full_width_curr_temp != full_width)
				TFT_clearPartDisplay(0x00, 0x00, 0x00, START_ROW_CURR_TEMP, START_ROW_CURR_TEMP + DIGIT_HEIGHT);
			start_row = START_ROW_CURR_TEMP;
			break;
		case AIM_TEMP:
			if(previous_full_width_aim_temp != full_width)
				TFT_clearPartDisplay(0x00, 0x00, 0x00, START_ROW_AIM_TEMP, START_ROW_AIM_TEMP + DIGIT_HEIGHT);
			start_row = START_ROW_AIM_TEMP;
			break;
		case NTC_TEMP:
			start_row = START_ROW_NTC_TEMP;
			break;
	}

	uint8_t start_col = (TFT_WIDTH - full_width) / 2;
	//mirror output just for eliminate sending cmd to TFT
	for(int i = symbols_distribution.amout_of_symbols - 1; i >= 0; i--) {
		Choose_symbol_for_draw(symbols_distribution.char_output[i], start_row, start_col);
		start_col += symbols_distribution.custom_width[i];
		for(uint16_t i = 0; i < 1000; i++);
	}

	switch(type_of_temp) {
		case CURRENT_TEMP:
			previous_full_width_curr_temp = full_width;
			break;
		case AIM_TEMP:
			previous_full_width_aim_temp = full_width;
			break;
	}
}

void Symbol_Distribution_clear () {
	for(uint8_t i = 0; i < MAX_SIZE_OF_OUTPUT; i++)
		symbols_distribution.char_output[i] = 0;
	for(uint8_t i = 0; i < MAX_SIZE_OF_OUTPUT; i++)
		symbols_distribution.custom_width[i] = 0;
	symbols_distribution.amout_of_symbols = 0;
}

void Parse_temperature(double* temperature) {
	double temp = *temperature;
	//if temperature is 0xFF then aim temperature not got
	if(*temperature == 255){
		Symbol_distribution_add_char(MINUS_WIDTH, '-');
		Symbol_distribution_add_char(MINUS_WIDTH, '-');
		return;
	}
	//if temperature is negative then add minus to symbols array
	if(*temperature < 0)
		Symbol_distribution_add_char(MINUS_WIDTH,'-');
	//find amount of hundred
	if(abs(*temperature) >= 100 )
		Symbol_distribution_add_char(DIGIT_WIDTH, '1');
	//find amount of tens
	if(abs(*temperature) >= 10 ) {
		temp = fmod(*temperature / 10, 10.0);
		Symbol_distribution_add_char(DIGIT_WIDTH, (abs(temp)) + 48);
	}
	//find amount of units
	temp = fmod(*temperature,10.0);
	Symbol_distribution_add_char(DIGIT_WIDTH, (abs(temp)) + 48);
	//check on fractional part of digit
	temp = fmod(*temperature,1.0);
	if(temp == 0) {				   //if it absent then add celsium symbol and return
		Symbol_distribution_add_char(CELSIUM_WIDTH, '@');
		return;
	}

	//else display digit with 2 digit after dot
	Symbol_distribution_add_char(DOT_WIDTH, '.');

	double fractionalPart = fmod(*temperature, 1.0);
	fractionalPart *= 10.0;
	Symbol_distribution_add_char(DIGIT_WIDTH, (abs(fractionalPart)) + 48);

	fractionalPart = fmod(*temperature, 0.1);
	fractionalPart *= 100.0;
	Symbol_distribution_add_char(DIGIT_WIDTH, (abs(fractionalPart)) + 48);

	Symbol_distribution_add_char(CELSIUM_WIDTH, '@');
}

void Symbol_distribution_add_char(uint8_t width, uint8_t symbol) {
	symbols_distribution.custom_width[symbols_distribution.amout_of_symbols] = width;
	symbols_distribution.char_output[symbols_distribution.amout_of_symbols++] = symbol;
}

void Choose_symbol_for_draw(uint8_t symbol, uint8_t start_row, uint8_t start_col) {
	//read symbol arr from FLASH and draw it
	switch(symbol) {
	case '0':
		ReadFromFlash(PAGE60_FOR_0_1_2_3, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE60_0_POSITION,PAGE60_0_POSITION + SIZE_0_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, start_row, start_col);
		break;
	case '1':
		ReadFromFlash(PAGE60_FOR_0_1_2_3, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE60_1_POSITION,PAGE60_1_POSITION + SIZE_1_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, start_row, start_col);
		break;
	case '2':
		ReadFromFlash(PAGE60_FOR_0_1_2_3, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE60_2_POSITION,PAGE60_2_POSITION + SIZE_0_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, start_row, start_col);
			break;
	case '3':
		ReadFromFlash(PAGE60_FOR_0_1_2_3, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE60_3_POSITION,PAGE60_3_POSITION + SIZE_3_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, start_row, start_col);
			break;
	case '4':
		ReadFromFlash(PAGE61_FOR_4_5_6_7, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE61_4_POSITION,PAGE61_4_POSITION + SIZE_4_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, start_row, start_col);
			break;
	case '5':
		ReadFromFlash(PAGE61_FOR_4_5_6_7, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE61_5_POSITION,PAGE61_5_POSITION + SIZE_5_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, start_row, start_col);
			break;
	case '6':
		ReadFromFlash(PAGE61_FOR_4_5_6_7, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE61_6_POSITION,PAGE61_6_POSITION + SIZE_6_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, start_row, start_col);
			break;
	case '7':
		ReadFromFlash(PAGE61_FOR_4_5_6_7, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE61_7_POSITION,PAGE61_7_POSITION + SIZE_7_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, start_row, start_col);
			break;
	case '8':
		ReadFromFlash(PAGE62_FOR_8_9_CELSIUM, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE62_8_POSITION,PAGE62_8_POSITION + SIZE_8_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, start_row, start_col);
			break;
	case '9':
		ReadFromFlash(PAGE62_FOR_8_9_CELSIUM, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE62_9_POSITION,PAGE62_9_POSITION + SIZE_9_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, start_row, start_col);
			break;
	case '@':
		ReadFromFlash(PAGE62_FOR_8_9_CELSIUM, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE62_CELSIUM_POSITION,PAGE62_CELSIUM_POSITION + CELSIUM_SIZE_BYTE, CELSIUM_WIDTH, CELSIUM_HEIGHT, start_row, start_col);
			break;
	case '.':
		ReadFromFlash(PAGE63_FOR_DOT_MINUS, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE63_DOT_POSITION,PAGE63_DOT_POSITION + DOT_SIZE_BYTE, DOT_WIDTH, DOT_HEIGHT, start_row, start_col);
				break;
	case '-':
		ReadFromFlash(PAGE63_FOR_DOT_MINUS, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE63_MINUS_POSITION,PAGE63_MINUS_POSITION + MINUSE_SIZE_BYTE, MINUS_WIDTH, MINUS_HEIGHT, start_row, start_col);
				break;
	}
}

void TFT_draw_symbol(uint16_t start_position_in_arr, uint16_t end_position_in_arr, uint8_t symbol_width, uint8_t symbol_height, uint8_t start_row, uint8_t start_col) {
	//split color mat on block, because 64*64 symbol need 4096*2 byte, but RAM just 8kb, so to reduce size of color_mat there are parcels
	uint16_t parcel = (end_position_in_arr - start_position_in_arr) / AMOUNT_OF_PARCEL;
	for(uint8_t parcel_iter = 0; parcel_iter < AMOUNT_OF_PARCEL; parcel_iter++) {
		for(uint16_t i = start_position_in_arr + parcel_iter * parcel; i < start_position_in_arr + parcel + parcel_iter * parcel; i++) {
			for(uint16_t j = 0; j < 8; j++) {
				//take the bit from byte and, depending on 0 or 1 it is, write color in color_mat
				if((mat_for_symbol[i] << j) & 0x80) {
					color_mat[8*(i - start_position_in_arr - parcel_iter * parcel) + j] = 0x0000;
					continue;
				}
				color_mat[8*(i - start_position_in_arr - parcel_iter * parcel) + j] = 0xFFFF;
			}
		}
		TFT_set_region(0x00, start_row + symbol_height / AMOUNT_OF_PARCEL * parcel_iter, start_row + symbol_height / AMOUNT_OF_PARCEL + symbol_height / AMOUNT_OF_PARCEL * parcel_iter - 1, start_col, start_col + symbol_width - 1);
		Set_DC_data();
		SPI1_SendDataDMA(&color_mat[0], parcel*8);
	}
}

void TFT_reset_temperature() {
	temperatures.curr_temperature = temperatures.aim_temperature = RESET_TEMPERATURE;
	display_temperature(temperatures.curr_temperature, CURRENT_TEMP);
	display_temperature(temperatures.aim_temperature, AIM_TEMP);
}

void TFT_DRAW_GRAPH(uint8_t* buf) {
	for(uint16_t i = 0; i < AMOUNT_OF_BYTE_ON_PAGES_44_55; i++) {
		for(uint16_t j = 0; j < 8; j++) {
			//take the bit from byte and, depending on 0 or 1 it is, write color in color_mat
			if((buf[i] << j) & 0x80) {
				color_mat[8*i + j] = 0x0000;
				continue;
			}
			color_mat[8*i + j] = 0xFFFF;
		}
	}
	TFT_set_region(0x00, 4*amount_of_got_parcel, 4*amount_of_got_parcel + 3, 0, TFT_WIDTH);
	Set_DC_data();
	SPI1_SendDataDMA(&color_mat[0], AMOUNT_OF_BYTE_ON_PAGES_44_55);
	amount_of_got_parcel++;

	if(amount_of_got_parcel == 10)
		amount_of_got_parcel = 0;

	//Write_data_to_flash(PAGE44 + amount_of_got_parcel * FLASH_PAGE_SIZE, buf, AMOUNT_OF_BYTE_ON_PAGES_44_55);
	//amount_of_got_parcel++;
}

#endif /* TFT_DISPLAY_TEMPERATURE_C_ */
