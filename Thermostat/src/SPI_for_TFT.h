/*
 * SPI_for_TFT.h
 *
 *  Created on: 1 ���. 2023 �.
 *      Author: denlo
 */

#ifndef SPI_FOR_TFT_H_
#define SPI_FOR_TFT_H_

#include "constant.h"

#define PORT_SPI GPIOA
#define SPI_RCC_GPIO RCC_AHBENR_GPIOAEN
#define SPI_SCK_MODER GPIO_MODER_MODER5_1
#define SPI_MOSI_MODER GPIO_MODER_MODER7_1
#define SPI_CS_MODER GPIO_MODER_MODER4_0
#define TFT_DC_MODER GPIO_MODER_MODER6_0
#define TFT_RES_MODER GPIO_MODER_MODER3_0
#define PIN_MOSI 7
#define PIN_DC 6
#define PIN_SCK 5
#define PIN_CS 4
#define PIN_RST 3

void DMA1_Channel2_3_IRQHandler();
void init_GPIO_for_SPI();
void SPI1_Master_init(uint8_t am_bits_send);
void SPI1_setDataSize(uint8_t am_bits_send);
void SPI_cs_set();
void SPI_cs_clear();
void DMA_for_SPI_init();
void SPI1_SendDataDMA_2byteNTimes(uint16_t data, uint16_t count_word);
void SPI1_SendDataDMA(uint16_t* data, uint16_t count_word);
void SPI1_Send1byte(uint8_t* data, uint8_t count_byte);

#endif /* SPI_FOR_TFT_H_ */
