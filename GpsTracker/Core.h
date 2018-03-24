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
	void updateSleepTime(uint8_t velocity);
	uint16_t computeSleepTime(uint8_t velocity);
}