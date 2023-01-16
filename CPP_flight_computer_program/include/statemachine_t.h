#ifndef STATEMACHINE_T_H
#define STATEMACHINE_T_H

namespace statemachine_t
    {
    typedef enum state
        {
        unarmed,
        ground_idle,
        powered_flight,
        unpowered_flight,
        ballistic_descent,
        chute_descent,
        land_safe,
        test,
        } e_rocket_state;
    }

#endif  // STATEMACHINE_T_H