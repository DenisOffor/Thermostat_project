
#include "manage_program.h"


int main(void)
{
	init_periphery();

	while (1)
	{
		check_UART_cmd();
		Measure_temperature();
		Display_data();
		TemperatureRegulating();
	}
}
