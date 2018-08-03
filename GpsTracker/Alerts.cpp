#pragma once

#include "Alerts.h"
#include "Config.h"

namespace alerts {

	uint8_t _alerts = 0;

	uint8_t getTriggered(PositionEntryMetadata &metadata) {
		config_t* config = &config::main::value;
		uint8_t result = 0;

		if (metadata.temperature == ALERT_SUSPICIOUS_RTC_TEMPERATURE && !bitRead(_alerts, ALERT_RTC_FAILURE)) {
			bitSet(result, ALERT_RTC_FAILURE);
		}

		if (metadata.batteryLevel <= config->alertBatteryLevel1 && !bitRead(_alerts, ALERT_BATTERY_LEVEL_1)) {
			bitSet(result, ALERT_BATTERY_LEVEL_1);
		}

		if (metadata.batteryLevel <= config->alertBatteryLevel2 && !bitRead(_alerts, ALERT_BATTERY_LEVEL_2)) {
			bitSet(result, ALERT_BATTERY_LEVEL_2);
		}

		return result;
	}

	void clear(PositionEntryMetadata &metadata) {
		config_t* config = &config::main::value;
		uint8_t clearMask = 0;

		if (bitRead(_alerts, ALERT_RTC_FAILURE) && metadata.temperature != ALERT_SUSPICIOUS_RTC_TEMPERATURE) {
			bitSet(clearMask, ALERT_RTC_FAILURE);
		}

		if (_alerts & (_BV(ALERT_BATTERY_LEVEL_1) | _BV(ALERT_BATTERY_LEVEL_2) && metadata.temperature >= config->alertBatteryLevelClear)) {
			clearMask |= _BV(ALERT_BATTERY_LEVEL_1) | _BV(ALERT_BATTERY_LEVEL_2);
		}

		_alerts &= ~clearMask;
	}
}
