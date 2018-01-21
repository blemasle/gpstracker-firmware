#pragma once

#include <TimeLib.h>
#include <Time.h>

namespace rtc {
	void powerOn();
	void powerOff();

	void setup();

	time_t getTime();
	void setTime(time_t &time);

	void setAlarm(uint16_t seconds);
	void setAlarm(time_t &time);
}