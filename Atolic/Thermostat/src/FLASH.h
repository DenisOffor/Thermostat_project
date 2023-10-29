/*
 * FLASH.h
 *
 *  Created on: 11 мая 2023 г.
 *      Author: denlo
 */

#ifndef FLASH_H_
#define FLASH_H_

#include "stm32f0xx.h"

#define PAGE59 (uint32_t)0x0800DC00

#define AMOUNT_OF_BYTE_ON_PAGES_44_55 960

#define FLASH_PAGE_SIZE (uint32_t)0x00000400

#define PAGE44 (uint32_t)0x0800B000 //page 44
#define PAGE45 (uint32_t)0x0800B400 //page 45
#define PAGE46 (uint32_t)0x0800B800 //page 46
#define PAGE47 (uint32_t)0x0800BC00 //page 47

#define PAGE48 (uint32_t)0x0800C000 //page 48
#define PAGE49 (uint32_t)0x0800C400 //page 49
#define PAGE50 (uint32_t)0x0800C800 //page 50
#define PAGE51 (uint32_t)0x0800CC00 //page 51

#define PAGE52 (uint32_t)0x0800D000 //page 52
#define PAGE53 (uint32_t)0x0800D400 //page 53
#define PAGE54 (uint32_t)0x0800D800 //page 54
#define PAGE55 (uint32_t)0x0800DC00 //page 55

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
