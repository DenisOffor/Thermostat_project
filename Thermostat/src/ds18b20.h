/*
 * ds18b20.h
 *
 *  Created on: 30 сент. 2023 г.
 *      Author: denlo
 */

#ifndef DS18B20_H_
#define DS18B20_H_

#include "constant.h"

#define PORT_DS18B20 GPIOB
#define RCC_PORT_DS18B20 RCC_AHBENR_GPIOBEN
#define GPIO_MODER_DS18B20 GPIO_MODER_MODER9_0
#define GPIO_OTYPER_DS18B20 GPIO_OTYPER_OT_9
#define PIN_DS18B20 9
#define SKIP_ROM_ADR 0xCC
#define CONVERT_TEMP 0x44
#define READ_DATA_COMAND 0xBE

uint8_t ds_buff[9];
uint16_t temp;

void TIM2_IRQHandler();
void init_ds();
void init_Gpio_for_ds();
void init_tim3_for_us();
void init_tim2_for_delay();
uint8_t ds_reset_pulse();
void ds_write_bit(uint8_t bit);
uint8_t ds_read_bit();
void ds_write_byte(uint8_t byte);
uint8_t ds_read_byte();
void TIM_on_1sec();
void temperature_measurment_start();
void temprepature_measurment_read();

#endif /* DS18B20_H_ */
