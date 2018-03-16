#include "Core.h"
#include "Config.h"
#include "Flash.h"

#define LOGGER_NAME "Core"

using namespace utils;

namespace core {
	uint16_t sleepTime = SLEEP_DEFAULT_TIME_SECONDS;;
	uint8_t increaseInARow = 0;

	void main() {
		VERBOSE("main");

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
	}

	uint16_t computeSleepTime(uint8_t velocity) {
		uint16_t result;

		for (uint8_t i = 0; i < flash::getArraySize(config::defaultSleepTimings); i++) {
			sleepTimings_t timing;
			flash::read(&config::defaultSleepTimings[i], timing);

			if (velocity > timing.speed) continue;
	
			result = timing.seconds;
			break;
		}

		VERBOSE_FORMAT("computeSleepTime", "%d,%d", velocity, result);
		return result;
	}
}