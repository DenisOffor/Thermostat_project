/*
 * ds18b20.c
 *
 *  Created on: 30 сент. 2023 г.
 *      Author: denlo
 */

#include "ds18b20.h"


DS18B20_CMD ds18b20_cmd = TEMPERATURE_CONVERTING;
uint8_t ds_buff[9];
uint16_t temp;

void TIM2_IRQHandler() {
	TIM2->SR &= ~TIM_SR_UIF;
	TIM2->CR1 &= ~TIM_CR1_CEN;
	ds18b20_cmd = TEMPERATURE_READING;
}

void DS18B20_measure_temperature() {
	switch(ds18b20_cmd) {
		case TEMPERATURE_CONVERTING:
			temperature_measurment_start();
			ds18b20_cmd = WAITING_1SEC;
			break;
		case TEMPERATURE_READING:
			temprepature_measurment_read();
			ds18b20_cmd = TEMPERATURE_CONVERTING;
			break;
		default:
			break;
	}
}

void init_ds() {
	init_Gpio_for_ds();
	init_tim1_for_us();
	init_tim2_for_delay();
}

void init_Gpio_for_ds() {
	RCC->AHBENR |= RCC_PORT_DS18B20;
	PORT_DS18B20->MODER |= GPIO_MODER_DS18B20;
	PORT_DS18B20->OTYPER |= GPIO_OTYPER_DS18B20;
	PORT_DS18B20->ODR |= (1 << PIN_DS18B20);
}

void init_tim1_for_us() {
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	TIM1->ARR = 1000;
	TIM1->PSC = 8 * FREQ_MULTIPLIER_COEF;
	TIM1->CR1 = TIM_CR1_CEN;
}

void init_tim2_for_delay() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->ARR = 8000;
	TIM2->PSC = 1000 * FREQ_MULTIPLIER_COEF;
	TIM2->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn, 1);
	TIM2->CR1 = TIM_CR1_CEN;
}

uint8_t ds_reset_pulse()
{
	uint8_t result;
	if((PORT_DS18B20->IDR & (1 << PIN_DS18B20)) == 0)
		return 2;                     //проверить линию на отсутствие замыкани€
	PORT_DS18B20->ODR &= ~(1 << PIN_DS18B20);            //пот€нуть шину к земле
	TIM1->CNT = 0;
	while(TIM1->CNT < 480) {};        //ждать 480 микросекунд
	PORT_DS18B20->ODR |= (1 << PIN_DS18B20);            //отпустить шину
	while(TIM1->CNT < 530) {};        //ждать 50 микросекунд
	result = (PORT_DS18B20->IDR & (1 << PIN_DS18B20)) << PIN_DS18B20;     //прочитать шину
	while(TIM1->CNT < 960) {};        //дождатьс€ окончани€ инициализации
	if(result)
	   return 1;                     //датчик не обнаружен
	return 0;                         //датчик обнаружен
}


void ds_write_bit(uint8_t bit)
{
	TIM1->CNT = 0;
    PORT_DS18B20->ODR &= ~(1 << PIN_DS18B20);        //пот€нуть шину к земле
    while(TIM1->CNT < 2) {};     //ждать 1 микросекунду
    if(bit)
	  PORT_DS18B20->ODR |=  (1 << PIN_DS18B20);     //если передаем 1, то отпускаем шину
    while(TIM1->CNT < 60) {};    //задержка 60 микросекунд
    PORT_DS18B20->ODR |=  (1 << PIN_DS18B20);        //отпускаем шину
}

void ds_write_byte(uint8_t byte)
{
	for(uint8_t i = 0; i < 8; i++)
		ds_write_bit( byte & (1<<i));
}

uint8_t ds_read_bit()
{
	uint8_t result;
	TIM1->CNT=0;
	PORT_DS18B20->ODR &= ~(1 << PIN_DS18B20);                  //пот€нуть шину к земле
	while(TIM1->CNT < 2) {};
	PORT_DS18B20->ODR |=  (1 << PIN_DS18B20);                  //отпускаем шину
	while(TIM1->CNT < 15) {};              //задержка 15 микросекунд
	result = (PORT_DS18B20->IDR & (1 << PIN_DS18B20)) >> PIN_DS18B20;  //прочитать шину
	while(TIM1->CNT < 60 ) {};             //оставшеес€ врем€
	return result;                          //возвратить результат
}

uint8_t ds_read_byte()
{
	uint8_t i,result = 0;
	for(i = 0; i < 8; i++)
		result |= (ds_read_bit() << i);
	return result;
}

void TIM_on_1sec() {
	TIM2->CNT = 0;
	TIM2->CR1 = TIM_CR1_CEN;
}

void temperature_measurment_start() {
	ds_reset_pulse(1 << PIN_DS18B20);          //послать импульс сброса                                       /
	ds_write_byte(SKIP_ROM_ADR);//разрешить доступ к датчику не использу€ адрес
	ds_write_byte(CONVERT_TEMP);      //запустить преобразование
	TIM_on_1sec();               //дать врем€ дл€ измерени€
}

void temprepature_measurment_read() {
	ds_reset_pulse();          //послать импульс сброса
	ds_write_byte(SKIP_ROM_ADR);//разрешить доступ к датчику не использу€ адрес
	ds_write_byte(READ_DATA_COMAND);      //команда, заставл€юща€ датчик выдать 9 байт своей пам€ти
	for(int i = 0; i < 9; i++ )           //прочитать 9 байт в массив
		ds_buff[i] = ds_read_byte();

	temp = ds_buff[1];
	temp = temp << 8;
	temp |= ds_buff[0];
	temperatures.cur_temperature_DS = temp * 0.0625;
}
