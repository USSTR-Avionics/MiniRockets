#include "SD.h"

const int chipSelect = BUILTIN_SDCARD;

File myFile;
Sd2Card card;
SdVolume volume;
SdFile root;

int init_SD()
    {
    Serial.println("init_SD()");

    SD.begin(chipSelect);
    
    if (!SD.begin(chipSelect)) 
        {
        Serial.println("failed init_SD()");
        return EXIT_FAILURE;
        }

    // SD.remove("datalog.txt");

    return EXIT_SUCCESS;
    }

int write_to_sd_card(const char* str)
    {
    File dataFile = SD.open("datalog.txt", FILE_WRITE);

    if (dataFile) 
        {
        dataFile.println(str);
        dataFile.close();
        Serial.println("write to SD card");
        return EXIT_SUCCESS;
        }
    else 
        {
        Serial.println("write to file failed");
        dataFile.close();
        return EXIT_FAILURE;
        }
    }