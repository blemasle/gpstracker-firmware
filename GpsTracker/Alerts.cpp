#pragma once

#include "Alerts.h"
#include "Config.h"
#include "Rtc.h"

namespace alerts {

	uint8_t getTriggered(PositionEntryMetadata &metadata) {
		config_t* config = &config::main::value;
		uint8_t result = 0;

		if (!rtc::isAccurate() && !bitRead(config->activeAlerts, ALERT_RTC_CLOCK_FAILURE)) {
			bitSet(result, ALERT_RTC_CLOCK_FAILURE);
		}

		if (metadata.temperature == ALERT_SUSPICIOUS_RTC_TEMPERATURE && !bitRead(config->activeAlerts, ALERT_RTC_TEMPERATURE_FAILURE)) {
			bitSet(result, ALERT_RTC_TEMPERATURE_FAILURE);
		}

		if (metadata.batteryLevel <= config->alertBatteryLevel1 && !bitRead(config->activeAlerts, ALERT_BATTERY_LEVEL_1)) {
			bitSet(result, ALERT_BATTERY_LEVEL_1);
		}

		if (metadata.batteryLevel <= config->alertBatteryLevel2 && !bitRead(config->activeAlerts, ALERT_BATTERY_LEVEL_2)) {
			bitSet(result, ALERT_BATTERY_LEVEL_2);
		}

		return result;
	}

	void add(uint8_t mask) {
		if (!mask) return; //save a write to eeprom if there is no change

		config_t* config = &config::main::value;
		config->activeAlerts |= mask;
		config::main::save();
	}

	void clear(PositionEntryMetadata &metadata) {
		config_t* config = &config::main::value;
		uint8_t clearMask = 0;

		if (bitRead(config->activeAlerts, ALERT_RTC_CLOCK_FAILURE) && rtc::isAccurate()) {
			bitSet(clearMask, ALERT_RTC_CLOCK_FAILURE);
		}

		if (bitRead(config->activeAlerts, ALERT_RTC_TEMPERATURE_FAILURE) && metadata.temperature != ALERT_SUSPICIOUS_RTC_TEMPERATURE) {
			bitSet(clearMask, ALERT_RTC_TEMPERATURE_FAILURE);
		}

		if ((config->activeAlerts & (_BV(ALERT_BATTERY_LEVEL_1) | _BV(ALERT_BATTERY_LEVEL_2))) && metadata.temperature >= config->alertBatteryLevelClear) {
			clearMask |= _BV(ALERT_BATTERY_LEVEL_1) | _BV(ALERT_BATTERY_LEVEL_2);
		}

		if (!clearMask) return; //save a write to eeprom if there is no change
		config->activeAlerts &= ~clearMask;
		config::main::save();
	}
}
