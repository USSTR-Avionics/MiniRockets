#ifndef STATEMACHINE_T_H
#define STATEMACHINE_T_H

namespace statemachine_t
	{
typedef enum state
{
	unarmed,           // 0
	ground_idle,       // 1
	powered_flight,    // 2
	unpowered_flight,  // 3
	ballistic_descent, // 4
	chute_descent,     // 5
	land_safe,         // 6
	soft_recovery,     // 7
	test_state,        // 8
} e_rocket_state;
	}

#define UNARMED_STATE           0
#define GROUND_IDLE_STATE       1
#define POWERED_FLIGHT_STATE    2
#define UNPOWERED_FLIGHT_STATE  3
#define BALLISTIC_DESCENT_STATE 4
#define CHUTE_DESCENT_STATE     5
#define LAND_SAFE_STATE         6
#define SOFT_RECOVERY_STATE     7
#define TEST_STATE              8


int get_current_state_for_statemachine(statemachine_t::e_rocket_state&);
statemachine_t::e_rocket_state set_current_state_for_statemachine(statemachine_t::e_rocket_state&, int);


#endif // STATEMACHINE_T_H
