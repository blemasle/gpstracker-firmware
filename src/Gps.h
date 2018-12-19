#pragma once

#include <Arduino.h>
#include <SIM808.Types.h>

#include "Hardware.h"
#include "Time2.h"

#define GPS_POSITION_SIZE 128

namespace gps {

	extern char lastPosition[GPS_POSITION_SIZE];
	extern SIM808_GPS_STATUS lastStatus;

	inline void powerOn() { hardware::sim808::gpsPowerOn(); }
	inline void powerOff() { hardware::sim808::gpsPowerOff(); }

	SIM808_GPS_STATUS acquireCurrentPosition(int32_t timeout);

	uint8_t getVelocity();
	float getDistanceFromPrevious();
	void getTime(tmElements_t &time);

	void preserveCurrentCoordinates();
}