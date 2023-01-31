#ifndef USER_VARIABLES_H
#define USER_VARIABLES_H

#if TESTPROFILE == 1
    #define LIFTOFF_THRESHOLD               2.0f  // in ms^-2
#endif // TESTPROFILE

#define LIFTOFF_THRESHOLD                   11.0f  // in ms^-2
#define DESCENT_CHECK_AMOUNT                2  
#define ALTITUDE_CHANGE                     0.6f
#define PARACHUTE_DEPLOYMENT_HEIGHT         200.0f // in meters
#define LANDING_ALTITUDE                    1.0f   // in meters

#endif // USER_VARIABLES_H