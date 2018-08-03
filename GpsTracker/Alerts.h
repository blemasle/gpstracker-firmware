#pragma once

#include "Positions.h"

#define ALERT_BATTERY_LEVEL_1				1
#define ALERT_BATTERY_LEVEL_2				2
#define ALERT_RTC_TEMPERATURE_FAILURE		3
#define ALERT_RTC_CLOCK_FAILURE				4

namespace alerts {

	uint8_t getTriggered(PositionEntryMetadata &metadata);
	void add(uint8_t mask);
	void clear(PositionEntryMetadata &metadata);
}
