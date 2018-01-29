#include "Core.h"
#include "Config.h"

#define LOGGER_NAME "Core"

namespace core {
	uint16_t sleepTime = SLEEP_DEFAULT_TIME_SECONDS;;

	namespace details {
		template<typename T, size_t N> size_t getFlashArraySize(T(&)[N]) { return N; }
		template<typename T> void readFromFlash(const T *source, T &dest) {
			memcpy_P(&dest, source, sizeof(T));
		}
	}

	void setSleepTime(uint8_t velocity) {
		for (uint8_t i = 0; i < details::getFlashArraySize(config::defaultSleepTimings); i++) {
			sleepTimings_t timing;
			details::readFromFlash(&config::defaultSleepTimings[i], timing);

			if (velocity > timing.speed) continue;

			sleepTime = timing.seconds;
			return;
		}

		sleepTime = SLEEP_DEFAULT_TIME_SECONDS;
	}
}