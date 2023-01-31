#ifndef ROCKET_PROFILE_H
#define ROCKET_PROFILE_H

// flight profile
#define _FLIGHT_MONTH                       9
#define _FLIGHT_TIME_HOUR                   11
#define _FLIGHT_TIME_MINUTES                3

// rocket params
#define ROCKET_HEIGHT 1 // in metres

#if TESTPROFILE == 1
    #define LIFTOFF_THRESHOLD               2.0f  // in ms^-2
#else
    #define LIFTOFF_THRESHOLD               11.0f  // in ms^-2
#endif // TESTPROFILE

#define DESCENT_CHECK_AMOUNT                2  
#define ALTITUDE_CHANGE                     0.6f
#define PARACHUTE_DEPLOYMENT_HEIGHT         200.0f // in meters
#define LANDING_ALTITUDE                    1.0f   // in meters

#endif // ROCKET_PROFILE_H