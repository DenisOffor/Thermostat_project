/*
 * TFT_display_temperature.c
 *
 *  Created on: 7 окт. 2023 г.
 *      Author: denlo
 */

#ifndef TFT_DISPLAY_TEMPERATURE_C_
#define TFT_DISPLAY_TEMPERATURE_C_

#include "TFT_display_temperature.h"


void Choose_symbol_for_draw(uint8_t symbol) {
	switch(symbol) {
	case '0':
		ReadFromFlash(PAGE60_FOR_0_1_2_3, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE60_0_POSITION,PAGE60_0_POSITION + SIZE_0_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, 80, 112);
		break;
	case '1':
		ReadFromFlash(PAGE60_FOR_0_1_2_3, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE60_1_POSITION,PAGE60_1_POSITION + SIZE_1_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, 80, 112);
		break;
	case '2':
		ReadFromFlash(PAGE60_FOR_0_1_2_3, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE60_2_POSITION,PAGE60_2_POSITION + SIZE_0_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, 80, 112);
			break;
	case '3':
		ReadFromFlash(PAGE60_FOR_0_1_2_3, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE60_3_POSITION,PAGE60_3_POSITION + SIZE_3_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, 80, 112);
			break;
	case '4':
		ReadFromFlash(PAGE61_FOR_4_5_6_7, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE61_4_POSITION,PAGE61_4_POSITION + SIZE_4_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, 80, 112);
			break;
	case '5':
		ReadFromFlash(PAGE61_FOR_4_5_6_7, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE61_5_POSITION,PAGE61_5_POSITION + SIZE_5_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, 80, 112);
			break;
	case '6':
		ReadFromFlash(PAGE61_FOR_4_5_6_7, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE61_6_POSITION,PAGE61_6_POSITION + SIZE_6_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, 80, 112);
			break;
	case '7':
		ReadFromFlash(PAGE61_FOR_4_5_6_7, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE61_7_POSITION,PAGE61_7_POSITION + SIZE_7_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, 80, 112);
			break;
	case '8':
		ReadFromFlash(PAGE62_FOR_8_9_CELSIUM, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE62_8_POSITION,PAGE62_8_POSITION + SIZE_8_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, 80, 112);
			break;
	case '9':
		ReadFromFlash(PAGE62_FOR_8_9_CELSIUM, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE62_9_POSITION,PAGE62_9_POSITION + SIZE_9_BYTE, DIGIT_WIDTH, DIGIT_HEIGHT, 80, 112);
			break;
	case 'c':
		ReadFromFlash(PAGE62_FOR_8_9_CELSIUM, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE62_CELSIUM_POSITION,PAGE62_CELSIUM_POSITION + CELSIUM_SIZE_BYTE, CELSIUM_WIDTH, CELSIUM_HEIGHT, 80, 112);
			break;
	case '.':
		ReadFromFlash(PAGE63_FOR_DOT_MINUS, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE63_DOT_POSITION,PAGE63_DOT_POSITION + DOT_SIZE_BYTE, DOT_WIDTH, DOT_HEIGHT, 80, 112);
				break;
	case '-':
		ReadFromFlash(PAGE63_FOR_DOT_MINUS, &mat_for_symbol[0], FLASH_PAGE_SIZE);
		TFT_draw_symbol(PAGE63_MINUS_POSITION,PAGE63_MINUS_POSITION + MINUSE_SIZE_BYTE, MINUS_WIDTH, MINUS_HEIGHT, 80, 112);
				break;
	}
}

void TFT_draw_symbol(uint16_t start_position, uint16_t end_position, uint8_t symbol_width, uint8_t symbol_height, uint8_t start_row, uint8_t start_col) {
	uint16_t parcel = (end_position - start_position) / AMOUNT_OF_PARCEL;
	for(uint8_t parcel_iter = 0; parcel_iter < AMOUNT_OF_PARCEL; parcel_iter++) {
		for(uint16_t i = start_position + parcel_iter * parcel; i < start_position + parcel + parcel_iter * parcel; i++) {
			for(uint16_t j = 0; j < 8; j++) {
				if((mat_for_symbol[i] << j) & 0x80) {
					color_mat[8*(i - start_position - parcel_iter * parcel) + j] = 0x0000;
					continue;
				}
				color_mat[8*(i - start_position - parcel_iter * parcel) + j] = 0xFFFF;
			}
		}
		TFT_set_region(0x00, start_row + symbol_height / AMOUNT_OF_PARCEL * parcel_iter, start_row + symbol_height / AMOUNT_OF_PARCEL + symbol_height / AMOUNT_OF_PARCEL * parcel_iter - 1, start_col, start_col + symbol_width - 1);
		Set_DC_data();
		spi1_SendDataDMA(&color_mat[0], parcel*8);
	}
}

#endif /* TFT_DISPLAY_TEMPERATURE_C_ */
