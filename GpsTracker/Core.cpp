#include "Core.h"
#include "Config.h"
#include "Flash.h"

#define LOGGER_NAME "Core"

using namespace utils;

namespace core {
	uint16_t sleepTime = SLEEP_DEFAULT_TIME_SECONDS;;
	uint8_t increaseInARow = 0;

	void main() {

		PositionEntryMetadata metadata;
		if (positions::acquire(metadata)) {
			positions::appendLast(metadata);
			updateSleepTime(gps::getVelocity());
		}

		positions::doBackup();
		mainunit::deepSleep(sleepTime);
	}

	void updateSleepTime(uint8_t velocity) {
		uint16_t result = computeSleepTime(velocity);

		if (result > sleepTime) {
			increaseInARow++;
			if (increaseInARow < SLEEP_DEFAULT_INCREASE_THRESHOLD) return;

		}
		else increaseInARow = 0;

		sleepTime = result;
		NOTICE_FORMAT("updateSleepTime", "%dkmh => %d seconds", velocity, sleepTime);
	}

	uint16_t computeSleepTime(uint8_t velocity) {
		uint16_t result;
		uint16_t currentTime = 0xFFFF;
		
		if (rtc::isAccurate()) {
			tmElements_t time;
			rtc::getTime(time);

			currentTime = SLEEP_TIMING_TIME(time.Hour, time.Minute);
		}
		
		for (uint8_t i = 0; i < flash::getArraySize(config::defaultSleepTimings); i++) {
			sleepTimings_t timing;
			flash::read(&config::defaultSleepTimings[i], timing);

			if (velocity > timing.speed) continue;
			if (currentTime != 0xFFFF && (currentTime < timing.timeMin || currentTime > timing.timeMax)) continue;

			result = timing.seconds;
			break;
		}

		VERBOSE_FORMAT("computeSleepTime", "%d,%d", velocity, result);
		return result;
	}
}