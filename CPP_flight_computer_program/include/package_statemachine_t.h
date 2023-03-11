#ifndef PACKAGE_STATEMACHINE_T_H
#define PACKAGE_STATEMACHINE_T_H

#include "statemachine_t.h"

statemachine_t::e_rocket_state set_current_state_for_statemachine(statemachine_t::e_rocket_state& rs, int state);
int get_current_state_for_statemachine(statemachine_t::e_rocket_state& rs);

#endif // PACKAGE_STATEMACHINE_T_H