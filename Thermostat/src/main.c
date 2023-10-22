
#include "manage_program.h"

int main(void)
{
	init_periphery();
	init_TIM15_as_TRGO();

	while (1)
	{
		check_UART_cmd();
		DS18B20_measure_temperature();
		Relay_regulating();
		//NTC_measurment();
	}
}
