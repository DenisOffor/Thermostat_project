/*
 * TFT_display_temperature.h
 *
 *  Created on: 7 окт. 2023 г.
 *      Author: denlo
 */

#ifndef TFT_DISPLAY_TEMPERATURE_H_
#define TFT_DISPLAY_TEMPERATURE_H_

#include "TFT_through_SPI.h"

#define AMOUNT_OF_PARCEL 4

void Choose_symbol_for_draw(uint8_t symbol);
void TFT_draw_symbol(uint16_t start_position, uint16_t end_position, uint8_t symbol_width, uint8_t symbol_height, uint8_t start_row, uint8_t start_col);

#endif /* TFT_DISPLAY_TEMPERATURE_H_ */
