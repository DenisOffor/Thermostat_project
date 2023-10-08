/*
 * constant.c
 *
 *  Created on: 30 ����. 2023 �.
 *      Author: denlo
 */

#include "constant.h"

PROGRAM_TASK program_task = 0;

uint8_t buf_tx[BUFFER_SIZE];
uint16_t color_mat[2048];
uint8_t mat_for_symbol[FLASH_PAGE_SIZE];

//uint8_t mat_for_symbol1[1024] = { 0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFC, 0x3F, 0xFF,
//		0xFF, 0xE0, 0x07, 0xFF,
//		0xFF, 0x83, 0xE1, 0xFF,
//		0xFF, 0x07, 0xE0, 0xFF,
//		0xFE, 0x0F, 0xF0, 0x3F,
//		0xFC, 0x0F, 0xF0, 0x3F,
//		0xF8, 0x0F, 0xF8, 0x1F,
//		0xF8, 0x0F, 0xF8, 0x0F,
//		0xF0, 0x1F, 0xF8, 0x0F,
//		0xF0, 0x1F, 0xF8, 0x07,
//		0xE0, 0x1F, 0xF8, 0x07,
//		0xE0, 0x1F, 0xF8, 0x07,
//		0xE0, 0x1F, 0xF8, 0x03,
//		0xC0, 0x1F, 0xF8, 0x03,
//		0xC0, 0x1F, 0xF8, 0x03,
//		0xC0, 0x1F, 0xF8, 0x03,
//		0xC0, 0x1F, 0xF8, 0x01,
//		0xC0, 0x1F, 0xF8, 0x01,
//		0x80, 0x1F, 0xF8, 0x01,
//		0x80, 0x1F, 0xF8, 0x01,
//		0x80, 0x1F, 0xF8, 0x01,
//		0x80, 0x1F, 0xF8, 0x01,
//		0x80, 0x1F, 0xF8, 0x01,
//		0x80, 0x1F, 0xF8, 0x01,
//		0x80, 0x1F, 0xF8, 0x01,
//		0x80, 0x1F, 0xF8, 0x01,
//		0x80, 0x1F, 0xF8, 0x01,
//		0x80, 0x1F, 0xF8, 0x01,
//		0x80, 0x1F, 0xF8, 0x01,
//		0x80, 0x1F, 0xF8, 0x01,
//		0x80, 0x1F, 0xF8, 0x01,
//		0x80, 0x1F, 0xF8, 0x01,
//		0x80, 0x1F, 0xF8, 0x01,
//		0xC0, 0x1F, 0xF8, 0x01,
//		0xC0, 0x1F, 0xF8, 0x01,
//		0xC0, 0x1F, 0xF8, 0x03,
//		0xC0, 0x1F, 0xF8, 0x03,
//		0xC0, 0x1F, 0xF8, 0x03,
//		0xE0, 0x1F, 0xF8, 0x03,
//		0xE0, 0x1F, 0xF8, 0x07,
//		0xE0, 0x1F, 0xF8, 0x07,
//		0xF0, 0x1F, 0xF8, 0x07,
//		0xF0, 0x0F, 0xF8, 0x0F,
//		0xF8, 0x0F, 0xF8, 0x0F,
//		0xFC, 0x0F, 0xF8, 0x1F,
//		0xFC, 0x0F, 0xF0, 0x3F,
//		0xFE, 0x0F, 0xF0, 0x7F,
//		0xFF, 0x07, 0xE0, 0xFF,
//		0xFF, 0x83, 0xE1, 0xFF,
//		0xFF, 0xE0, 0x07, 0xFF,
//		0xFF, 0xFC, 0x3F, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xEF, 0xFF, 0xFF,
//		0xFF, 0xE3, 0xFF, 0xFF,
//		0xFF, 0xE0, 0xFF, 0xFF,
//		0xFF, 0xE0, 0x1F, 0xFF,
//		0xFF, 0xE0, 0x07, 0xFF,
//		0xFF, 0xE0, 0x01, 0xFF,
//		0xFF, 0xE0, 0x00, 0x7F,
//		0xFF, 0xE0, 0x00, 0x1F,
//		0xFF, 0xE0, 0x03, 0xF7,
//		0xFF, 0xE0, 0x07, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xE0, 0x0F, 0xFF,
//		0xFF, 0xC0, 0x0F, 0xFF,
//		0xFF, 0xC0, 0x0F, 0xFF,
//		0xFF, 0xC0, 0x07, 0xFF,
//		0xFF, 0xC0, 0x07, 0xFF,
//		0xFF, 0x80, 0x03, 0xFF,
//		0xFF, 0x00, 0x01, 0xFF,
//		0xE0, 0x00, 0x00, 0x0F,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xF8, 0x3F, 0xFF,
//		0xFF, 0xC0, 0x07, 0xFF,
//		0xFF, 0x00, 0x01, 0xFF,
//		0xFE, 0x00, 0x00, 0xFF,
//		0xFC, 0x00, 0x00, 0x7F,
//		0xF8, 0x00, 0x00, 0x3F,
//		0xF0, 0x00, 0x00, 0x1F,
//		0xF0, 0x00, 0x00, 0x0F,
//		0xE0, 0x00, 0x7F, 0x0F,
//		0xE0, 0x00, 0xFF, 0xC7,
//		0xE0, 0x01, 0xFF, 0xE7,
//		0xE0, 0x03, 0xFF, 0xF7,
//		0xE0, 0x07, 0xFF, 0xF3,
//		0xE0, 0x07, 0xFF, 0xFF,
//		0xE0, 0x0F, 0xFF, 0xFF,
//		0xE0, 0x0F, 0xFF, 0xFF,
//		0xE0, 0x0F, 0xFF, 0xFF,
//		0xE0, 0x0F, 0xFF, 0xFF,
//		0xF0, 0x0F, 0xFF, 0xFF,
//		0xF0, 0x0F, 0xFF, 0xFF,
//		0xF0, 0x0F, 0xFF, 0xFF,
//		0xF8, 0x07, 0xFF, 0xFF,
//		0xF8, 0x07, 0xFF, 0xFF,
//		0xFC, 0x07, 0xFF, 0xFF,
//		0xFE, 0x03, 0xFF, 0xFF,
//		0xFE, 0x03, 0xFF, 0xFF,
//		0xFF, 0x03, 0xFF, 0xFF,
//		0xFF, 0x81, 0xFF, 0xFF,
//		0xFF, 0xC1, 0xFF, 0xFF,
//		0xFF, 0xC0, 0xFF, 0xFF,
//		0xFF, 0xE0, 0x7F, 0xFF,
//		0xFF, 0xF0, 0x7F, 0xFF,
//		0xFF, 0xF8, 0x3F, 0xFF,
//		0xFF, 0xFC, 0x1F, 0xFF,
//		0xFF, 0xFE, 0x1F, 0xFF,
//		0xFF, 0xFF, 0x0F, 0xFF,
//		0xFF, 0xFF, 0x87, 0xFF,
//		0x7F, 0xFF, 0x83, 0xFF,
//		0xFF, 0xFF, 0xC3, 0xFF,
//		0xFF, 0xFF, 0xE1, 0xFF,
//		0xFF, 0xFF, 0xF0, 0xFF,
//		0x80, 0x00, 0x00, 0x7F,
//		0xC0, 0x00, 0x00, 0x7F,
//		0xC0, 0x00, 0x00, 0x3F,
//		0xC0, 0x00, 0x00, 0x1F,
//		0xC0, 0x00, 0x00, 0x0F,
//		0xC0, 0x00, 0x00, 0x07,
//		0xE0, 0x00, 0x00, 0x07,
//		0xE0, 0x00, 0x00, 0x03,
//		0xE0, 0x00, 0x00, 0x01,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xF0, 0x7F, 0xFF,
//		0xFF, 0x80, 0x07, 0xFF,
//		0xFE, 0x00, 0x01, 0xFF,
//		0xFC, 0x00, 0x00, 0x7F,
//		0xF8, 0x00, 0x00, 0x3F,
//		0xF0, 0x00, 0x7F, 0x1F,
//		0xF0, 0x01, 0xFF, 0xCF,
//		0xF0, 0x03, 0xFF, 0xE7,
//		0xE0, 0x03, 0xFF, 0xF3,
//		0xE0, 0x07, 0xFF, 0xFF,
//		0xE0, 0x07, 0xFF, 0xFF,
//		0xF0, 0x07, 0xFF, 0xFF,
//		0xF0, 0x0F, 0xFF, 0xFF,
//		0xF0, 0x0F, 0xFF, 0xFF,
//		0xF8, 0x07, 0xFF, 0xFF,
//		0xFC, 0x07, 0xFF, 0xFF,
//		0xFE, 0x07, 0xFF, 0xFF,
//		0xFF, 0x03, 0xFF, 0xFF,
//		0xFF, 0x83, 0xFF, 0xFF,
//		0xFF, 0x01, 0xFF, 0xFF,
//		0xFC, 0x00, 0xFF, 0xFF,
//		0xF8, 0x00, 0x3F, 0xFF,
//		0xF0, 0x00, 0x0F, 0xFF,
//		0xE0, 0x00, 0x03, 0xFF,
//		0xE0, 0x00, 0x0F, 0xFF,
//		0xC0, 0x00, 0x7F, 0xFF,
//		0xC0, 0x01, 0xFF, 0xFF,
//		0x80, 0x03, 0xFF, 0xFF,
//		0x80, 0x07, 0xFF, 0xFF,
//		0x80, 0x0F, 0xFF, 0xFF,
//		0x80, 0x1F, 0xFF, 0xFF,
//		0x80, 0x1F, 0xFF, 0xFF,
//		0x80, 0x3F, 0xFF, 0xFF,
//		0x80, 0x3F, 0xFF, 0xFF,
//		0x80, 0x7F, 0xFF, 0xFF,
//		0x80, 0x7F, 0xFF, 0xFF,
//		0xC0, 0x7F, 0xFF, 0xFF,
//		0xC0, 0xFF, 0xFF, 0xFF,
//		0xC0, 0xFF, 0xFF, 0xFF,
//		0xE0, 0xFF, 0xFF, 0xFF,
//		0xE0, 0x7F, 0xFF, 0xC7,
//		0xF0, 0x7F, 0xFF, 0x81,
//		0xF0, 0x7F, 0xFE, 0x01,
//		0xF8, 0x3F, 0xFC, 0x01,
//		0xFC, 0x1F, 0xF0, 0x01,
//		0xFE, 0x0F, 0xE0, 0x01,
//		0xFF, 0x00, 0x00, 0x01,
//		0xFF, 0x80, 0x00, 0x01,
//		0xFF, 0xE0, 0x00, 0x03,
//		0xFF, 0xF8, 0x00, 0x0F,
//		0xFF, 0xFF, 0xC0, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF };
//
//
//uint8_t mat_for_symbol2[1024] = { 0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xF8, 0x7F, 0xFF, 0xFF,
//		0xF8, 0x3F, 0xFF, 0xFF,
//		0xF8, 0x1F, 0xFF, 0xFF,
//		0xF8, 0x1F, 0xFF, 0xFF,
//		0xF8, 0x0F, 0xFF, 0xFF,
//		0xF8, 0x07, 0xFF, 0xFF,
//		0xF8, 0x03, 0xFF, 0xFF,
//		0xF8, 0x03, 0xFF, 0xFF,
//		0xF8, 0x01, 0xFF, 0xFF,
//		0xF8, 0x00, 0xFF, 0xFF,
//		0xF8, 0x00, 0xFF, 0xFF,
//		0xF8, 0x00, 0x7F, 0xFF,
//		0xF8, 0x00, 0x3F, 0xFF,
//		0xF8, 0x03, 0x1F, 0xFF,
//		0xF8, 0x03, 0x1F, 0xFF,
//		0xF8, 0x03, 0x8F, 0xFF,
//		0xF8, 0x03, 0xC7, 0xFF,
//		0xF8, 0x03, 0xE3, 0xFF,
//		0xF8, 0x03, 0xE3, 0xFF,
//		0xF8, 0x03, 0xF1, 0xFF,
//		0xF8, 0x03, 0xF8, 0xFF,
//		0xF8, 0x03, 0xFC, 0xFF,
//		0xF8, 0x03, 0xFC, 0x7F,
//		0xF8, 0x03, 0xFE, 0x3F,
//		0xF8, 0x03, 0xFF, 0x1F,
//		0xF8, 0x03, 0xFF, 0x1F,
//		0xF8, 0x03, 0xFF, 0x8F,
//		0xF8, 0x03, 0xFF, 0xC7,
//		0xF8, 0x03, 0xFF, 0xE7,
//		0xF8, 0x03, 0xFF, 0xE3,
//		0xF8, 0x03, 0xFF, 0xF1,
//		0xF8, 0x03, 0xFF, 0xF8,
//		0x80, 0x00, 0x00, 0x00,
//		0x80, 0x00, 0x00, 0x00,
//		0x80, 0x00, 0x00, 0x00,
//		0x80, 0x00, 0x00, 0x00,
//		0x80, 0x00, 0x00, 0x00,
//		0x80, 0x00, 0x00, 0x00,
//		0x80, 0x00, 0x00, 0x00,
//		0xF8, 0x03, 0xFF, 0xFF,
//		0xF8, 0x03, 0xFF, 0xFF,
//		0xF8, 0x03, 0xFF, 0xFF,
//		0xF8, 0x03, 0xFF, 0xFF,
//		0xF8, 0x03, 0xFF, 0xFF,
//		0xF8, 0x03, 0xFF, 0xFF,
//		0xF8, 0x03, 0xFF, 0xFF,
//		0xF8, 0x03, 0xFF, 0xFF,
//		0xF8, 0x03, 0xFF, 0xFF,
//		0xF8, 0x03, 0xFF, 0xFF,
//		0xF8, 0x03, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0x80, 0x00, 0x01, 0xFF,
//		0x80, 0x00, 0x01, 0xFF,
//		0x80, 0x00, 0x00, 0xFF,
//		0xC0, 0x00, 0x00, 0xFF,
//		0xC0, 0x00, 0x00, 0xFF,
//		0xE0, 0x00, 0x00, 0x7F,
//		0xE0, 0x00, 0x00, 0x7F,
//		0xE0, 0x00, 0x00, 0x7F,
//		0xF0, 0x00, 0x00, 0x3F,
//		0xFF, 0xFF, 0xFE, 0x3F,
//		0xFF, 0xFF, 0xFF, 0x1F,
//		0xFF, 0xFF, 0xFF, 0x1F,
//		0xFF, 0xFF, 0xFF, 0x9F,
//		0xFF, 0xFF, 0xFC, 0x0F,
//		0xFF, 0xFF, 0x00, 0x0F,
//		0xFF, 0xF8, 0x00, 0x0F,
//		0xFF, 0xE0, 0x00, 0x07,
//		0xFF, 0x80, 0x00, 0x07,
//		0xFF, 0x00, 0x00, 0x03,
//		0xFC, 0x00, 0x00, 0x03,
//		0xF8, 0x00, 0x00, 0x03,
//		0xF0, 0x00, 0x00, 0x01,
//		0xF0, 0x00, 0x00, 0x01,
//		0xE0, 0x00, 0x07, 0xFF,
//		0xC0, 0x00, 0x3F, 0xFF,
//		0xC0, 0x00, 0xFF, 0xFF,
//		0x80, 0x03, 0xFF, 0xFF,
//		0x80, 0x07, 0xFF, 0xFF,
//		0x80, 0x0F, 0xFF, 0xFF,
//		0x80, 0x1F, 0xFF, 0xFF,
//		0x80, 0x3F, 0xFF, 0xFF,
//		0x80, 0x7F, 0xFF, 0xFF,
//		0x80, 0xFF, 0xFF, 0xFF,
//		0x80, 0xFF, 0xFF, 0xFF,
//		0x81, 0xFF, 0xFF, 0xFF,
//		0x81, 0xFF, 0xFF, 0xFF,
//		0x81, 0xFF, 0xFF, 0xFF,
//		0x83, 0xFF, 0xFF, 0xFF,
//		0x83, 0xFF, 0xFF, 0xFF,
//		0xC3, 0xFF, 0xFF, 0xFF,
//		0xC3, 0xFF, 0xFF, 0xC7,
//		0xE3, 0xFF, 0xFF, 0x03,
//		0xE3, 0xFF, 0xFE, 0x01,
//		0xF1, 0xFF, 0xF8, 0x01,
//		0xF1, 0xFF, 0xF0, 0x01,
//		0xF8, 0xFF, 0xE0, 0x01,
//		0xFE, 0x7F, 0xC0, 0x03,
//		0xFF, 0x3F, 0x00, 0x07,
//		0xFF, 0xE0, 0x00, 0x1F,
//		0xFF, 0xFF, 0x81, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0x8F, 0xFF, 0xFF, 0xFF,
//		0xE0, 0x3F, 0xFF, 0xFF,
//		0xFC, 0x07, 0xFF, 0xFF,
//		0xFF, 0x01, 0xFF, 0xFF,
//		0xFF, 0xC0, 0x7F, 0xFF,
//		0xFF, 0xE0, 0x1F, 0xFF,
//		0xFF, 0xF8, 0x0F, 0xFF,
//		0xFF, 0xFC, 0x03, 0xFF,
//		0xFF, 0xFE, 0x01, 0xFF,
//		0xFF, 0xFF, 0x00, 0xFF,
//		0xFF, 0xFF, 0x00, 0x7F,
//		0xFF, 0xFF, 0x80, 0x3F,
//		0xFF, 0xFF, 0xC0, 0x3F,
//		0xFF, 0xFF, 0xC0, 0x1F,
//		0xFF, 0xFF, 0xE0, 0x0F,
//		0xFF, 0xFF, 0xE0, 0x0F,
//		0xFF, 0xFF, 0xE0, 0x07,
//		0xFF, 0xFF, 0xF0, 0x07,
//		0xFF, 0xFF, 0xF0, 0x03,
//		0xFF, 0xFF, 0xF0, 0x03,
//		0xFF, 0xFF, 0xF8, 0x01,
//		0xFF, 0xE0, 0x78, 0x01,
//		0xFF, 0x00, 0x00, 0x01,
//		0xFC, 0x01, 0xF0, 0x01,
//		0xF8, 0x03, 0xFC, 0x01,
//		0xF0, 0x07, 0xFC, 0x00,
//		0xE0, 0x0F, 0xFC, 0x00,
//		0xE0, 0x0F, 0xFC, 0x00,
//		0xC0, 0x0F, 0xFC, 0x00,
//		0xC0, 0x0F, 0xFC, 0x00,
//		0xC0, 0x1F, 0xFC, 0x00,
//		0x80, 0x1F, 0xFC, 0x00,
//		0x80, 0x1F, 0xFC, 0x00,
//		0x80, 0x1F, 0xFC, 0x00,
//		0x80, 0x1F, 0xFC, 0x00,
//		0x80, 0x1F, 0xFC, 0x00,
//		0x80, 0x1F, 0xFC, 0x01,
//		0x80, 0x1F, 0xF8, 0x01,
//		0x80, 0x1F, 0xF8, 0x01,
//		0xC0, 0x1F, 0xF8, 0x01,
//		0xC0, 0x1F, 0xF8, 0x03,
//		0xC0, 0x1F, 0xF8, 0x03,
//		0xE0, 0x1F, 0xF8, 0x07,
//		0xE0, 0x1F, 0xF0, 0x07,
//		0xF0, 0x1F, 0xF0, 0x0F,
//		0xF8, 0x1F, 0xF0, 0x1F,
//		0xF8, 0x0F, 0xE0, 0x3F,
//		0xFE, 0x0F, 0xC0, 0x7F,
//		0xFF, 0x07, 0x80, 0xFF,
//		0xFF, 0xC0, 0x03, 0xFF,
//		0xFF, 0xFC, 0x3F, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0x80, 0x00, 0x00, 0x07,
//		0x80, 0x00, 0x00, 0x07,
//		0x80, 0x00, 0x00, 0x07,
//		0xC0, 0x00, 0x00, 0x07,
//		0xC0, 0x00, 0x00, 0x03,
//		0xC0, 0x00, 0x00, 0x03,
//		0xE0, 0x00, 0x00, 0x03,
//		0xE0, 0x00, 0x00, 0x03,
//		0xE0, 0x00, 0x00, 0x01,
//		0xF0, 0x7F, 0xFF, 0x81,
//		0xF0, 0x7F, 0xFF, 0xF1,
//		0xF0, 0x7F, 0xFF, 0xF9,
//		0xF8, 0x3F, 0xFF, 0xFC,
//		0xF8, 0x3F, 0xFF, 0xFE,
//		0xF8, 0x3F, 0xFF, 0xFE,
//		0xFC, 0x1F, 0xFF, 0xFF,
//		0xFC, 0x1F, 0xFF, 0xFF,
//		0xFC, 0x1F, 0xFF, 0xFF,
//		0xFE, 0x0F, 0xFF, 0xFF,
//		0xFE, 0x0F, 0xFF, 0xFF,
//		0xFE, 0x0F, 0xFF, 0xFF,
//		0xFF, 0x07, 0xFF, 0xFF,
//		0xFF, 0x07, 0xFF, 0xFF,
//		0xFF, 0x07, 0xFF, 0xFF,
//		0xFF, 0x83, 0xFF, 0xFF,
//		0xFF, 0x83, 0xFF, 0xFF,
//		0xFF, 0x83, 0xFF, 0xFF,
//		0xFF, 0xC1, 0xFF, 0xFF,
//		0xFF, 0xC1, 0xFF, 0xFF,
//		0xFF, 0xC1, 0xFF, 0xFF,
//		0xFF, 0xE0, 0xFF, 0xFF,
//		0xFF, 0xE0, 0xFF, 0xFF,
//		0xFF, 0xE0, 0xFF, 0xFF,
//		0xFF, 0xF0, 0x7F, 0xFF,
//		0xFF, 0xF0, 0x7F, 0xFF,
//		0xFF, 0xF0, 0x7F, 0xFF,
//		0xFF, 0xF8, 0x3F, 0xFF,
//		0xFF, 0xF8, 0x3F, 0xFF,
//		0xFF, 0xF8, 0x3F, 0xFF,
//		0xFF, 0xFC, 0x1F, 0xFF,
//		0xFF, 0xFC, 0x1F, 0xFF,
//		0xFF, 0xFC, 0x1F, 0xFF,
//		0xFF, 0xFE, 0x0F, 0xFF,
//		0xFF, 0xFE, 0x0F, 0xFF,
//		0xFF, 0xFE, 0x0F, 0xFF,
//		0xFF, 0xFF, 0x07, 0xFF,
//		0xFF, 0xFF, 0x07, 0xFF,
//		0xFF, 0xFF, 0x07, 0xFF,
//		0xFF, 0xFF, 0x83, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF };
//
//
//uint8_t mat_for_symbol3[1024] = {0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xF0, 0x3F, 0xFF,
//		0xFF, 0x00, 0x01, 0xFF,
//		0xFC, 0x07, 0xC0, 0x7F,
//		0xF8, 0x0F, 0xF0, 0x3F,
//		0xF0, 0x1F, 0xF0, 0x1F,
//		0xE0, 0x3F, 0xF8, 0x0F,
//		0xC0, 0x3F, 0xF8, 0x07,
//		0xC0, 0x7F, 0xFC, 0x07,
//		0xC0, 0x7F, 0xFC, 0x03,
//		0x80, 0x7F, 0xF8, 0x03,
//		0x80, 0x7F, 0xF8, 0x03,
//		0x80, 0x7F, 0xF8, 0x03,
//		0x80, 0x7F, 0xF0, 0x03,
//		0xC0, 0x7F, 0xF0, 0x03,
//		0xC0, 0x7F, 0xE0, 0x03,
//		0xC0, 0x3F, 0xC0, 0x03,
//		0xE0, 0x3F, 0x80, 0x03,
//		0xE0, 0x3F, 0x00, 0x03,
//		0xF0, 0x1E, 0x00, 0x07,
//		0xF8, 0x1C, 0x00, 0x07,
//		0xFE, 0x00, 0x00, 0x0F,
//		0xFF, 0x00, 0x00, 0x1F,
//		0xFF, 0x80, 0x00, 0x3F,
//		0xFF, 0x00, 0x00, 0x7F,
//		0xFE, 0x00, 0x00, 0xFF,
//		0xFC, 0x00, 0x03, 0xFF,
//		0xF8, 0x00, 0x03, 0xFF,
//		0xF0, 0x00, 0x00, 0xFF,
//		0xE0, 0x00, 0x30, 0x7F,
//		0xC0, 0x00, 0xF8, 0x1F,
//		0xC0, 0x01, 0xF8, 0x0F,
//		0x80, 0x03, 0xFC, 0x07,
//		0x80, 0x07, 0xFC, 0x03,
//		0x80, 0x0F, 0xFC, 0x03,
//		0x80, 0x0F, 0xFC, 0x01,
//		0x80, 0x1F, 0xFE, 0x01,
//		0x80, 0x1F, 0xFE, 0x01,
//		0x80, 0x3F, 0xFE, 0x01,
//		0x80, 0x3F, 0xFE, 0x01,
//		0x80, 0x3F, 0xFE, 0x01,
//		0x80, 0x3F, 0xFE, 0x01,
//		0x80, 0x7F, 0xFE, 0x01,
//		0xC0, 0x3F, 0xFC, 0x01,
//		0xC0, 0x3F, 0xFC, 0x03,
//		0xE0, 0x3F, 0xFC, 0x03,
//		0xF0, 0x3F, 0xFC, 0x07,
//		0xF0, 0x1F, 0xF8, 0x0F,
//		0xFC, 0x0F, 0xF0, 0x1F,
//		0xFE, 0x07, 0xE0, 0x3F,
//		0xFF, 0x80, 0x00, 0xFF,
//		0xFF, 0xFC, 0x1F, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFC, 0x1F, 0xFF,
//		0xFF, 0xE0, 0x03, 0xFF,
//		0xFF, 0x81, 0xE0, 0xFF,
//		0xFE, 0x03, 0xF0, 0x3F,
//		0xFC, 0x07, 0xF8, 0x1F,
//		0xF8, 0x07, 0xF8, 0x0F,
//		0xF8, 0x07, 0xF8, 0x07,
//		0xF0, 0x0F, 0xFC, 0x07,
//		0xE0, 0x0F, 0xFC, 0x03,
//		0xE0, 0x0F, 0xFC, 0x03,
//		0xC0, 0x0F, 0xFC, 0x01,
//		0xC0, 0x1F, 0xFC, 0x01,
//		0xC0, 0x1F, 0xFC, 0x01,
//		0x80, 0x1F, 0xFC, 0x01,
//		0x80, 0x1F, 0xFC, 0x01,
//		0x80, 0x1F, 0xFC, 0x01,
//		0x80, 0x1F, 0xFC, 0x01,
//		0x80, 0x1F, 0xFC, 0x01,
//		0x80, 0x1F, 0xF8, 0x01,
//		0x80, 0x1F, 0xF8, 0x01,
//		0x80, 0x1F, 0xF8, 0x01,
//		0x80, 0x1F, 0xF8, 0x01,
//		0x80, 0x1F, 0xF8, 0x03,
//		0x80, 0x1F, 0xF0, 0x03,
//		0x80, 0x1F, 0xF0, 0x07,
//		0x80, 0x1F, 0xE0, 0x0F,
//		0x80, 0x1F, 0xE0, 0x1F,
//		0x80, 0x0F, 0xC0, 0x3F,
//		0xC0, 0x00, 0x00, 0xFF,
//		0xC0, 0x0F, 0x07, 0xFF,
//		0xC0, 0x0F, 0xFF, 0xFF,
//		0xE0, 0x0F, 0xFF, 0xFF,
//		0xE0, 0x0F, 0xFF, 0xFF,
//		0xE0, 0x07, 0xFF, 0xFF,
//		0xF0, 0x07, 0xFF, 0xFF,
//		0xF0, 0x07, 0xFF, 0xFF,
//		0xF8, 0x03, 0xFF, 0xFF,
//		0xFC, 0x03, 0xFF, 0xFF,
//		0xFC, 0x01, 0xFF, 0xFF,
//		0xFE, 0x01, 0xFF, 0xFF,
//		0xFF, 0x00, 0xFF, 0xFF,
//		0xFF, 0x80, 0x7F, 0xFF,
//		0xFF, 0xC0, 0x3F, 0xFF,
//		0xFF, 0xE0, 0x1F, 0xFF,
//		0xFF, 0xF0, 0x0F, 0xFF,
//		0xFF, 0xFC, 0x07, 0xFF,
//		0xFF, 0xFE, 0x01, 0xFF,
//		0xFF, 0xFF, 0x80, 0xFF,
//		0xFF, 0xFF, 0xF0, 0x3F,
//		0xFF, 0xFF, 0xFE, 0x07,
//		0xFF, 0xFF, 0xFF, 0xF9,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0x81, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xDF, 0xF0, 0x00, 0x07,
//		0xFF, 0xFF, 0xF8, 0x3F,
//		0xCF, 0x83, 0xFE, 0x01,
//		0xFF, 0xFF, 0xE4, 0x0F,
//		0xC0, 0x1F, 0xFF, 0x80,
//		0x7F, 0xFF, 0xC7, 0xC7,
//		0xC0, 0x3F, 0xFF, 0xC0,
//		0x1F, 0xFF, 0xCF, 0xE7,
//		0xC0, 0xFF, 0xFF, 0xF0,
//		0x0F, 0xFF, 0x8F, 0xE3,
//		0xC1, 0xFF, 0xFF, 0xF8,
//		0x03, 0xFF, 0x8F, 0xE3,
//		0xC1, 0xFF, 0xFF, 0xFC,
//		0x01, 0xFF, 0x8F, 0xE3,
//		0xC3, 0xFF, 0xFF, 0xFC,
//		0x00, 0xFF, 0x8F, 0xE3,
//		0xC7, 0xFF, 0xFF, 0xFE,
//		0x00, 0x7F, 0xCF, 0xE7,
//		0xCF, 0xFF, 0xFF, 0xFF,
//		0x00, 0x7F, 0xC7, 0xC7,
//		0xCF, 0xFF, 0xFF, 0xFF,
//		0x00, 0x3F, 0xE0, 0x0F,
//		0xCF, 0xFF, 0xFF, 0xFF,
//		0x00, 0x1F, 0xF8, 0x3F,
//		0xDF, 0xFF, 0xFF, 0xFF,
//		0x80, 0x1F, 0xFF, 0xFF,
//		0xDF, 0xFF, 0xFF, 0xFF,
//		0x80, 0x0F, 0xFF, 0xFF,
//		0xDF, 0xFF, 0xFF, 0xFF,
//		0x80, 0x0F, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xC0, 0x07, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xC0, 0x07, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xC0, 0x03, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xC0, 0x03, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xC0, 0x03, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xC0, 0x03, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xC0, 0x03, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xE0, 0x01, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xE0, 0x01, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xE0, 0x01, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xE0, 0x01, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xC0, 0x01, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xC0, 0x01, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xC0, 0x03, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xC0, 0x03, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xC0, 0x03, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xC0, 0x03, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xC0, 0x03, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xC0, 0x07, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0x80, 0x07, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0x80, 0x07, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0x80, 0x0F, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0x00, 0x0F, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0x00, 0x1F, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0x00, 0x3F, 0xFF, 0xFF,
//		0xDF, 0xFF, 0xFF, 0xFE,
//		0x00, 0x3F, 0xFF, 0xFF,
//		0xCF, 0xFF, 0xFF, 0xFE,
//		0x00, 0x7F, 0xFF, 0xFF,
//		0xC7, 0xFF, 0xFF, 0xFC,
//		0x00, 0xFF, 0xFF, 0xFF,
//		0xE3, 0xFF, 0xFF, 0xF8,
//		0x01, 0xFF, 0xFF, 0xFF,
//		0xF1, 0xFF, 0xFF, 0xF0,
//		0x03, 0xFF, 0xFF, 0xFF,
//		0xFC, 0x7F, 0xFF, 0xE0,
//		0x0F, 0xFF, 0xFF, 0xFF,
//		0xFE, 0x1F, 0xFF, 0xC0,
//		0x1F, 0xFF, 0xFF, 0xFF,
//		0xFF, 0x87, 0xFE, 0x00,
//		0x7F, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xF0, 0x00, 0x03,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0x80, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF};
//
//
//uint8_t mat_for_symbol4[256] = { 0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0x1F, 0x01, 0x80, 0x60,
//		0x08, 0x02, 0x00, 0x80,
//		0x70, 0x1F, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFE, 0x00, 0x00,
//		0x38, 0x00, 0x00, 0xE0,
//		0x00, 0x03, 0x80, 0x00,
//		0x0E, 0x00, 0x00, 0x38,
//		0x00, 0x00, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0xFF };
