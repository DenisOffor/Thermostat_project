
#include "TFT_display_temperature.h"
#include "ds18b20.h"

void init_clock();

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
			break;
	}
};

uint8_t check = 1;

void TIM6_DAC_IRQHandler(void) {
	TIM6->SR &= ~TIM_SR_UIF;
	TIM6->CR1 &= ~TIM_CR1_CEN;
	//if the timer has counted up to 25ms then disable rattle check
	check = 1;
}

void EXTI0_1_IRQHandler(void) {
	if( (EXTI->PR & EXTI_PR_PIF0) == EXTI_PR_PIF0) {
		EXTI->PR |= EXTI_PR_PIF0;
		if(check == 1) {
			check = 0;
	//		GPIOC->ODR ^= GPIO_ODR_8;
			PORT->ODR ^= GPIO_ODR_7;
			TIM6->CR1 |= TIM_CR1_CEN;
		}
	}
}

void init_GPIO() {
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	GPIOB->MODER |= GPIO_MODER_MODER7_0;
	GPIOB->OTYPER |= GPIO_OTYPER_OT_7;
	PORT->ODR &= ~GPIO_ODR_7;
}

void init_GPIO_for_Button() {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN;
	//input mode on PA0
	GPIOA->MODER &= ~GPIO_MODER_MODER0;
	GPIOC->MODER |= GPIO_MODER_MODER8_0;
	GPIOC->ODR &= ~GPIO_ODR_8;
	GPIOC->PUPDR |= GPIO_PUPDR_PUPDR8_1;

	EXTI->IMR |= EXTI_IMR_IM0;	 //interrupt mask register
	EXTI->RTSR |= EXTI_RTSR_RT0; //rising trigger selection

	NVIC_EnableIRQ(EXTI0_1_IRQn);//tune NVIC for EXTI0
	NVIC_SetPriority(EXTI0_1_IRQn, 5);
}

void init_TIM6_for_rattle_eliminating () {
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	TIM6->ARR = 4000; //tune TIM6 for 25ms
	TIM6->PSC = 250;
	//interrupt on
	TIM6->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM6_DAC_IRQn);
	NVIC_SetPriority(TIM6_DAC_IRQn, 5);
	TIM6->CR1 |= TIM_CR1_CEN;
}

int main(void)
{
	init_GPIO();
	init_GPIO_for_Button();
	init_TIM6_for_rattle_eliminating ();

//	Write_data_to_flash(PAGE60_FOR_0_1_2_3, &mat_for_symbol1[0], 1024);
//	Write_data_to_flash(PAGE61_FOR_4_5_6_7, &mat_for_symbol2[0], 1024);
//	Write_data_to_flash(PAGE62_FOR_8_9_CELSIUM, &mat_for_symbol3[0], 1024);
//	Write_data_to_flash(PAGE63_FOR_DOT_MINUS, &mat_for_symbol4[0], 256);

	init_clock();
	init_ds();
	TFT_init();

	while (1)
	{
		process_cmd();
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
	//SystemCoreClockUpdate();
}
