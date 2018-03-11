#pragma once

#include "Time2.h"
#include "Hardware.h"

namespace rtc {
	void setup();

	void getTime(tmElements_t &time);
	void setTime(tmElements_t &time);

	void setAlarm(uint16_t seconds);
	void setAlarm(tmElements_t &time);
}