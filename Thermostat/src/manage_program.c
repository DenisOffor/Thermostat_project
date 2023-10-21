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
	init_ds();
	TFT_init();
	TFT_reset_temperature();
	init_USART();
	init_GPIO();
	ADC_init();
	display_temperature(temperatures.curr_temperature, CURRENT_TEMP);
	display_temperature(temperatures.curr_temperature, AIM_TEMP);
}

void DS18B20_measure_temperature() {
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
			UART_send_temperature(symbols_distribution.char_output, symbols_distribution.amout_of_symbols - 1, DS18B20_ADDRESS);
			program_task = START;
			break;
	}
};

void NTC_measurment() {
	if(program_task == TURN_OFF)
		return;
	if(cycle_start == 1){
		Ntc_R = ((NTC_UP_R)/((4095.0/ADC_Raw[0]) - 1));
		double Ntc_Ln = log(Ntc_R);
		Ntc_Tmp = (1.0/(A + B*Ntc_Ln + C*Ntc_Ln*Ntc_Ln*Ntc_Ln)) - 273.15;
		display_temperature(Ntc_Tmp, NTC_TEMP);
		UART_send_temperature(symbols_distribution.char_output, symbols_distribution.amout_of_symbols - 1, NTC_ADDRESS);
		cycle_start = 0;
	}
}

void check_UART_cmd() {
	if(rx_data_state == DATA_WAITING)
		return;
	switch(UART_rx_buf[0]) {
		case UART_CMD_TURN_OFF:
			program_task = TURN_OFF;
			TFT_reset_temperature();
			rx_data_state = DATA_WAITING;
			break;
		case UART_CMD_TURN_ON:
			program_task = START;
			regulate_status = WAITING;
			display_temperature(temperatures.aim_temperature, AIM_TEMP);
			rx_data_state = DATA_WAITING;
			break;
		case UART_CMD_SET_AIM_TEMP:
			temperatures.aim_temperature = UART_rx_buf[1];

			if(temperatures.aim_temperature > 120)
				temperatures.aim_temperature = temperatures.aim_temperature - 256;
			display_temperature(temperatures.aim_temperature, AIM_TEMP);

			if(temperatures.aim_temperature > temperatures.curr_temperature)
				regulate_status = HEATING;

			rx_data_state = DATA_WAITING;
			break;
		case UART_CMD_HEAT_DURING:
			regulate_status = HEATING_DURING_TIME;
			//amount of second
			TIM6->ARR = UART_rx_buf[1] * 1000;

			rx_data_state = DATA_WAITING;
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
