/*
 * manage_program.c
 *
 *  Created on: 18 окт. 2023 г.
 *      Author: denlo
 */

#include "manage_program.h"

void init_periphery() {
	//	Write_data_to_flash(PAGE60_FOR_0_1_2_3, &mat_for_symbol1[0], 1024);
	//	Write_data_to_flash(PAGE61_FOR_4_5_6_7, &mat_for_symbol2[0], 1024);
	//	Write_data_to_flash(PAGE62_FOR_8_9_CELSIUM, &mat_for_symbol3[0], 1024);
	//	Write_data_to_flash(PAGE63_FOR_DOT_MINUS, &mat_for_symbol4[0], 256);

	init_clock();
	init_GPIO();
	init_ds();
	TFT_init();
	TFT_reset_temperature();
	init_USART();
	ADC_init();
	display_temperature(temperatures.curr_temperature, CURRENT_TEMP);
	display_temperature(temperatures.curr_temperature, AIM_TEMP);
}

void process_cmd() {
	switch(program_task) {
		case START:
			temperature_measurment_start();
			program_task = TEMPERATURE_CONVERTING;
			break;
		case TEMPERATURE_READING:
			temprepature_measurment_read();
			program_task = TEMPERATURE_DISPLAYING;
			break;
		case TEMPERATURE_DISPLAYING:
			display_temperature(temperatures.curr_temperature, CURRENT_TEMP);
			DMA1_Channel4->CMAR = (uint32_t)(&symbol_distribution.char_output[0]);
			DMA1_Channel4->CNDTR = symbol_distribution.amout_of_symbols - 1;
			DMA1_Channel4->CCR |= DMA_CCR_EN;
			while(DMA1_Channel4->CCR & DMA_CCR_EN);
			program_task = START;
			break;
	}
};

void check_UART_cmd() {
	switch(UART_rx_buf[0]) {
		case 0x10:
			program_task = TURN_OFF;
			TFT_reset_temperature();
			UART_rx_buf[0] = 0;
			break;
		case 0x11:
			program_task = START;
			regulate_status = WAITING;
			display_temperature(temperatures.aim_temperature, AIM_TEMP);
			UART_rx_buf[0] = 0;
			break;
		case 0x25:
			for(int i = 0; i < 3000; i++);
			temperatures.aim_temperature = UART_rx_buf[1];
			if(temperatures.aim_temperature > 120)
				temperatures.aim_temperature = temperatures.aim_temperature - 256;
			display_temperature(temperatures.aim_temperature, AIM_TEMP);

			if(temperatures.aim_temperature > temperatures.curr_temperature)
				regulate_status = HEATING;
			if(temperatures.aim_temperature < temperatures.curr_temperature)
				regulate_status = COOLING;

			UART_rx_buf[0] = 0;
			break;
		case 0x31:
			regulate_status = HEATING_DURING_TIME;
			//amount of second
			TIM6->ARR = UART_rx_buf[1] * 1000;
			UART_rx_buf[0] = 0;
			break;
	}
}

void init_clock() {
	//if PLL is SYSCLK
	if( (RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL) {
		//switch on HSI
		RCC->CFGR &= ~RCC_CFGR_SW;
		//wait until HSI isn't SYSCLK
		while( (RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);
	}

	//1.Disable the PLL by setting PLLON to 0.
	RCC->CR &= ~RCC_CR_PLLON;
	//2. Wait until PLLRDY is cleared. The PLL is now fully stopped.
	while( (RCC->CR & RCC_CR_PLLRDY) != 0 );
	//3. Change the desired parameter
	RCC->CFGR = ( RCC->CFGR & (~RCC_CFGR_PLLMUL) ) | RCC_CFGR_PLLMUL10;
	//4. Enable the PLL again by setting PLLON to 1.
	RCC->CR |= RCC_CR_PLLON;
	//5. Wait until PLLRDY is set.
	while( (RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);

	//switch on PLL as SYSCLK
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	//wait until PLL isn't SYSCLK
	while( (RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL);
	SystemCoreClockUpdate();
}
