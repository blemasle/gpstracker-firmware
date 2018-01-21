#pragma once

#include "Time.h"

#define GPS_POSITION_SIZE 128

namespace gps {

	char lastPosition[GPS_POSITION_SIZE];
	SIM808_GPS_STATUS lastStatus;

	void powerOn();
	void powerOff();

	SIM808_GPS_STATUS acquireCurrentPosition();
	Time getTime();

}