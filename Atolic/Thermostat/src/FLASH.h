/*
 * FLASH.h
 *
 *  Created on: 11 мая 2023 г.
 *      Author: denlo
 */

#ifndef FLASH_H_
#define FLASH_H_

#include "stm32f0xx.h"

#define FLASH_PAGE_SIZE 1024

#define PAGE60_FOR_0_1_2_3 (uint32_t)0x0800F000 //page 60
#define PAGE60_0_POSITION 0
#define SIZE_0_BYTE 256
#define PAGE60_1_POSITION 256
#define SIZE_1_BYTE 256
#define PAGE60_2_POSITION 512
#define SIZE_2_BYTE 256
#define PAGE60_3_POSITION 768
#define SIZE_3_BYTE 256

#define PAGE61_FOR_4_5_6_7 (uint32_t)0x0800F400 //page 61
#define PAGE61_4_POSITION 0
#define SIZE_4_BYTE 256
#define PAGE61_5_POSITION 256
#define SIZE_5_BYTE 256
#define PAGE61_6_POSITION 512
#define SIZE_6_BYTE 256
#define PAGE61_7_POSITION 768
#define SIZE_7_BYTE 256

#define PAGE62_FOR_8_9_CELSIUM (uint32_t)0x0800F800 //page 62
#define PAGE62_8_POSITION 0
#define SIZE_8_BYTE 256
#define PAGE62_9_POSITION 256
#define SIZE_9_BYTE 256
#define PAGE62_CELSIUM_POSITION 512
#define CELSIUM_SIZE_BYTE 512

#define PAGE63_FOR_DOT_MINUS (uint32_t)0x0800FC00 //page 63
#define PAGE63_DOT_POSITION 0
#define DOT_SIZE_BYTE 80
#define PAGE63_MINUS_POSITION 80
#define MINUSE_SIZE_BYTE 176

#define DIGIT_WIDTH 32
#define DIGIT_HEIGHT 64
#define CELSIUM_WIDTH 64
#define CELSIUM_HEIGHT 64
#define DOT_WIDTH 10
#define DOT_HEIGHT 64
#define MINUS_WIDTH 22
#define MINUS_HEIGHT 64

void Set_protection_of_flash();
void Flash_unlock();
void Erase_Page(uint32_t Page_for_erase);
void Write_data_to_flash(uint32_t Page_adress, uint8_t* data, uint16_t data_size);
void ReadFromFlash(uint32_t Page_adress, uint8_t* data, uint16_t data_size);


#endif /* FLASH_H_ */
