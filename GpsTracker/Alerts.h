#pragma once

#include "Positions.h"

#define ALERT_BATTERY_LEVEL_1	1
#define ALERT_BATTERY_LEVEL_2	2
#define ALERT_RTC_FAILURE		3

namespace alerts {

	extern uint8_t _alerts;

	uint8_t getTriggered(PositionEntryMetadata &metadata);
	inline void add(uint8_t mask) { _alerts |= mask; } //TODO : save to EEPROM to survive reset
	void clear(PositionEntryMetadata &metadata);
}
