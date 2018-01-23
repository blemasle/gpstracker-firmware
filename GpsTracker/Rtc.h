#pragma once

#include "Time2.h"

namespace rtc {
	void powerOn();
	void powerOff();

	void setup();

	timestamp_t getTime();
	void setTime(timestamp_t &time);

	void setAlarm(uint16_t seconds);
	void setAlarm(timestamp_t &time);
}