/*
 * TFT_throuht_SPI.c
 *
 *  Created on: 30 сент. 2023 г.
 *      Author: denlo
 */


#include <TFT_through_SPI.h>

void DC_set() {
	PORT_SPI->ODR |= (1 << PA_DC);
}

void DC_clear() {
	PORT_SPI->ODR &= ~(1 << PA_DC);
}

void TFT_reset() {
	PORT_SPI->ODR |= (1 << PA_RST);
}

void Set_DC_cmd() {
	PORT_SPI->ODR &= ~(1 << PA_DC);
}

void Set_DC_data() {
	PORT_SPI->ODR |= (1 << PA_DC);
}

void TFT_send_cmd(uint8_t cmd, uint8_t *data, uint8_t size) {
	Set_DC_cmd();
	for (int i = 0; i < 10; i++);
	spi1_Send1byte(&cmd, 1);
	for (int i = 0; i < 10; i++);

	if (size == 0) {
		return;
	}

	Set_DC_data();
	for (int i = 0; i < 10; i++);
	spi1_Send1byte(&data[0], size);
	for (int i = 0; i < 10; i++);
}

void TFT_init() {
  init_GPIO_for_SPI();
  SPI1_Master_init(8);
  TFT_reset();
  for (int i = 0; i < 150000; i++);

  TFT_cmd_sleep_out();
  TFT_display_normal_mode();
  TFT_display_on();
  TFT_pixel_format();
  TFT_clearAllDisplay(0x00, 0x00, 0x00);	//purple
  for(int i = 0; i < 10000; i++);
}

void TFT_cmd_sleep_out() {
	uint8_t data = 0x11;
	TFT_send_cmd(data, 0, 0);
	for (int i = 0; i < 500000; i++);
}

void TFT_display_normal_mode() {
	uint8_t data = 0x13;
	TFT_send_cmd(data, 0, 0);
	for (int i = 0; i < 10; i++);
}

void TFT_display_on() {
	TFT_send_cmd(0x29, 0, 0);
	for (int i = 0; i < 10; i++);
}

void TFT_pixel_format() {
	uint8_t data = 0x55;	//pixel forma is 565 bit
	TFT_send_cmd(0x3A, &data, 1);
	for (int i = 0; i < 10; i++);
}

static uint16_t count_pixels;
void TFT_clearAllDisplay(uint8_t red, uint8_t green, uint8_t blue) {
	uint16_t row_start = 0;
	uint16_t row_end = 0x10;

	for (;;) {
		TFT_set_region(0x00, row_start, row_end, 0, 240);
		TFT_colorise(red, green, blue);
		row_start += 0x10;
		row_end += 0x10;

		if (row_start > 320) {
			row_end = 0x10;
			row_start = 0;
			break;
		}

		for (int i = 0; i < 10000; i++);
	}
}

void TFT_clearPartDisplay(uint8_t red, uint8_t green, uint8_t blue) {
	TFT_set_region(0x00, 80, 144, 112, 176);
	TFT_colorise(red, green, blue);
}


void TFT_set_region(uint8_t data, uint16_t row_start, uint16_t row_end, uint16_t col_start, uint16_t col_end) {
	count_pixels = (row_end - row_start) * (col_end - col_start);
	TFT_send_cmd(0x36, &data, 1);
	for (int i = 0; i < 10; i++);

	TFT_set_column(col_start, col_end);
	TFT_set_row(row_start, row_end);
	TFT_ram_write();
}

void TFT_colorise(uint8_t red, uint8_t green, uint8_t blue) {
	Set_DC_data();
	for (int i = 0; i < 10; i++);
	uint16_t total_color = ((blue << 11) & 0xF800) | ((green << 5) & 0x07E0) | (red & 0x001F);
	total_color = (total_color << 8) | (total_color >> 8);
	spi1_SendDataDMA_2byteNTimes(total_color, count_pixels);
}

void TFT_ram_write() {
	uint8_t data_row[2] = {0x00, 0x00};
	TFT_send_cmd(0x2C, &data_row[0], 2);
	for (int i = 0; i < 10; i++);
}

void TFT_set_column(uint16_t col_start, uint16_t col_end) {
	uint8_t data_column[4] = {	(uint8_t)((col_start >> 8) & 0xFF),
								(uint8_t)(col_start & 0xFF),
								(uint8_t)((col_end >> 8) & 0xFF),
								(uint8_t)(col_end & 0xFF)};
	TFT_send_cmd(0x2A, &data_column[0], 4);
	for (int i = 0; i < 10; i++);
}

void TFT_set_row(uint16_t row_start, uint16_t row_end) {
	uint8_t data_row[4] = {	(uint8_t)((row_start >> 8) & 0xFF),
							(uint8_t)(row_start & 0xFF),
							(uint8_t)((row_end >> 8) & 0xFF),
							(uint8_t)(row_end & 0xFF)};
	TFT_send_cmd(0x2B, &data_row[0], 4);
	for (int i = 0; i < 10; i++);
}
