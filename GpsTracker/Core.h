#pragma once

#include <Arduino.h>

#include "Debug.h"

enum class GPS_TRACKER_STATES : uint8_t
{
	SETUP = 0,
	GET_POSITION = 1,
	SAVE_POSITION = 2,
	SEND_POSITION = 3,
	SLEEP = 4
};



namespace core {

	extern GPS_TRACKER_STATES state;

	void setCurrentState(GPS_TRACKER_STATES s);

}