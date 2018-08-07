#pragma once

#include "Time2.h"

namespace rtc {
	void setup();

	int16_t getTemperature();

	bool isAccurate();
	timestamp_t getTime();
	void getTime(tmElements_t &time);
	void setTime(const tmElements_t &time);

	void setAlarm(uint16_t seconds);
	void setAlarm(const tmElements_t &time);
}