/*
 * SPI_for_TFT.c
 *
 *  Created on: 1 окт. 2023 г.
 *      Author: denlo
 */

#include "SPI_for_TFT.h"


void DMA1_Channel2_3_IRQHandler() {
	if (DMA1->ISR & DMA_ISR_TCIF3) {
		DMA1->IFCR |= DMA_IFCR_CTCIF3;
		status_dma_tx = 1;
		SPI_cs_set();
		DMA1_Channel3->CCR &= ~DMA_CCR_EN;
	}
}

void init_GPIO_for_SPI() {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	PORT_SPI->MODER |= SPI_SCK_MODER | SPI_MOSI_MODER;
	//AF0
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL5 | GPIO_AFRL_AFRL7);
	PORT_SPI->MODER |= TFT_DC_MODER | SPI_CS_MODER | TFT_RES_MODER;

	SPI_cs_set();
}

void SPI1_Master_init(uint8_t am_bits_send) {
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	SPI1->CR1 &= ~SPI_CR1_SPE;
	SPI1->CR1 |= SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI;
	uint16_t DataSize = 0;
    DataSize |= ((am_bits_send - 1) << SPI_CR2_DS_Pos);
	SPI1->CR2 = DataSize;
	DMA_for_SPI_init();
	SPI1->CR1 |= SPI_CR1_SPE;
}

void SPI1_setDataSize(uint8_t am_bits_send) {
	SPI1->CR1 &= ~SPI_CR1_SPE;
	uint16_t DataSize = SPI1->CR2 & ~SPI_CR2_DS;
	DataSize |= (am_bits_send - 1) << 8;
	SPI1->CR2 = DataSize;
	SPI1->CR1 |= SPI_CR1_SPE;
}

void SPI_cs_set() {
	PORT_SPI->ODR |= (1 << PA_CS);
}

void SPI_cs_clear() {
	PORT_SPI->ODR &= ~(1 << PA_CS);
}

void DMA_for_SPI_init() {
	RCC->AHBENR |= RCC_AHBENR_DMAEN;
	DMA1_Channel3->CMAR = (uint32_t)(&color_mat[0]);
	DMA1_Channel3->CPAR = (uint32_t)(&(SPI1->DR));
    DMA1_Channel3->CNDTR = 0;
	DMA1_Channel3->CCR |= DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_TCIE;

	SPI1->CR2 |= SPI_CR2_TXDMAEN;
	NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
	NVIC_SetPriority(DMA1_Channel2_3_IRQn, 5);
}

uint16_t status_dma_tx;
void spi1_SendDataDMA_2byteNTimes(uint16_t data, uint16_t count_word) {
	DMA1_Channel3->CCR &= ~DMA_CCR_EN;
	DMA1_Channel3->CCR |= DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0;
	DMA1_Channel3->CCR &= ~DMA_CCR_MINC;
	DMA1_Channel3->CMAR = (uint32_t)(&color_mat[0]);
	DMA1_Channel3->CNDTR = count_word;

	SPI1_setDataSize(16);
	color_mat[0] = data;

	SPI_cs_clear();
	for(int i = 0; i < 10; i++);
	DMA1_Channel3->CCR |= DMA_CCR_EN;
}

void spi1_SendDataDMA(uint16_t* data, uint16_t count_word) {
	DMA1_Channel3->CCR &= ~DMA_CCR_EN;
	DMA1_Channel3->CCR |= DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0;
	DMA1_Channel3->CCR |= DMA_CCR_MINC;
	DMA1_Channel3->CMAR = (uint32_t)(&color_mat[0]);
	DMA1_Channel3->CNDTR = count_word;

	SPI1_setDataSize(16);

	SPI_cs_clear();
	for(int i = 0; i < 10; i++);
	DMA1_Channel3->CCR |= DMA_CCR_EN;
}

void spi1_Send1byte(uint8_t* data, uint8_t count_byte) {
	SPI1_setDataSize(8);
	SPI_cs_clear();
	for(int i = 0; i < 10; i++);
	for( uint8_t i = 0; i < count_byte; i++) {
		while (!(SPI1->SR & SPI_SR_TXE)) {};
		*(uint8_t*)&(SPI1->DR) = data[i];
	}
	while ((SPI1->SR & SPI_SR_BSY)) {};
	SPI_cs_set();
}
