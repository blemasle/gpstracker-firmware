#pragma once

#include <Arduino.h>
#include <LowPower.h>

namespace mainunit {
	void sleep(period_t period);
	void deepSleep(uint16_t seconds);
}