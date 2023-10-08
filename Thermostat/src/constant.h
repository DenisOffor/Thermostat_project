/*
 * constant.h
 *
 *  Created on: 30 сент. 2023 г.
 *      Author: denlo
 */

#ifndef CONSTANT_H_
#define CONSTANT_H_

#include "stm32f0xx.h"
#include "buffer.h"
#include "FLASH.h"

#define PORT_SPI GPIOA
#define PA_MOSI 7
#define PA_DC 6
#define PA_SCK 5
#define PA_CS 4
#define PA_RST 1


//uint8_t mat_for_symbol1[1024];
//uint8_t mat_for_symbol2[1024];
//uint8_t mat_for_symbol3[1024];
//uint8_t mat_for_symbol4[256];


uint16_t color_mat[2048];
uint8_t mat_for_symbol[FLASH_PAGE_SIZE];
uint8_t buf_tx[BUFFER_SIZE];

typedef enum PROGRAM_TASK{
	waiting = 0,
	read_temp
} PROGRAM_TASK;

PROGRAM_TASK program_task;


#endif /* CONSTANT_H_ */
