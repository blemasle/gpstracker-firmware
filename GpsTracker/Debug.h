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

#define DEBUG_SERIAL_SPEED 9600

namespace debug {

	enum class GPSTRACKER_DEBUG_COMMAND : uint8_t {
		NONE = 0,
		ONCE = 1,
		RAM = 2,
		BATTERY = 3,
		GPS_ON = 4,
		GPS_OFF = 5,
		GPS_GET = 6,
		RTC_ON = 7,
		RTC_OFF = 8,
		RTC_GET = 9,
		RTC_SET = 10
	};

	void waitForSerial();
	int freeRam();

	GPSTRACKER_DEBUG_COMMAND menu();

	void getAndDisplayBattery();
	void getAndDisplayGpsPosition();

	void getAndDisplayRtcTime();
	void setRtcTime();
	inline void displayFreeRam() { Serial.println(freeRam()); }

}
