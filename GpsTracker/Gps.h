#pragma once

#include <SIM808.h>

#include "Hardware.h"
#include "Time2.h"

#define GPS_POSITION_SIZE 128

namespace gps {

	extern char lastPosition[GPS_POSITION_SIZE];
	extern SIM808_GPS_STATUS lastStatus;

	void powerOn();
	void powerOff();
	
	SIM808_GPS_STATUS acquireCurrentPosition();
	timestamp_t getTime();

}