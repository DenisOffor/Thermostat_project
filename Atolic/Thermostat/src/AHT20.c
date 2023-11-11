/*
 * AHT20.c
 *
 *  Created on: 3 но€б. 2023 г.
 *      Author: denlo
 */


#include "AHT20.h"

uint8_t AHT_state = AHT_START_CONVERSATION;
uint8_t start_state = 0;

void I2C_for_AHT_init() {
	RCC->AHBENR |= RCC_AHBENR_GPIOFEN;
	GPIOF->MODER |= GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1;
	GPIOF->OTYPER |= GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7;
	GPIOF->PUPDR |= GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0;

	RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
	I2C2->CR1 &= ~I2C_CR1_PE;
	I2C2->CR1 &= ~I2C_CR1_ANFOFF;

	//I2C2 clk 40 MHz
	I2C2->TIMINGR = (uint32_t)0x00D09DE9;
	I2C2->CR1 |= I2C_CR1_PE;
}

void I2C_AHT_SendByte(uint8_t addr, uint8_t byte) {
	while((I2C2->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY);

	while((I2C2->ISR & I2C_ISR_TXE) != I2C_ISR_TXE);
	I2C2->CR2 = I2C_CR2_AUTOEND | addr << 1 | 1 << 16;
	I2C2->CR2 |= I2C_CR2_START;
	I2C2->TXDR = byte;
}

void I2C_AHT_Send_NBytes(uint8_t addr, uint8_t *data, uint8_t size) {
	while((I2C2->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY);
	I2C2->CR2 = I2C_CR2_AUTOEND | addr << 1 | size << 16;
	I2C2->CR2 |= I2C_CR2_START;

	for (uint8_t i = 0; i < size; i++)
	{
		while((I2C2->ISR & I2C_ISR_TXE) != I2C_ISR_TXE);
		I2C2->TXDR = data[i];
	}
}

void I2C_Get_Data(uint8_t addr, uint8_t *data, uint8_t size) {
	while((I2C2->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY);

	I2C2->CR2 = I2C_CR2_AUTOEND | addr << 1 | size << 16;
	I2C2->CR2 |= I2C_CR2_RD_WRN;
	I2C2->CR2 |= I2C_CR2_START;

	for (uint8_t i = 0; i < size;)
	{
		if ((I2C2->ISR & I2C_ISR_RXNE) == I2C_ISR_RXNE) {
			data[i++] = I2C2->RXDR;
		}
	}
}

void AHT_measure_temperature() {
	if(AHT_state == AHT_START_CONVERSATION) {
		uint8_t data[3] = {AHT_CMD_CONVERT_TEMPERATURE, 0x33, 0x00};
		I2C_AHT_Send_NBytes(AHT_ADDRESS, &data[0], 3);
		start_state = 1;
		return;
	}

	if(AHT_state == AHT_READ_CONVERSATION && start_state == 1) {
		uint8_t result[6];
		I2C_Get_Data(AHT_ADDRESS, &(result[0]), 6);
		float buffer = (((uint32_t)result[3] & 15) << 16) | ((uint32_t)result[4] << 8) | result[5];
		temperatures.cur_temperature_AHT = (float)(buffer * 200.00 / 1048576.00) - 50.00;
		AHT_state = AHT_START_CONVERSATION;
		start_state = 0;
	}
}
