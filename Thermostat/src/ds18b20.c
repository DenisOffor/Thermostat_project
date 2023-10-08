/*
 * ds18b20.c
 *
 *  Created on: 30 сент. 2023 г.
 *      Author: denlo
 */

#include "ds18b20.h"

uint8_t ds_buff[9];
uint16_t temp;

void TIM2_IRQHandler() {
	TIM2->SR &= ~TIM_SR_UIF;
	TIM2->CR1 &= ~TIM_CR1_CEN;
	if(program_task == TEMPERATURE_CONVERTING)
		program_task = TEMPERATURE_READING;
}

void init_ds() {
	init_Gpio_for_ds();
	init_tim3_for_us();
	init_tim2_for_delay();
}

void init_Gpio_for_ds() {
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	GPIOB->MODER |= GPIO_MODER_MODER9_0;
	GPIOB->OTYPER |= GPIO_OTYPER_OT_9;
	PORT->ODR |= GPIO_ODR_9;
}

void init_tim3_for_us() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->ARR = 1000;
	TIM3->PSC = 8 * FREQ_MULTIPLIER_COEF;
	TIM3->CR1 = TIM_CR1_CEN;
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

uint8_t ds_reset_pulse(uint16_t PinMask)
{
   uint8_t result;
   if((PORT->IDR & PinMask) == 0)
	   return 2;                     //проверить линию на отсутствие замыкани€
   PORT->ODR &= ~PinMask;            //пот€нуть шину к земле
   TIMER->CNT = 0;
   while(TIMER->CNT < 480) {};        //ждать 480 микросекунд
   PORT->ODR |= PinMask;            //отпустить шину
   while(TIMER->CNT < 550) {};        //ждать 70 микросекунд
   result = (PORT->IDR & PinMask) << PIN;     //прочитать шину
   while(TIMER->CNT < 960) {};        //дождатьс€ окончани€ инициализации
   if(result)
	   return 1;                     //датчик не обнаружен
   return 0;                         //датчик обнаружен
}


void ds_write_bit(uint8_t bit,uint16_t PinMask)
{
   TIMER->CNT = 0;
   PORT->ODR &= ~PinMask;        //пот€нуть шину к земле
   while(TIMER->CNT < 2) {};     //ждать 1 микросекунду
   if(bit)
	  PORT->ODR |=  PinMask;     //если передаем 1, то отпускаем шину
   while(TIMER->CNT < 60) {};    //задержка 60 микросекунд
   PORT->ODR |=  PinMask;        //отпускаем шину
}

uint8_t ds_read_bit(uint16_t PinMask)
{
   uint8_t result;
   TIMER->CNT=0;
   PORT->ODR &= ~PinMask;                  //пот€нуть шину к земле
   while(TIMER->CNT < 2) {};
   PORT->ODR |=  PinMask;                  //отпускаем шину
   while(TIMER->CNT < 15) {};              //задержка 15 микросекунд
   result = (PORT->IDR & PinMask) >> PIN;  //прочитать шину
   while(TIMER->CNT < 60 ) {};             //оставшеес€ врем€
   return result;                          //возвратить результат
}

void ds_write_byte(uint8_t byte, uint16_t PinMask)
{
   for(uint8_t i = 0; i < 8; i++)
	   ds_write_bit( byte & (1<<i), PinMask );
}

uint8_t ds_read_byte(uint16_t PinMask)
{
   uint8_t i,result = 0;
   for(i = 0; i < 8; i++)
	   result |= (ds_read_bit(PinMask) << i);
   return result;
}

void delay() {
	TIM2->CNT = 0;
	TIM2->CR1 = TIM_CR1_CEN;
}

void temperature_measurment_start() {
	ds_reset_pulse(1 << PIN);          //послать импульс сброса                                       /
	ds_write_byte(SKIP_ROM_ADR, 1 << PIN);//разрешить доступ к датчику не использу€ адрес
	ds_write_byte(CONVERT_TEMP, 1 << PIN);      //запустить преобразование
	delay();               //дать врем€ дл€ измерени€
}

void temprepature_measurment_read() {
	ds_reset_pulse(1 << PIN);          //послать импульс сброса
	ds_write_byte(SKIP_ROM_ADR, 1 << PIN);//разрешить доступ к датчику не использу€ адрес
	ds_write_byte(READ_DATA_COMAND, 1 << PIN);      //команда, заставл€юща€ датчик выдать 9 байт своей пам€ти
	for(int i = 0; i < 9; i++ )           //прочитать 9 байт в массив
		ds_buff[i] = ds_read_byte(1 << PIN);
	temp = ds_buff[1];
	temp = temp << 8;
	temp |= ds_buff[0];
	temperature = temp * 0.0625;
	program_task = TEMPERATURE_DISPLAYING;
}
