#include "Gps.h"
#include "Config.h"
#include "Debug.h"
#include "Hardware.h"
#include "MainUnit.h"

#define LOGGER_NAME "Gps"

#define TIME_YEAR_OFFSET	0
#define TIME_MONTH_OFFSET	4
#define TIME_DAY_OFFSET		6
#define TIME_HOUR_OFFSET	8
#define TIME_MINUTE_OFFSET	10
#define TIME_SECOND_OFFSET	12

namespace gps {

	namespace details {
		uint8_t parseSubstring(char *buffer, char *start, uint8_t size) {
			strlcpy(buffer, start, size + 1);
			return static_cast<uint8_t>(strtoul(buffer, NULL, 10));
		}

	}
	char lastPosition[GPS_POSITION_SIZE];
	SIM808_GPS_STATUS lastStatus;

	SIM808_GPS_STATUS acquireCurrentPosition(uint16_t timeout) {
		SIM808_GPS_STATUS currentStatus = SIM808_GPS_STATUS::OFF;

		do {
			currentStatus = hardware::sim808::device.getGpsStatus();
			if (currentStatus > SIM808_GPS_STATUS::NO_FIX) break;

			mainunit::deepSleep(GPS_DEFAULT_INTERMEDIATE_TIMEOUT_MS / 1000);
			timeout -= GPS_DEFAULT_INTERMEDIATE_TIMEOUT_MS;
		} while (timeout > 1);

		if (currentStatus > SIM808_GPS_STATUS::NO_FIX) {
			lastStatus = currentStatus;
			hardware::sim808::device.getGpsPosition(lastPosition);
		}

		return currentStatus;
	}

	uint8_t getVelocity() {
		uint8_t velocity;
		hardware::sim808::device.getGpsField(lastPosition, SIM808_GPS_FIELD::SPEED, &velocity);

		VERBOSE_FORMAT("getVelocity", "%d", velocity);

		return velocity;
	}

	void getTime(tmElements_t &time) {
		char *timeStr;
		char buffer[5];
		hardware::sim808::device.getGpsField(lastPosition, SIM808_GPS_FIELD::UTC, &timeStr);

		VERBOSE_FORMAT("getTime", "%s", timeStr);

		time.Year = CalendarYrToTm(details::parseSubstring(buffer, timeStr + TIME_YEAR_OFFSET, 4));
		time.Month = details::parseSubstring(buffer, timeStr + TIME_MONTH_OFFSET, 2);
		time.Day = details::parseSubstring(buffer, timeStr + TIME_DAY_OFFSET, 2);
		time.Hour = details::parseSubstring(buffer, timeStr + TIME_HOUR_OFFSET, 2);
		time.Minute = details::parseSubstring(buffer, timeStr + TIME_MINUTE_OFFSET, 2);
		time.Second = details::parseSubstring(buffer, timeStr + TIME_SECOND_OFFSET, 2);

		VERBOSE_FORMAT("getTime", "%d/%d/%d %d:%d:%d", tmYearToCalendar(time.Year), time.Month, time.Day, time.Hour, time.Minute, time.Second);
	}
}