#pragma once

namespace statemachine
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
        } e_rocket_state;
    }