#include "sensor_sdcard.h"
#include "statemachine_t.h"

void get_start_signal_from_ground_station(statemachine_t::e_rocket_state& rs)
	{
	// TODO: write code to await a signal from radio!
	rs = statemachine_t::ground_idle;
	write_to_sd_card(EVENTLOG, "received arming signal from ground stations");
	}
