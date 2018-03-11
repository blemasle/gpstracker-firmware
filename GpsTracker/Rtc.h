#pragma once

#include "Time2.h"
#include "Hardware.h"

namespace rtc {
	void setup();

	float getTemperature();
	timestamp_t getTime();
	void getTime(tmElements_t &time);
	void setTime(timestamp_t time);
	void setTime(const tmElements_t &time);

	void setAlarm(uint16_t seconds);
	void setAlarm(const tmElements_t &time);
}