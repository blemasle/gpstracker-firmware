#pragma once

#include "Positions.h"

#define ALERT_BATTERY_LEVEL_1				0
#define ALERT_BATTERY_LEVEL_2				1
#define ALERT_RTC_TEMPERATURE_FAILURE		2
#define ALERT_RTC_CLOCK_FAILURE				3

namespace alerts {

	uint8_t getTriggered(PositionEntryMetadata &metadata);
	void add(uint8_t mask);
	void clear(PositionEntryMetadata &metadata);
}
