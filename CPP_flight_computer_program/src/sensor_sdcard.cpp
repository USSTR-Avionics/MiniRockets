#include "sensor_sdcard.h"
#include "SD.h"

const int chipSelect = BUILTIN_SDCARD;

const File myFile;
const Sd2Card card;
const SdVolume volume;
const SdFile root;

auto init_SD() -> int
	{
	Serial.println("init_SD()");

	SD.begin(chipSelect);

	if (!SD.begin(chipSelect))
		{
		Serial.println("failed init_SD()");
		return EXIT_FAILURE;
		}

	return EXIT_SUCCESS;
	}

auto write_to_sd_card(int event_type, const char* str) -> int
	{
	File dataFile = nullptr;

	if (event_type == DATALOG)
		{
		dataFile = SD.open("datalog.txt", FILE_WRITE);
		}
	else if (event_type == EVENTLOG)
		{
		dataFile = SD.open("eventlog.txt", FILE_WRITE);
		}

	if (dataFile)
		{
		dataFile.println(str);
		dataFile.close();
		// Serial.println("write to SD card");
		return EXIT_SUCCESS;
		}
	else
		{
		Serial.println("write to file failed");
		dataFile.close();
		return EXIT_FAILURE;
		}
	}
