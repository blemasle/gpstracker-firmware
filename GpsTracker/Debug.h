#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>

#include "Config.h"
#include "Hardware.h"
#include "Gps.h"
#include "Rtc.h"

#ifdef _DEBUG

#define VERBOSE(f) Log.verbose(F("[" LOGGER_NAME "::" f "]\n"))
#define VERBOSE_MSG(f, msg) Log.verbose(F("[" LOGGER_NAME "::" f "] " msg "\n"))
#define VERBOSE_FORMAT(f, msg, ...) Log.verbose(F("[" LOGGER_NAME "::" f "] " msg "\n"), __VA_ARGS__)

#else

#define DISABLE_LOGGING 1

#define VERBOSE(f)
#define VERBOSE_MSG(f, msg)
#define VERBOSE_FORMAT(f, msg, ...)

#endif

#define DEBUG_SERIAL_SPEED 115200

namespace debug {

	enum class GPSTRACKER_DEBUG_COMMAND : uint8_t {
		NONE =					0,
		RUN =					1,
		ONCE =					2,
		RAM =					3,
		BATTERY =				4,
		GPS_ON =				5,
		GPS_OFF =				6,
		GPS_GET =				7,
		GPS_SET =				8,
		RTC_GET =				11,
		RTC_SET =				12,
		SD_WRITE_TEST =			13,
		EEPROM_GET_CONFIG =		14,
		EEPROM_RESET_CONFIG =	15,
		EEPROM_GET_CONTENT =	16,
		EEPROM_GET_LAST_ENTRY =	17,
		EEPROM_GET_ENTRIES =	18,
		EEPROM_ADD_ENTRY =		19
	};

	void waitForSerial();
	int freeRam();

	GPSTRACKER_DEBUG_COMMAND menu();

	void getAndDisplayBattery();
	void getAndDisplayGpsPosition();
	void setFakeGpsPosition();

	void getAndDisplayRtcTime();
	void setRtcTime();

	void getAndDisplayEepromConfig();
	void getAndDisplayEepromContent();
	void getAndDisplayEepromPositions();
	void getAndDisplayEepromLastPosition();
	void addLastPositionToEeprom();

	inline void displayFreeRam() { Serial.println(freeRam()); }

}
