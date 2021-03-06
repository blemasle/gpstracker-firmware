#pragma once

#include <Arduino.h>
#include "Positions.h"

enum class TRACKER_MOVING_STATE : uint8_t {
	MOVING = 0,
	PAUSED = 1,
	ABOUT_TO_STOP = 2,
	STOPPED = 3,
	STATIC = 4
};

namespace core {
	extern uint16_t sleepTime;

	void main();
	void updateRtcTime();
	TRACKER_MOVING_STATE updateSleepTime();
	uint16_t mapSleepTime(uint8_t velocity);

	uint8_t notifyFailures(PositionEntryMetadata &metadata);
}