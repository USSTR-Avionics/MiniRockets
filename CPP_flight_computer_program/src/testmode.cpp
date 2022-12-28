#include "statemachine.h"
#include "testmode.h"

statemachine::e_rocket_state enter_state()
{
    switch(STATE_TO_ENTER) 
    {
        case 0:
            return statemachine::e_rocket_state::unarmed;
            break;
        case 1:
            return statemachine::e_rocket_state::ground_idle;
            break;
        case 2:
            return statemachine::e_rocket_state::powered_flight;
        case 3:
            return statemachine::e_rocket_state::unpowered_flight;
        case 4:
            return statemachine::e_rocket_state::ballistic_descent;
        case 5:
            return statemachine::e_rocket_state::chute_descent;
        case 6:
            return statemachine::e_rocket_state::land_safe;
        default:
            return statemachine::e_rocket_state::unarmed;
    }

}
