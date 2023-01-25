#include "statemachine_t.h"
#include "sensor_led.h"

statemachine_t::e_rocket_state enter_state(int state)
{
    switch(state) 
    {
        case 0:
            return statemachine_t::e_rocket_state::unarmed;
            break;
        case 1:
            return statemachine_t::e_rocket_state::ground_idle;
            break;
        case 2:
            return statemachine_t::e_rocket_state::powered_flight;
        case 3:
            return statemachine_t::e_rocket_state::unpowered_flight;
        case 4:
            return statemachine_t::e_rocket_state::ballistic_descent;
        case 5:
            return statemachine_t::e_rocket_state::chute_descent;
        case 6:
            return statemachine_t::e_rocket_state::land_safe;
        case 7:
            return statemachine_t::e_rocket_state::test;
        default:
            return statemachine_t::e_rocket_state::unarmed;
    }

}
void test_mode_state()
{

    return ;
}
