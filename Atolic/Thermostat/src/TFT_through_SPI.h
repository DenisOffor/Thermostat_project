/*
 * TFT_throuht_SPI.h
 *
 *  Created on: 30 сент. 2023 г.
 *      Author: denlo
 */

#ifndef TFT_THROUGH_SPI_H_
#define TFT_THROUGH_SPI_H_

#include "SPI_for_TFT.h"

#define TFT_WIDTH 240
#define TFT_HEIGHT 320

void TFT_init();
void DC_set();
void DC_clear();
void TFT_reset();
void Set_DC_cmd();
void Set_DC_data();
void TFT_send_cmd(uint8_t cmd, uint8_t *data, uint8_t size);
void TFT_cmd_sleep_out();
void TFT_display_normal_mode();
void TFT_display_on();
void TFT_pixel_format();
void TFT_clearAllDisplay(uint8_t red, uint8_t green, uint8_t blue);
void TFT_clearPartDisplay(uint8_t red, uint8_t green, uint8_t blue, uint8_t row_start, uint8_t row_end);
void TFT_set_region(uint8_t data, uint16_t row_start, uint16_t row_end, uint16_t col_start, uint16_t col_end);
void TFT_colorise(uint8_t red, uint8_t green, uint8_t blue);
void TFT_ram_write();
void TFT_set_column(uint16_t col_start, uint16_t col_end);
void TFT_set_row(uint16_t row_start, uint16_t row_end);


#endif /* TFT_THROUGH_SPI_H_ */
