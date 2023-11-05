
#include "manage_program.h"



int main(void)
{
	init_periphery();

	while (1)
	{
		check_UART_cmd();
		Measure_temperature();
		Display_data();
		if(program_status != STATUS_TURN_OFF && pid_state == PID_ON)
				PID_regulation();
		//Relay_regulating();
	}
}
