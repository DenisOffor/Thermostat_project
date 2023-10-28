/*
 * manage_program.c
 *
 *  Created on: 18 окт. 2023 г.
 *      Author: denlo
 */

#include "manage_program.h"

PROGRAM_STATUS program_status = STATUS_TURN_OFF;

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
	init_periphery_relay_regulating();
	NTC_init_periphery();
	init_TIM3_for_PWM();
	Constants_Relay_set(27,0.006,0.006,0.65,0.25);
	reset_all_var();
}

void DS18B20_measure_temperature() {
	if(program_status == STATUS_TURN_OFF)
		return;
	switch(ds18b20_cmd) {
		case TEMPERATURE_CONVERTING:
			temperature_measurment_start();
			ds18b20_cmd = WAITING_1SEC;
			break;
		case TEMPERATURE_READING:
			temprepature_measurment_read();
			ds18b20_cmd = TEMPERATURE_DISPLAYING;
			break;
		case TEMPERATURE_DISPLAYING:
			display_temperature(temperatures.curr_temperature, CURRENT_TEMP);
			UART_send_temperature(symbols_distribution.char_output, symbols_distribution.amout_of_symbols - 1, DS18B20_ADDRESS);
			ds18b20_cmd = TEMPERATURE_CONVERTING;
			break;
	}
};

void NTC_measure_temperature() {
	if(program_status == STATUS_TURN_OFF)
		return;
	//if ADC not get value yet, return
	if(ADC_HAVE_DATA == 0)
		return;

	display_temperature(NTC_get_temperature(), NTC_TEMP);
	UART_send_temperature(symbols_distribution.char_output, symbols_distribution.amout_of_symbols - 1, NTC_ADDRESS);
}

void check_UART_cmd() {
	if(rx_data_state == UART_DATA_WAITING || rx_data_state == UART_CMD_RECEIVED)
		return;
	switch(rx_received_cmd) {
		case UART_CMD_TURN_OFF:
			program_status = STATUS_TURN_OFF;
			reset_all_var();
			break;
		case UART_CMD_TURN_ON:
			program_status = STATUS_START;
			reset_all_var();
			break;
		case UART_CMD_SET_AIM_TEMP:
			temperatures.aim_temperature = UART_data_buf[0];
			if(temperatures.aim_temperature > 120)
				temperatures.aim_temperature = temperatures.aim_temperature - 256;
			display_temperature(temperatures.aim_temperature, AIM_TEMP);
			if(temperatures.aim_temperature > temperatures.curr_temperature)
				TIM3->CR1 |= TIM_CR1_CEN;
				//regulate_status = HEATING;
			TIM16_set_wait_time(9.f);
			break;
		case UART_CMD_HEAT_DURING:
			regulate_status = HEATING_DURING_TIME;
			TIM6_set_heat_time(UART_data_buf[0]);
			break;
		case UART_CMD_SET_PID_COEF:
			set_Pid_Coef((uint16_t)(UART_data_buf[0] | (UART_data_buf[1] << 8)) , (uint16_t)(UART_data_buf[2] | (UART_data_buf[3] << 8)),
					(uint16_t)(UART_data_buf[4] | (UART_data_buf[5] << 8)));
			break;
		//case UART_DRAW_GRAPH_ON_DISPLAY:
			//TFT_DRAW_GRAPH(&UART_data_buf[0]);
			//break;
	}
	rx_received_cmd = 0;
	rx_data_state = UART_DATA_WAITING;
}

void Relay_regulating() {
	if(program_status == STATUS_TURN_OFF)
		return;

	double time_heat = 0;

	switch(regulate_status) {
		case WAITING:
			relay_off();
			break;
		case MAINTENANCE:
			if(temperatures.curr_temperature < (temperatures.aim_temperature -
						(constants_relay.delta - (temperatures.aim_temperature - constants_relay.room_temperature)*constants_relay.maintenance_coef) )) {

				TIM6->ARR = 1000 * (1 + (temperatures.aim_temperature - constants_relay.room_temperature)*5*constants_relay.maintenance_coef);
				relay_on();
				TIM6->CR1 |= TIM_CR1_CEN;
				regulate_status = PROCESS;
			}
			break;
		case HEATING:
			time_heat = (temperatures.aim_temperature - temperatures.curr_temperature)
					/ (constants_relay.heat_for_1sec - (temperatures.aim_temperature - constants_relay.room_temperature)*constants_relay.heat_coef);

			TIM6_set_heat_time(time_heat);
			relay_on();
			TIM6->CR1 |= TIM_CR1_CEN;
			regulate_status = PROCESS;

			break;
		case HEATING_DURING_TIME:
			TIM6->CR1 |= TIM_CR1_CEN;
			relay_on();
			break;
	}
}

void PID_regulation() {
	if(!(TIM3->CR1 & TIM_CR1_CEN))
		return;

    errorCurrent = temperatures.aim_temperature - temperatures.curr_temperature;

    if ((((pid_coef.Ki * errorIntegral) <= PID_DUTY_CYCLE_MAX) && (errorCurrent >= 0)) ||
        (((pid_coef.Ki * errorIntegral) >= PID_DUTY_CYCLE_MIN) && (errorCurrent < 0)))
    {
      errorIntegral += errorCurrent;
    }

    errorDifferential = (errorCurrent - errorPrevious);

    pwmDutyCycle = pid_coef.Kp * errorCurrent + pid_coef.Ki * errorIntegral + pid_coef.Kd * errorDifferential;

    if (pwmDutyCycle < PID_DUTY_CYCLE_MIN)
    {
      pwmDutyCycle = PID_DUTY_CYCLE_MIN;
    }

    if (pwmDutyCycle > PID_DUTY_CYCLE_MAX)
    {
      pwmDutyCycle = PID_DUTY_CYCLE_MAX;
    }

    errorPrevious = errorCurrent;

    pid_state = PID_OFF;
    uint8_t data[2] = {((pwmDutyCycle >> 8) & 0xFF), (pwmDutyCycle & 0xFF)};
	UART_send_temperature(&data[0], 2, PWM_ADDRESS);
}

void reset_all_var() {
	TFT_reset_temperature();
	TIM3->CR1 &= ~TIM_CR1_CEN;
	ds18b20_cmd = TEMPERATURE_CONVERTING;
	regulate_status = WAITING;
}

void init_clock() {
	if( (RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL) {
		RCC->CFGR &= ~RCC_CFGR_SW;
		while( (RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);
	}

	RCC->CR &= ~RCC_CR_PLLON;
	while( (RCC->CR & RCC_CR_PLLRDY) != 0 );
	RCC->CFGR = ( RCC->CFGR & (~RCC_CFGR_PLLMUL) ) | RCC_CFGR_PLLMUL10;
	RCC->CR |= RCC_CR_PLLON;
	while( (RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);

	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while( (RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL);
	SystemCoreClockUpdate();
}
