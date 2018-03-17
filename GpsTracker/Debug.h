#pragma once

#include <Arduino.h>
#include "Config.h"
#include "Log.h"

#include "Core.h"

#include "Hardware.h"
#include "Gps.h"
#include "Rtc.h"

#define DEBUG_SERIAL_SPEED 115200

namespace debug {

	enum class GPSTRACKER_DEBUG_COMMAND : uint8_t {
		NONE,
		RUN,
		ONCE,
		RAM,
		BATTERY,
		GPS_ON,
		GPS_OFF,
		GPS_GET,
		GPS_SET,
		RTC_GET,
		RTC_SET,
		SD_WRITE_TEST,
		EEPROM_GET_CONFIG,
		EEPROM_RESET_CONFIG,
		EEPROM_GET_CONTENT,
		EEPROM_GET_LAST_ENTRY,
		EEPROM_GET_ENTRIES,
		EEPROM_ADD_ENTRY,
		EEPROM_BACKUP_ENTRIES,
		SLEEP,
		SLEEP_DEEP
	};

	int freeRam();
	void displayFreeRam();

	GPSTRACKER_DEBUG_COMMAND menu(uint16_t timeout);

	void getAndDisplayBattery();
	void getAndDisplayGpsPosition();
	void setFakeGpsPosition();

	void getAndDisplayRtcTime();
	void setRtcTime();

	void getAndDisplaySleepTimes();

	void getAndDisplayEepromConfig();
	void getAndDisplayEepromContent();
	void getAndDisplayEepromPositions();
	void getAndDisplayEepromLastPosition();
	void addLastPositionToEeprom();


}
