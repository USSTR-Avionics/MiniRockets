#include "sensor_sdcard.h"
#include "statemachine.h"

void get_start_signal_from_ground_station(statemachine::e_rocket_state &rs)
    {
    // Await signal
    rs = statemachine::ground_idle; 
    write_to_sd_card(EVENTLOG, "received arming signal from ground stations");
    }