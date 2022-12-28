#define LIFTOFF_THRESHOLD 11.0f // confirm with propulsion, nominal is 8.5 to 10.5
#define DESCENT_CHECK_AMOUNT 2  
#define ALTITUDE_CHANGE 0.1
#define PARACHUTE_DEPLOYMENT_HEIGHT 0.2
#define LANDING_ALTITUDE -0.2f

int debug_mode = false;

int test_mode = true;
#define STATE_TO_ENTER 1

/*
TEST MODE:
#define LIFTOFF_THRESHOLD 11.0f // confirm with propulsion, nominal is 8.5 to 10.5
#define DESCENT_CHECK_AMOUNT 3  
#define ALTITUDE_CHANGE 0.1
#define PARACHUTE_DEPLOYMENT_HEIGHT 0.2
#define LANDING_ALTITUDE 0.0f

int debug_mode = false;

int test_mode = true;
#define STATE_TO_ENTER 7
*/

/*
MINI-ROCKET MODE:
#define LIFTOFF_THRESHOLD 12.0f // confirm with propulsion, nominal is 8.5 to 10.5
#define DESCENT_CHECK_AMOUNT 2  
#define ALTITUDE_CHANGE 1
#define PARACHUTE_DEPLOYMENT_HEIGHT 50
#define LANDING_ALTITUDE 2.0f

int debug_mode = false;

int test_mode = false;
#define STATE_TO_ENTER 7
*/