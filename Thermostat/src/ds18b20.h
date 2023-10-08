/*
 * ds18b20.h
 *
 *  Created on: 30 ����. 2023 �.
 *      Author: denlo
 */

#ifndef DS18B20_H_
#define DS18B20_H_

#include "constant.h"

#define PORT GPIOB   			//������� ����, � �������� ���������� �������
#define PIN 9
#define TIMER TIM3    			//������ ������, ������������ ��� ������������ ��������
#define SKIP_ROM_ADR 0xCC 		//������� ���������
#define CONVERT_TEMP 0x44 		//��������� �������������� �����������
#define READ_DATA_COMAND 0xBE   //������� ������ ����������� ������
#define TIME_FOR_CONVERT_TEMP 500 //ms

uint8_t ds_buff[9];
uint16_t temp;

void TIM2_IRQHandler();

void init_ds();

void init_Gpio_for_ds();

void init_tim3_for_us();

void init_tim2_for_delay();

uint8_t ds_reset_pulse(uint16_t PinMask);

void ds_write_bit(uint8_t bit,uint16_t PinMask);

uint8_t ds_read_bit(uint16_t PinMask);

void ds_write_byte(uint8_t byte, uint16_t PinMask);

uint8_t ds_read_byte(uint16_t PinMask);

void delay();

void temperature_measurment_start();

void temprepature_measurment_read();

#endif /* DS18B20_H_ */
