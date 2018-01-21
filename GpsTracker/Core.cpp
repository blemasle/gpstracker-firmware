#include "Core.h"

#define LOGGER_NAME "Core"

namespace core {

	GPS_TRACKER_STATES state;

	void setCurrentState(GPS_TRACKER_STATES s) {
		state = s;
		VERBOSE_FORMAT("setCurrentState", "%d, Free RAM : %d", state debug::freeRam());
	}

}