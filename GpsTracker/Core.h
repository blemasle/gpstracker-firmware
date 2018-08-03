#pragma once

#include <Arduino.h>

#include "Debug.h"
#include "Gps.h"
#include "MainUnit.h"
#include "Network.h"
#include "Rtc.h"
#include "Pins.h"
#include "Positions.h"

namespace core {
	extern uint16_t sleepTime;

	void main();
	void updateRtcTime();
	bool updateSleepTime();
	uint16_t mapSleepTime(uint8_t velocity);

	void notifyFailures(PositionEntryMetadata &metadata);
}