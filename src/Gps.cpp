#include <math.h>
#include "Gps.h"
#include "Config.h"
#include "Hardware.h"
#include "MainUnit.h"
#include "Logging.h"

#define TIME_YEAR_OFFSET	0
#define TIME_MONTH_OFFSET	4
#define TIME_DAY_OFFSET		6
#define TIME_HOUR_OFFSET	8
#define TIME_MINUTE_OFFSET	10
#define TIME_SECOND_OFFSET	12

#define EARTH_RADIUS 6371 //kilometers


namespace gps {
	#define CURRENT_LOGGER "gps"

	namespace details {
		uint16_t parseSubstring(char *buffer, char *start, uint8_t size) {
			strlcpy(buffer, start, size + 1);
			return static_cast<uint16_t>(strtoul(buffer, NULL, 10));
		}

	}

	char lastPosition[GPS_POSITION_SIZE];
	SIM808_GPS_STATUS lastStatus;

	float previousLat = 0;
	float previousLng = 0;

	SIM808_GPS_STATUS acquireCurrentPosition(int32_t timeout) {
		#define CURRENT_LOGGER_FUNCTION "acquireCurrentPosition"

		SIM808_GPS_STATUS currentStatus = SIM808_GPS_STATUS::OFF;

		do {
			currentStatus = hardware::sim808::device.getGpsStatus(lastPosition, GPS_POSITION_SIZE);
			if (currentStatus > SIM808_GPS_STATUS::FIX) break; //if we have an accurate fix, break right now

			NOTICE_FORMAT("%d", currentStatus);
			mainunit::deepSleep(GPS_DEFAULT_INTERMEDIATE_TIMEOUT_MS / 1000);
			timeout -= GPS_DEFAULT_INTERMEDIATE_TIMEOUT_MS;
		} while (timeout > 1);

		if (currentStatus > SIM808_GPS_STATUS::NO_FIX) {
			lastStatus = currentStatus;
		}

		NOTICE_FORMAT("%d", currentStatus);
		return currentStatus;
	}

	void preserveCurrentCoordinates() {
		float lat, lng;
		if(!hardware::sim808::device.getGpsField(lastPosition, SIM808_GPS_FIELD::LATITUDE, &lat)) lat = 0;
		if(!hardware::sim808::device.getGpsField(lastPosition, SIM808_GPS_FIELD::LONGITUDE, &lng)) lng = 0;

		if (lat == 0 || lng == 0) return;
		previousLat = lat;
		previousLng = lng;
	}

	float getDistanceFromPrevious() {
		#define CURRENT_LOGGER_FUNCTION "getDistanceFromPrevious"

		float lat1, lng1, lat2, lng2;

		if(!hardware::sim808::device.getGpsField(lastPosition, SIM808_GPS_FIELD::LATITUDE, &lat2)) return 0;
		if(!hardware::sim808::device.getGpsField(lastPosition, SIM808_GPS_FIELD::LONGITUDE, &lng2)) return 0;

		VERBOSE_FORMAT("%s, %F, %F, %F, %F", lastPosition, previousLat, previousLng, lat2, lng2);

		lat1 = radians(previousLat);
		lng1 = radians(previousLng);

		lat2 = radians(lat2);
		lng2 = radians(lng2);

		float dlat = lat2 - lat1;
		float dlng = lng2 - lng1;
		float a = (
				pow(sin(dlat / 2), 2) +
				cos(lat1) * cos(lat2) * pow(sin(dlng / 2), 2)
			);


		a = EARTH_RADIUS * (2 * atan2(sqrt(a), sqrt(1 - a))); //kilometers

		NOTICE_FORMAT("%Fkm", a);
		return a;
	}

	uint8_t getVelocity() {
		#define CURRENT_LOGGER_FUNCTION "getVelocity"

		int16_t velocity;
		if (!hardware::sim808::device.getGpsField(lastPosition, SIM808_GPS_FIELD::SPEED, &velocity)) velocity = 0;

		VERBOSE_FORMAT("%d", velocity);

		return velocity;
	}

	void getTime(tmElements_t &time) {
		#define CURRENT_LOGGER_FUNCTION "getTime"

		char *timeStr;
		char buffer[5];
		hardware::sim808::device.getGpsField(lastPosition, SIM808_GPS_FIELD::UTC, &timeStr);

		VERBOSE_FORMAT("%s", timeStr);

		time.year = details::parseSubstring(buffer, timeStr + TIME_YEAR_OFFSET, 4);
		time.month = details::parseSubstring(buffer, timeStr + TIME_MONTH_OFFSET, 2);
		time.day = details::parseSubstring(buffer, timeStr + TIME_DAY_OFFSET, 2);
		time.hour = details::parseSubstring(buffer, timeStr + TIME_HOUR_OFFSET, 2);
		time.minute = details::parseSubstring(buffer, timeStr + TIME_MINUTE_OFFSET, 2);
		time.second = details::parseSubstring(buffer, timeStr + TIME_SECOND_OFFSET, 2);

		NOTICE_FORMAT("%d/%d/%d %d:%d:%d", time.year, time.month, time.day, time.hour, time.minute, time.second);
	}
}