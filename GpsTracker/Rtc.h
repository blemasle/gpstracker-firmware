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

	timestamp_t getTime();
	void setTime(timestamp_t &time);

	inline void setAlarm(uint16_t seconds) {
		setAlarm(getTime() + seconds);
	}

	void setAlarm(timestamp_t &time);
}