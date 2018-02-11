#include "Core.h"
#include "Config.h"
#include "Flash.h"

#define LOGGER_NAME "Core"

namespace core {
	uint16_t sleepTime = SLEEP_DEFAULT_TIME_SECONDS;;

	void main() {
		gps::powerOn();
		SIM808_GPS_STATUS gpsStatus = gps::acquireCurrentPosition(GPS_DEFAULT_TOTAL_TIMEOUT_MS);
		SIM808ChargingStatus battery = hardware::sim808::device.getChargingState();
		gps::powerOff();

		if (gpsStatus > SIM808_GPS_STATUS::NO_FIX) {
			tmElements_t time;
			gps::getTime(time);
			rtc::powerOn();
			rtc::setTime(time);
			rtc::powerOff();

			positions::appendLast(battery, gpsStatus);

			uint8_t velocity;
			gps::getVelocity(velocity);
			core::setSleepTime(velocity);
		}

		if (positions::needsToSend()) {
			positions::send();
		}

		mainunit::deepSleep(core::sleepTime);
	}

	void setSleepTime(uint8_t velocity) {
		for (uint8_t i = 0; i < flash::getFlashArraySize(config::defaultSleepTimings); i++) {
			sleepTimings_t timing;
			flash::readFromFlash(&config::defaultSleepTimings[i], timing);

			if (velocity > timing.speed) continue;

			sleepTime = timing.seconds;
			return;
		}

		sleepTime = SLEEP_DEFAULT_TIME_SECONDS;
	}
}