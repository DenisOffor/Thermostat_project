/*
 * manage_program.c
 *
 *  Created on: 18 ���. 2023 �.
 *      Author: denlo
 */

#include "manage_program.h"

PROGRAM_STATUS program_status = STATUS_TURN_OFF;
uint8_t display_status = DISPLAY_TEMPERATURE;

Sensors_State sensors_state;
uint8_t send_temp_on_PC = 1;

void TIM15_IRQHandler() {
	TIM15->SR &= ~TIM_SR_UIF;
	send_temp_on_PC = 1;
	AHT_state = AHT_READ_CONVERSATION;
}

void TIM14_IRQHandler(void) {
	TIM14->SR &= ~TIM_SR_UIF;
	TIM14->CR1 &= ~TIM_CR1_CEN;
	pid_state = PID_ON;
}

void init_periphery() {
	//	Write_data_to_flash(PAGE60_FOR_0_1_2_3, &mat_for_symbol1[0], 1024);
	//	Write_data_to_flash(PAGE61_FOR_4_5_6_7, &mat_for_symbol2[0], 1024);
	//	Write_data_to_flash(PAGE62_FOR_8_9_CELSIUM, &mat_for_symbol3[0], 1024);
	//	Write_data_to_flash(PAGE63_FOR_DOT_MINUS, &mat_for_symbol4[0], 256);

	//clock initialize
	init_clock();

	//tempperature sensors initialize
	init_ds();
	NTC_init_periphery();
	I2C_for_AHT_init();

	TFT_init();
	//USART for connection with PC
	init_USART();
	//relay regulating
	init_periphery_relay_regulating();
	Constants_Relay_set(27,0.006,0.006,0.65,0.25);
	//pid regulating
	init_TIM3_for_PWM();
	init_tim14_for_3sec();
	set_Pid_Coef(300, 4, 10);
	//
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
				TIM3->CR1 |= TIM_CR1_CEN;
				TIM14->CR1 |= TIM_CR1_CEN;
				//regulate_status = HEATING;
			}
			TIM16_set_wait_time(9.f);
			break;

		case UART_CMD_HEAT_DURING:
			regulate_status = HEATING_DURING_TIME;
			TIM6_set_heat_time(UART_data_buf[0]);
			break;

		case CMD_SENSOR_CHOOSE:
			Set_sensors_state(UART_data_buf[0], UART_data_buf[1], UART_data_buf[2], UART_data_buf[3]);
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

	if(send_temp_on_PC == 1) {
		uint8_t temprerature_parcel[12];
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

		UART_send_temperature(temprerature_parcel, 16, SEND_TEMPERATURE);

		if(display_status == DISPLAY_TEMPERATURE) {
			display_temperature(temperatures.curr_temperature, CURRENT_TEMP);
			display_temperature(temperatures.aim_temperature, AIM_TEMP);
		}

		send_temp_on_PC = 0;
	}


	if(display_status == DISPLAY_GRAPH) {
		if(parcel_state == NOT_ALL_PARCEL_HERE)
			return;
		TFT_draw_plot();
		parcel_state = NOT_ALL_PARCEL_HERE;
		amount_of_got_parcel = 0;
	}
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
		default:
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

    pwmDutyCycle = pid_coef.Kp * errorCurrent + pid_coef.Ki * errorIntegral - pid_coef.Kd * errorDifferential;

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
    TIM14->CNT = 0;
    TIM14->CR1 |= TIM_CR1_CEN;
    uint8_t data[2] = {((pwmDutyCycle >> 8) & 0xFF), (pwmDutyCycle & 0xFF)};
	UART_send_temperature(&data[0], 2, PWM_ADDRESS);
}

void reset_all_var() {
	//reset sensors
	ds18b20_cmd = TEMPERATURE_CONVERTING;
	ADC_HAVE_DATA = 0;
	AHT_state = AHT_START_CONVERSATION;
	Clear_sensors_state();
	//reset display
	TFT_clearAllDisplay(0x00,0x00,0x00);
	TFT_reset_temperature();
	display_status = DISPLAY_TEMPERATURE;
	amount_of_got_parcel = 0;
	parcel_state = NOT_ALL_PARCEL_HERE;

	//PID reset
	TIM3->CNT = 0;
	TIM3->CR1 &= ~TIM_CR1_CEN;
	pid_state = PID_OFF;
	set_Pid_Coef(300, 4, 10);

	//relay reset
	Constants_Relay_set(27,0.006,0.006,0.65,0.25);
	regulate_status = WAITING;

	//UART reset
	rx_data_state = UART_DATA_WAITING;
	rx_received_cmd = 0;
	size_of_parcel = 0;
	send_temp_on_PC = 1;
	// if turn off was in the middle of graph transfer, then need to reset DMA channel for command receive
	DMA1_Channel5->CCR &= ~DMA_CCR_EN;
	DMA1_Channel5->CNDTR = 4;
	DMA1_Channel5->CCR |= DMA_CCR_EN;
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

void init_tim14_for_3sec() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;

	TIM14->ARR = 8000 * FREQ_MULTIPLIER_COEF;
	TIM14->PSC = 3000;

	TIM14->DIER |= TIM_DIER_UIE;

	NVIC_EnableIRQ(TIM14_IRQn);
	NVIC_SetPriority(TIM14_IRQn,5);

	TIM14->CR1 |= TIM_CR1_CEN;
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

