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

#define UNARMED_STATE               0
#define GROUND_IDLE_STATE           1
#define POWERED_FLIGHT_STATE        2
#define UNPOWERED_FLIGHT_STATE      3
#define BALLISTIC_DESCENT_STATE     4
#define CHUTE_DESCENT_STATE         5
#define LAND_SAFE_STATE             6
#define TEST_STATE                  7


#endif  // STATEMACHINE_T_H