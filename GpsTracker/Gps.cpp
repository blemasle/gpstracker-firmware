#include "Gps.h"
#include "Debug.h"

#define LOGGER_NAME "Gps"

namespace gps {

	char lastPosition[GPS_POSITION_SIZE];
	SIM808_GPS_STATUS lastStatus;

	void powerOn() {

	}

	void powerOff() {

	}

	SIM808_GPS_STATUS acquireCurrentPosition() {
		SIM808_GPS_STATUS currentStatus = SIM808_GPS_STATUS::OFF;

		//TODO : do while (!timeout && < accurate_fix)
		if (currentStatus > SIM808_GPS_STATUS::NO_FIX) {
			lastStatus = currentStatus;
		}

		return currentStatus;
	}

	time_t getTime() {
		
	}
}