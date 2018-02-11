#pragma once

#include "Time2.h"
#include "Hardware.h"

namespace rtc {
	inline void powerOn() {
		hardware::i2c::rtcPowerOn();
	}

	inline void powerOff() {
		hardware::i2c::rtcPowerOff();
	}

	void setup();

	void getTime(tmElements_t &time);
	void setTime(tmElements_t &time);

	void setAlarm(uint16_t seconds);
	void setAlarm(tmElements_t &time);
}