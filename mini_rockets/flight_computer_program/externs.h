// KX134
float kx134_accel_x;
float kx134_accel_y;
float kx134_accel_z;

// FRAM
int offsetfram = 0;
int myIntsSize = 5;
int myInts[5];
int fullBit = 0;

// BNO-055
float bno055_accel_x;
float bno055_accel_y;
float bno055_accel_z;
float bno055_orient_x;
float bno055_orient_y;
float bno055_orient_z;
float bno055_gyro_x;
float bno055_gyro_y;
float bno055_gyro_z;
float bno055_linear_x;
float bno055_linear_y;
float bno055_linear_z;
float bno055_mag_x;
float bno055_mag_y;
float bno055_mag_z;
float bno055_gravity_x;
float bno055_gravity_y;
float bno055_gravity_z;
float bno055_ang_vel_x;
float bno055_ang_vel_y;
float bno055_ang_vel_z;
float bno055_euler_x;
float bno055_euler_y;
float bno055_euler_z;
float bno055_quat_w;
float bno055_quat_y;
float bno055_quat_x;
float bno055_quat_z;
float bno055_temp;
float bno055_calib_sys;
float bno055_calib_gyro;
float bno055_calib_accel;
float bno055_calib_mag;
float last_alt = 0;

// MS5611
uint32_t rawTemp;
uint32_t rawPressure;
double realTemperature;
long realPressure;
float absoluteAltitude;
float relativeAltitude;
double referencePressure;

// Kalman Filter
float kalmanAverage = 0;
float kalmanGain;
int startkalman = 0;

// Moving Average Filter
int iterationCount;
float val0;
float val1;
float val2;
float val3;
int numCounter = 0 ;
float currentVal = 0;
float movingAverageVal=0;

bool debug = false;

int currentState = 0;
// MODE: 0 = testing, 1 = mini rocket, 2 = big rocket
int mode = 0;
int ledActivate = 1;
int buzzerActivate = 1;
int MS5611Activate = 1;
int microSdActivate = 1;
int KX134Activate = 1;
int FRAMActivate = 1;
int TXActivate = 1;
int kalmanFilterActivate = 1;
int movingAverageFilterActivate = 1;
