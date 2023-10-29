
#include "manage_program.h"

int main(void)
{
	init_periphery();


	while (1)
	{
		check_UART_cmd();
		//DS18B20_measure_temperature();
		//if(program_status != STATUS_TURN_OFF && pid_state == PID_ON)
			//	PID_regulation();
		//Relay_regulating();
		//NTC_measure_temperature();
	}
}
