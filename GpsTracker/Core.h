#pragma once

#include <Arduino.h>

#include "Debug.h"

namespace core {
	extern uint16_t sleepTime;

	void setSleepTime(uint8_t velocity);
}