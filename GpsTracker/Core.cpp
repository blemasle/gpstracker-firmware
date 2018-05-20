#include "Core.h"
#include "Config.h"
#include "Flash.h"

#define LOGGER_NAME "Core"

using namespace utils;

namespace core {
	uint16_t sleepTime = SLEEP_DEFAULT_TIME_SECONDS;
	uint8_t stoppedInARow = SLEEP_DEFAULT_STOPPED_THRESHOLD - 1;

	void main() {
		bool forceBackup = false;
		positions::prepareBackup();
		PositionEntryMetadata metadata;
		if (positions::acquire(metadata)) {
			positions::appendLast(metadata);
			forceBackup = updateSleepTime();

			gps::preserveCurrentCoordinates();
		}

		positions::doBackup(forceBackup);
		mainunit::deepSleep(sleepTime);
	}

	bool updateSleepTime() {
		uint8_t velocity = gps::getVelocity();
		uint16_t result = mapSleepTime(velocity);
		bool goingLongSleep = false;

		if (velocity < SLEEP_TIMING_MIN_MOVING_VELOCITY) {
			float distance = gps::getDistanceFromPrevious(); //did we missed positions because we were sleeping ?
			if (distance > GPS_DEFAULT_MISSED_POSITION_GAP_KM) stoppedInARow = 0;
			else stoppedInARow = min(stoppedInARow + 1, SLEEP_DEFAULT_STOPPED_THRESHOLD + 1); //avoid overflow on REALLY long stops
			
			if (stoppedInARow < SLEEP_DEFAULT_STOPPED_THRESHOLD) {
				result = SLEEP_DEFAULT_PAUSING_TIME_SECONDS;
			}
			else if (stoppedInARow == SLEEP_DEFAULT_STOPPED_THRESHOLD) goingLongSleep = true;
		}
		else stoppedInARow = 0;

		sleepTime = result;
		NOTICE_FORMAT("updateSleepTime", "%dkmh => %d seconds", velocity, sleepTime);

		return goingLongSleep;
	}

	uint16_t mapSleepTime(uint8_t velocity) {
		uint16_t result;
		uint16_t currentTime = 0xFFFF;
		
		if (rtc::isAccurate()) {
			tmElements_t time;
			rtc::getTime(time);

			currentTime = SLEEP_TIMING_TIME(time.Hour, time.Minute);
		}
		
		for (uint8_t i = flash::getArraySize(config::defaultSleepTimings); i--;) {
			sleepTimings_t timing;
			flash::read(&config::defaultSleepTimings[i], timing);

			if (velocity < timing.speed) continue;
			if (currentTime != 0xFFFF && (currentTime < timing.timeMin || currentTime > timing.timeMax)) continue;

			result = timing.seconds;
			break;

		}

		VERBOSE_FORMAT("computeSleepTime", "%d,%d", velocity, result);
		return result;
	}
}