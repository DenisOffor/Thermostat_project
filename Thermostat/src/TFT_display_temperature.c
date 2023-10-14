/*
 * TFT_display_temperature.c
 *
 *  Created on: 7 окт. 2023 г.
 *      Author: denlo
 */

#ifndef TFT_DISPLAY_TEMPERATURE_C_
#define TFT_DISPLAY_TEMPERATURE_C_

#include "TFT_display_temperature.h"

Symbol_Distribution symbol_distribution;
uint8_t previous_amount_of_symbols = 0;

void display_temperature(double temperature) {
	Symbol_Distribution_clear();
	Parse_temperature(&temperature);

	uint8_t full_width = 0;
	for(uint8_t i = 0; i < symbol_distribution.amout_of_symbols; i++)
		full_width += symbol_distribution.custom_width[i];
	//full_width += SPACE_BEETWEN_SYMBOLS * (symbol_distribution.amout_of_symbols - 2);
	if(previous_amount_of_symbols != symbol_distribution.amout_of_symbols)
		TFT_clearPartDisplay(0x00, 0x00, 0x00);
	uint8_t start_col = (TFT_WIDTH - full_width) / 2;
	for(int i = symbol_distribution.amout_of_symbols - 1; i >= 0; i--) { //mirror output just for eliminate sending cmd to TFT
		Choose_symbol_for_draw(symbol_distribution.consequense_of_output[i], start_col);
		start_col += symbol_distribution.custom_width[i];
		for(uint16_t i = 0; i < 1000; i++);
	}
	previous_amount_of_symbols = symbol_distribution.amout_of_symbols;
	program_task = WAITING;
}

void Symbol_Distribution_clear () {
	for(uint8_t i = 0; i < MAX_SIZE_OF_OUTPUT; i++)
		symbol_distribution.char_output[i] = 0;
	for(uint8_t i = 0; i < MAX_SIZE_OF_OUTPUT; i++)
		symbol_distribution.consequense_of_output[i] = 0;
	for(uint8_t i = 0; i < MAX_SIZE_OF_OUTPUT; i++)
		symbol_distribution.custom_width[i] = 0;
	symbol_distribution.amout_of_symbols = 0;
	symbol_distribution.amout_of_0 = 0;
	symbol_distribution.amout_of_1 = 0;
	symbol_distribution.amout_of_2 = 0;
	symbol_distribution.amout_of_3 = 0;
	symbol_distribution.amout_of_4 = 0;
	symbol_distribution.amout_of_5 = 0;
	symbol_distribution.amout_of_6 = 0;
	symbol_distribution.amout_of_7 = 0;
	symbol_distribution.amout_of_8 = 0;
	symbol_distribution.amout_of_9 = 0;
	symbol_distribution.amout_of_c = 0;
	symbol_distribution.amout_of_dot = 0;
	symbol_distribution.amout_of_minus = 0;
}

void Parse_temperature(double* temperature) {
	double temp = *temperature;
	//if temperature is negative then add minus to symbols array
	if(temp < 0){
		symbol_distribution.custom_width[symbol_distribution.amout_of_symbols] = MINUS_WIDTH;
		symbol_distribution.consequense_of_output[symbol_distribution.amout_of_symbols] = Number_for_MINUS;
		symbol_distribution.char_output[symbol_distribution.amout_of_symbols++] = '-';
	}
	//there is always 1 symbol before dot, so if digit < 1 then there is zero before dot
	if(abs(temp) < 1) {
		symbol_distribution.custom_width[symbol_distribution.amout_of_symbols] = DIGIT_WIDTH;
		symbol_distribution.consequense_of_output[symbol_distribution.amout_of_symbols] = 0;
		symbol_distribution.char_output[symbol_distribution.amout_of_symbols++] = '0';
	}
	//find amount of tens
	if(abs(temp) >= 10 ) {
		temp /= 10;
		symbol_distribution.custom_width[symbol_distribution.amout_of_symbols] = DIGIT_WIDTH;
		symbol_distribution.consequense_of_output[symbol_distribution.amout_of_symbols] = abs(temp);
		symbol_distribution.char_output[symbol_distribution.amout_of_symbols++] = ((char)abs(temp)) + 48;
	}
	//find amount of units
	if(abs(temp) >= 1 ) {
		temp = fmod(*temperature,10.0);
		symbol_distribution.custom_width[symbol_distribution.amout_of_symbols] = DIGIT_WIDTH;
		symbol_distribution.consequense_of_output[symbol_distribution.amout_of_symbols] = abs(temp);
		symbol_distribution.char_output[symbol_distribution.amout_of_symbols++] = ((char)abs(temp)) + 48;
	}
	//check on fractional part of digit
	temp = fmod(*temperature,1.0);
	if(temp == 0) {				   //if it absent then add celsium symbol and return
		symbol_distribution.custom_width[symbol_distribution.amout_of_symbols] = CELSIUM_WIDTH;
		symbol_distribution.consequense_of_output[symbol_distribution.amout_of_symbols++] = Number_for_CELSIUM;
		return;
	}

	//else display digit with 2 digit after dot
	symbol_distribution.custom_width[symbol_distribution.amout_of_symbols] = DOT_WIDTH;
	symbol_distribution.consequense_of_output[symbol_distribution.amout_of_symbols] = Number_for_DOT;
	symbol_distribution.char_output[symbol_distribution.amout_of_symbols++] = '.';

	double fractionalPart = fmod(*temperature, 1.0);
	fractionalPart *= 10.0;
	symbol_distribution.custom_width[symbol_distribution.amout_of_symbols] = DIGIT_WIDTH;
	symbol_distribution.consequense_of_output[symbol_distribution.amout_of_symbols] = abs(fractionalPart);
	symbol_distribution.char_output[symbol_distribution.amout_of_symbols++] = ((char)abs(fractionalPart)) + 48;

	fractionalPart = fmod(*temperature, 0.1);
	fractionalPart *= 100.0;
	symbol_distribution.custom_width[symbol_distribution.amout_of_symbols] = DIGIT_WIDTH;
	symbol_distribution.consequense_of_output[symbol_distribution.amout_of_symbols] = abs(fractionalPart);
	symbol_distribution.char_output[symbol_distribution.amout_of_symbols++] = ((char)abs(fractionalPart)) + 48;

	symbol_distribution.custom_width[symbol_distribution.amout_of_symbols] = CELSIUM_WIDTH;
	symbol_distribution.consequense_of_output[symbol_distribution.amout_of_symbols++] = Number_for_CELSIUM;
}

void Choose_symbol_for_draw(uint8_t symbol, uint8_t start_col) {
	//read symbol arr from FLASH and draw it
	switch(symbol) {
	case 0:
		ReadFromFlash(PAGE60_FOR_0_1_2_3, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE60_0_POSITION,PAGE60_0_POSITION + SIZE_0_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, START_ROW, start_col);
		break;
	case 1:
		ReadFromFlash(PAGE60_FOR_0_1_2_3, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE60_1_POSITION,PAGE60_1_POSITION + SIZE_1_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, START_ROW, start_col);
		break;
	case 2:
		ReadFromFlash(PAGE60_FOR_0_1_2_3, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE60_2_POSITION,PAGE60_2_POSITION + SIZE_0_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, START_ROW, start_col);
			break;
	case 3:
		ReadFromFlash(PAGE60_FOR_0_1_2_3, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE60_3_POSITION,PAGE60_3_POSITION + SIZE_3_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, START_ROW, start_col);
			break;
	case 4:
		ReadFromFlash(PAGE61_FOR_4_5_6_7, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE61_4_POSITION,PAGE61_4_POSITION + SIZE_4_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, START_ROW, start_col);
			break;
	case 5:
		ReadFromFlash(PAGE61_FOR_4_5_6_7, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE61_5_POSITION,PAGE61_5_POSITION + SIZE_5_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, START_ROW, start_col);
			break;
	case 6:
		ReadFromFlash(PAGE61_FOR_4_5_6_7, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE61_6_POSITION,PAGE61_6_POSITION + SIZE_6_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, START_ROW, start_col);
			break;
	case 7:
		ReadFromFlash(PAGE61_FOR_4_5_6_7, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE61_7_POSITION,PAGE61_7_POSITION + SIZE_7_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, START_ROW, start_col);
			break;
	case 8:
		ReadFromFlash(PAGE62_FOR_8_9_CELSIUM, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE62_8_POSITION,PAGE62_8_POSITION + SIZE_8_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, START_ROW, start_col);
			break;
	case 9:
		ReadFromFlash(PAGE62_FOR_8_9_CELSIUM, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE62_9_POSITION,PAGE62_9_POSITION + SIZE_9_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, START_ROW, start_col);
			break;
	case 10:
		ReadFromFlash(PAGE62_FOR_8_9_CELSIUM, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE62_CELSIUM_POSITION,PAGE62_CELSIUM_POSITION + CELSIUM_SIZE_BYTE, CELSIUM_WIDTH, CELSIUM_HEIGHT, START_ROW, start_col);
			break;
	case 11:
		ReadFromFlash(PAGE63_FOR_DOT_MINUS, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE63_DOT_POSITION,PAGE63_DOT_POSITION + DOT_SIZE_BYTE, DOT_WIDTH, DOT_HEIGHT, START_ROW, start_col);
				break;
	case 12:
		ReadFromFlash(PAGE63_FOR_DOT_MINUS, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE63_MINUS_POSITION,PAGE63_MINUS_POSITION + MINUSE_SIZE_BYTE, MINUS_WIDTH, MINUS_HEIGHT, START_ROW, start_col);
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

#endif /* TFT_DISPLAY_TEMPERATURE_C_ */
