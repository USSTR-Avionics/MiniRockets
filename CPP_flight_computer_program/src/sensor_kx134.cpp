#include "SparkFun_Qwiic_KX13X.h"

QwiicKX134 kxAccel; 
outputData accel_data;

#define g 9.8067f

bool init_kx134()
    {
    if( !kxAccel.begin() )
        {
        return 0;
        }
    else if( !kxAccel.initialize(DEFAULT_SETTINGS))
        { // Loading default settings.
        return 0;
        }
    else
        {
        // kxAccel.setRange(KX132_RANGE16G);
        // kxAccel.setRange(KX134_RANGE32G); // For a larger range uncomment
        return 1;
        }
    }

float get_kx134_accel_x()
    {
    accel_data = kxAccel.getAccelData(); 
    return accel_data.xData * g; // g * 9.8 => raw accel in ms^-2
    }

float get_kx134_accel_y()
    {
    accel_data = kxAccel.getAccelData(); 
    return accel_data.yData * g; 
    }

float get_kx134_accel_z()
    {
    accel_data = kxAccel.getAccelData(); 
    return accel_data.zData * g;
    }