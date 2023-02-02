#include "statemachine_t.h"

statemachine_t::e_rocket_state set_current_state_for_statemachine(statemachine_t::e_rocket_state& rs, int state)
	{
	switch (state)
		{
	case 0:
		rs = statemachine_t::e_rocket_state::unarmed;
		return statemachine_t::e_rocket_state::unarmed;
		break;
	case 1:
		rs = statemachine_t::e_rocket_state::ground_idle;
		return statemachine_t::e_rocket_state::ground_idle;
		break;
	case 2:
		rs = statemachine_t::e_rocket_state::powered_flight;
		return statemachine_t::e_rocket_state::powered_flight;
	case 3:
		rs = statemachine_t::e_rocket_state::unpowered_flight;
		return statemachine_t::e_rocket_state::unpowered_flight;
	case 4:
		rs = statemachine_t::e_rocket_state::ballistic_descent;
		return statemachine_t::e_rocket_state::ballistic_descent;
	case 5:
		return statemachine_t::e_rocket_state::chute_descent;
	case 6:
		rs = statemachine_t::e_rocket_state::land_safe;
		return statemachine_t::e_rocket_state::land_safe;
	case 7:
		rs = statemachine_t::e_rocket_state::test_state;
		return statemachine_t::e_rocket_state::test_state;
	default:
		rs = statemachine_t::e_rocket_state::unarmed;
		return statemachine_t::e_rocket_state::unarmed;
		}
	}

int get_current_state_for_statemachine(statemachine_t::e_rocket_state& rs)
	{
	return rs;
	}
