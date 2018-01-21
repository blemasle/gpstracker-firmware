#pragma once

#include <TimeLib.h>
#include <Time.h>
#include <SIM808.h>

#define GPS_POSITION_SIZE 128

namespace gps {

	extern char lastPosition[GPS_POSITION_SIZE];
	extern SIM808_GPS_STATUS lastStatus;

	void powerOn();
	void powerOff();

	SIM808_GPS_STATUS acquireCurrentPosition();
	time_t getTime();

}