/*
 * manage_program.c
 *
 *  Created on: 18 окт. 2023 г.
 *      Author: denlo
 */

#include "manage_program.h"

uint8_t send_temp_on_PC = 0;

void TIM15_IRQHandler() {
	TIM15->SR &= ~TIM_SR_UIF;
	send_temp_on_PC = 1;
	AHT_state = AHT_READ_CONVERSATION;
}

void init_periphery() {
	//clock initialize
	init_clock();

	//tempperature sensors initialize
	init_ds();
	NTC_init_periphery();
	I2C_for_AHT_init();

	//TFT initialization
	TFT_init();

	//USART for connection with PC
	init_USART();

	//relay regulating
	init_periphery_relay_regulating();

	//pid regulating
	init_PID_regulation();

	//service function for initial program states set
	init_TIM15_as_TRGO_for_ADC_and_1sec_timer();
	reset_all_var();
}

void check_UART_cmd() {
	if(rx_data_state == UART_DATA_WAITING || rx_data_state == UART_CMD_RECEIVED)
		return;

	switch(rx_received_cmd) {
		case UART_CMD_TURN_ON_OFF:
			if(UART_data_buf[0] == 0x00)
				program_status = STATUS_TURN_OFF;
			if(UART_data_buf[0] == 0x01)
				program_status = STATUS_START;
			reset_all_var();
			break;

		case UART_CMD_SET_AIM_TEMP:
			temperatures.aim_temperature = UART_data_buf[0];
			if(temperatures.aim_temperature > 120)
				temperatures.aim_temperature = temperatures.aim_temperature - 256;

			if(temperatures.aim_temperature > temperatures.curr_temperature) {
				PID_start();
				//regulate_status = HEATING;
			}
			TIM16_set_wait_time(9.f);
			break;

		case UART_CMD_REGULATE_MODE:
			regulate_mode = (UART_data_buf[0] == 0) ? FREE_CONTROL : (UART_data_buf[0] == 1) ? RELAY : PID;
			break;

		case UART_CMD_HEAT_DURING:
			program_status = STATUS_HEATING_DURING_TIME;
			TIM6_set_heat_time(UART_data_buf[0]);
			break;

		case CMD_SENSOR_CHOOSE:
			Set_sensors_state(UART_data_buf[0], UART_data_buf[1], UART_data_buf[2], UART_data_buf[3]);
			AHT_state = AHT_START_CONVERSATION;
			break;

		case UART_CMD_SET_RELAY_COEF:
			memcpy(&(constants_relay.room_temperature), &UART_data_buf[0], sizeof(float));
			memcpy(&(constants_relay.maintenance_coef), &UART_data_buf[4], sizeof(float));
			memcpy(&(constants_relay.heat_coef), &UART_data_buf[8], sizeof(float));
			memcpy(&(constants_relay.heat_for_1sec), &UART_data_buf[12], sizeof(float));
			memcpy(&(constants_relay.delta), &UART_data_buf[16], sizeof(float));
			break;

		case UART_CMD_SET_PID_COEF:
			set_Pid_Coef((uint16_t)(UART_data_buf[0] | (UART_data_buf[1] << 8)) , (uint16_t)(UART_data_buf[2] | (UART_data_buf[3] << 8)),
					(uint16_t)(UART_data_buf[4] | (UART_data_buf[5] << 8)));
			break;


		case UART_GET_GRAPH_ON_DISPLAY:
			TFT_picture_Wrire_in_FLASH(&UART_data_buf[0]);
			break;

		case CMD_DRAW_CHOOSE:
			if(UART_data_buf[0] == 0x01) {
				TFT_clearAllDisplay(0x00,0x00,0x00);
				amount_of_got_parcel = 0;
				parcel_state = NOT_ALL_PARCEL_HERE;
				display_status = DISPLAY_TEMPERATURE;
			}

			if(UART_data_buf[0] == 0x02)
				display_status = DISPLAY_GRAPH;
			break;

	}

	rx_received_cmd = 0;
	rx_data_state = UART_DATA_WAITING;
}

void Measure_temperature() {
	if(program_status == STATUS_TURN_OFF)
		return;

	if(sensors_state.DS_as_add_sensor)
		DS18B20_measure_temperature();
	if(sensors_state.NTC_as_add_sensor)
		NTC_measure_temperature();
	if(sensors_state.AHT_as_add_sensor)
		AHT_measure_temperature();

	if(sensors_state.main_sensor == 1)
		temperatures.curr_temperature = temperatures.cur_temperature_DS;
	if(sensors_state.main_sensor == 2)
		temperatures.curr_temperature = temperatures.cur_temperature_NTC;
	if(sensors_state.main_sensor == 3)
		temperatures.curr_temperature = temperatures.cur_temperature_AHT;
}

void Display_data() {
	if(program_status == STATUS_TURN_OFF)
		return;

	//each 1 sec send temperatures on PC and display temperature on TFT if it chosen
	if(send_temp_on_PC == 1) {
		uint8_t temprerature_parcel[12];
		Form_temperature_parcel(&temprerature_parcel[0]);
		UART_send_data_to_PC(temprerature_parcel, 16, SEND_TEMPERATURE);

		if(display_status == DISPLAY_TEMPERATURE) {
			display_temperature(temperatures.curr_temperature, CURRENT_TEMP);
			display_temperature(temperatures.aim_temperature, AIM_TEMP);
		}

		send_temp_on_PC = 0;
	}

	//else if graph chosen then temperature already sent, just need to display graph
	if(display_status == DISPLAY_GRAPH) {
		if(parcel_state == NOT_ALL_PARCEL_HERE)
			return;
		TFT_draw_plot();
		parcel_state = NOT_ALL_PARCEL_HERE;
		amount_of_got_parcel = 0;
	}
}

void TemperatureRegulating() {
	if(program_status == STATUS_TURN_OFF)
		return;

	switch(regulate_mode) {
		case FREE_CONTROL:
			if(program_status == STATUS_HEATING_DURING_TIME) {
				TIM6->CR1 |= TIM_CR1_CEN;
				relay_on();
			}
			break;
		case RELAY:
			Relay_regulating();
			break;
		case PID:
			if(pid_state == PID_OFF)
				return;
			PID_regulation();
		    uint8_t data[2] = {((pwmDutyCycle >> 8) & 0xFF), (pwmDutyCycle & 0xFF)};
			UART_send_data_to_PC(&data[0], 2, PWM_ADDRESS);
			break;
	}
}

void reset_all_var() {
	Reset_temperatures();
	regulate_mode = FREE_CONTROL;

	//reset sensors
	ds18b20_cmd = TEMPERATURE_CONVERTING;
	ADC_HAVE_DATA_FOR_NTC = 0;
	AHT_state = AHT_START_CONVERSATION;
	Clear_sensors_state();

	//reset display
	TFT_reset_program();

	//PID reset
	PID_reset();

	//relay reset
	Relay_reset();

	//UART reset
	UART_reset();

	send_temp_on_PC = 1;
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

	FLASH->ACR |= FLASH_ACR_LATENCY;
}

void Form_temperature_parcel(uint8_t* temprerature_parcel) {
	if(sensors_state.DS_as_add_sensor && temperatures.cur_temperature_DS != RESET_TEMPERATURE) {
    	char *ptr = (char*)&(temperatures.cur_temperature_DS);
    	for (size_t i = 0; i < sizeof(float); ++i)
    		temprerature_parcel[i] = ptr[i];
	}

	if(sensors_state.NTC_as_add_sensor && temperatures.cur_temperature_NTC != RESET_TEMPERATURE) {
    	char *ptr = (char*)&(temperatures.cur_temperature_NTC);
		for (size_t i = 4; i < sizeof(float)*2; ++i)
			temprerature_parcel[i] = ptr[i - 4];
	}

	if(sensors_state.AHT_as_add_sensor && temperatures.cur_temperature_AHT != RESET_TEMPERATURE) {
		char *ptr = (char*)&(temperatures.cur_temperature_AHT);
		for (size_t i = 8; i < sizeof(float)*3; ++i)
			temprerature_parcel[i] = ptr[i - 8];
	}

	temprerature_parcel[12] = sensors_state.main_sensor;
	temprerature_parcel[13] = temperatures.cur_temperature_DS == RESET_TEMPERATURE ? 0 : sensors_state.DS_as_add_sensor;
	temprerature_parcel[14] = temperatures.cur_temperature_NTC == RESET_TEMPERATURE ? 0 : sensors_state.NTC_as_add_sensor;
	temprerature_parcel[15] = temperatures.cur_temperature_AHT == RESET_TEMPERATURE ? 0 : sensors_state.AHT_as_add_sensor;
}

void Clear_sensors_state() {
	sensors_state.main_sensor = 1;
	sensors_state.DS_as_add_sensor = 1;
	sensors_state.NTC_as_add_sensor = 0;
	sensors_state.AHT_as_add_sensor = 0;
}

void Set_sensors_state(uint8_t main_sensor, uint8_t DS_as_add_sensor, uint8_t NTC_as_add_sensor, uint8_t AHT_as_add_sensor) {
	sensors_state.main_sensor = main_sensor;
	sensors_state.DS_as_add_sensor = DS_as_add_sensor;
	sensors_state.NTC_as_add_sensor = NTC_as_add_sensor;
	sensors_state.AHT_as_add_sensor = AHT_as_add_sensor;
}

void init_TIM15_as_TRGO_for_ADC_and_1sec_timer() {
	RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;

	TIM15->ARR = 8000 * FREQ_MULTIPLIER_COEF;
	TIM15->PSC = 1000;

	TIM15->CR2 |= TIM_CR2_MMS_1;
	TIM15->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM15_IRQn);
	NVIC_SetPriority(TIM15_IRQn, 1);

	TIM15->CR1 |= TIM_CR1_CEN;
}

