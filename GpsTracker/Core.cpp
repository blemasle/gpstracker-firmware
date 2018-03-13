#include "Core.h"
#include "Config.h"
#include "Flash.h"

#define LOGGER_NAME "Core"

using namespace utils;

namespace core {
	uint16_t sleepTime = SLEEP_DEFAULT_TIME_SECONDS;;

	void main() {
		VERBOSE("main");

		PositionEntryMetadata metadata;
		if (positions::acquire(metadata)) {
			positions::appendLast(metadata);
			setSleepTime();
		}

		positions::doBackup();		
	}

	void setSleepTime() {
		setSleepTime(gps::getVelocity());
	}

	void setSleepTime(uint8_t velocity) {
		sleepTime = SLEEP_DEFAULT_TIME_SECONDS;

		for (uint8_t i = 0; i < flash::getArraySize(config::defaultSleepTimings); i++) {
			sleepTimings_t timing;
			flash::read(&config::defaultSleepTimings[i], timing);

			if (velocity > timing.speed) continue;

			sleepTime = timing.seconds;
			break;
		}

		VERBOSE_FORMAT("setSleepTime", "%d", sleepTime);
	}
}