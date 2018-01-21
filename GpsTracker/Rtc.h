#pragma once

namespace rtc {
	void powerOn();
	void powerOff();

	void setup();

	Time getTime();
	void setTime(Time time);
}