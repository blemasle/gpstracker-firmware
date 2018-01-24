#include "Gps.h"
#include "Debug.h"
#include "Hardware.h"
#include "MainUnit.h"

#define LOGGER_NAME "Gps"
#define WAIT_FOR_FIX_DELAY 10000

#define TIME_YEAR_OFFSET	0
#define TIME_MONTH_OFFSET	4
#define TIME_DAY_OFFSET		6
#define TIME_HOUR_OFFSET	8
#define TIME_MINUTE_OFFSET	10
#define TIME_SECOND_OFFSET	12

#define STRTOUL_SUBSTRING(dst, src, size) strtoul(strncpy(dst, src , size), NULL, 2);

namespace gps {

	char lastPosition[GPS_POSITION_SIZE];
	SIM808_GPS_STATUS lastStatus;

	SIM808_GPS_STATUS acquireCurrentPosition(uint16_t timeout) {
		SIM808_GPS_STATUS currentStatus = SIM808_GPS_STATUS::OFF;

		do {
			currentStatus = hardware::sim808::device.getGpsStatus();
			if (currentStatus > SIM808_GPS_STATUS::NO_FIX) break;

			mainunit::deepSleep(WAIT_FOR_FIX_DELAY);
			timeout -= WAIT_FOR_FIX_DELAY;
		} while (timeout > 1);

		if (currentStatus > SIM808_GPS_STATUS::NO_FIX) {
			lastStatus = currentStatus;
			hardware::sim808::device.getGpsPosition(lastPosition);
		}

		return currentStatus;
	}

	void getTime(tmElements_t &time) {
		char *timeStr;
		char buffer[4];

		hardware::sim808::device.getGpsField(lastPosition, SIM808_GPS_FIELD::UTC, timeStr);

		time.Year = STRTOUL_SUBSTRING(buffer, timeStr + TIME_YEAR_OFFSET, 4);
		time.Month = STRTOUL_SUBSTRING(buffer, timeStr + TIME_MONTH_OFFSET, 2);
		time.Day = STRTOUL_SUBSTRING(buffer, timeStr + TIME_DAY_OFFSET, 2);
		time.Hour = STRTOUL_SUBSTRING(buffer, timeStr + TIME_HOUR_OFFSET, 2);
		time.Minute = STRTOUL_SUBSTRING(buffer, timeStr + TIME_MINUTE_OFFSET, 2);
		time.Second = STRTOUL_SUBSTRING(buffer, timeStr + TIME_SECOND_OFFSET, 2);
	}
}