
#include "manage_program.h"

int main(void)
{
	init_periphery();
	while (1)
	{
		check_UART_cmd();
		process_cmd();
		Relay_regulating();
		NTC_measurment();
	}
}
