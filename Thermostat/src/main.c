
#include "TFT_display_temperature.h"
#include "ds18b20.h"

void init_clock();

void process_cmd() {
	switch(program_task) {
		case 0:
			temp_measur();
			break;
	}
};


int main(void)
{
//	Write_data_to_flash(PAGE60_FOR_0_1_2_3, &mat_for_symbol1[0], 1024);
//	Write_data_to_flash(PAGE61_FOR_4_5_6_7, &mat_for_symbol2[0], 1024);
//	Write_data_to_flash(PAGE62_FOR_8_9_CELSIUM, &mat_for_symbol3[0], 1024);
//	Write_data_to_flash(PAGE63_FOR_DOT_MINUS, &mat_for_symbol4[0], 256);
	init_clock();
	//init_ds();
	TFT_init();
	Choose_symbol_for_draw('c');
	for(int i = 0; i < 10000; i++);
	while (1)
	{

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
	RCC->CFGR = ( RCC->CFGR & (~RCC_CFGR_PLLMUL) ) | RCC_CFGR_PLLMUL2;
	//4. Enable the PLL again by setting PLLON to 1.
	RCC->CR |= RCC_CR_PLLON;
	//5. Wait until PLLRDY is set.
	while( (RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);

	//switch on PLL as SYSCLK
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	//wait until PLL isn't SYSCLK
	while( (RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL);
	//SystemCoreClockUpdate();
}
