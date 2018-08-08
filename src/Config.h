#pragma once

#include <Arduino.h>
#include "config/Main.h"

struct config_t {
	uint8_t seed;					//sizeof = 1
	char version[5];				//sizeof = 5
	uint16_t firstEntry;			//sizeof = 2
	uint16_t lastEntry;				//sizeof = 2
#if BACKUP_ENABLE_NETWORK
	networkConfig_t network;		//sizeof = 73
#else
	char reserved[73];
#endif
	uint8_t alertBatteryLevel1;		//sizeof = 1
	uint8_t alertBatteryLevel2;		//sizeof = 1
	uint8_t alertBatteryLevelClear; //sizeof = 1
	uint8_t activeAlerts;			//sizeof = 1
	char contactPhone[15];			//sizeof = 15
};									//sizeof = 29 + 73 = 102

namespace config {

	namespace main {
		extern config_t value;

		void setup();
		void save();

		void reset();
	}
}