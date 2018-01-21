#include "Gps.h"

#define LOGGER_NAME "Gps"

namespace gps {

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

	Time getTime() {
		
	}
}