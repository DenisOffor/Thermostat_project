
#include "manage_program.h"


int main(void)
{
	init_periphery();


	while (1)
	{
		check_UART_cmd();
		Display_data();
		Measure_temperature();
		if(program_status != STATUS_TURN_OFF && pid_state == PID_ON)
				PID_regulation();
		//Relay_regulating();
		//if(parcel_state == ALL_PARCEL_HERE)
		//	TFT_draw_plot();
	}
}
