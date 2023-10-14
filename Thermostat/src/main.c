
#include "TFT_display_temperature.h"
#include "ds18b20.h"
#include "init_TEC_throght_relay.h"
#include "UART_for_PC.h"

void init_clock();
void process_cmd();
void init_periphery();
void check_UART_cmd();

int main(void)
{
	init_periphery();
	init_USART();

	while (1)
	{
		check_UART_cmd();
		process_cmd();
		//Relay_regulating();

	}
}

void check_UART_cmd() {
	switch(UART_rx_buf[0]) {
	case 0x25:
		temperature_aim = UART_rx_buf[1];
		if(temperature_aim > 120)
			temperature_aim = temperature_aim - 256;
		break;
	}
}

void init_periphery() {
	//	Write_data_to_flash(PAGE60_FOR_0_1_2_3, &mat_for_symbol1[0], 1024);
	//	Write_data_to_flash(PAGE61_FOR_4_5_6_7, &mat_for_symbol2[0], 1024);
	//	Write_data_to_flash(PAGE62_FOR_8_9_CELSIUM, &mat_for_symbol3[0], 1024);
	//	Write_data_to_flash(PAGE63_FOR_DOT_MINUS, &mat_for_symbol4[0], 256);

	//init_GPIO();
	init_clock();
	init_ds();
	TFT_init();
}

void process_cmd() {
	switch(program_task) {
		case WAITING:
			temperature_measurment_start();
			program_task = TEMPERATURE_CONVERTING;
			break;
		case TEMPERATURE_READING:
			temprepature_measurment_read();
			break;
		case TEMPERATURE_DISPLAYING:
			display_temperature(temperature);
			DMA1_Channel4->CMAR = (uint32_t)(&symbol_distribution.char_output[0]);
			DMA1_Channel4->CNDTR = symbol_distribution.amout_of_symbols - 1;
			DMA1_Channel4->CCR |= DMA_CCR_EN;
			while(DMA1_Channel4->CCR & DMA_CCR_EN);
			break;
	}
};

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
