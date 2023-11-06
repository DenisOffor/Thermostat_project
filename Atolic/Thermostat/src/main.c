
#include "manage_program.h"

int main(void)
{
	init_periphery();
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER8_0;

	while (1)
	{
		check_UART_cmd();
		Measure_temperature();
		Display_data();
		TemperatureRegulating();
	}
}
