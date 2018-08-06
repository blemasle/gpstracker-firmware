#pragma once

#include "Debug.h"
#include "Alerts.h"
#include "Config.h"
#include "Rtc.h"

#define LOGGER_NAME "Alerts"

namespace alerts {

	uint8_t getTriggered(PositionEntryMetadata &metadata) {
		config_t* config = &config::main::value;
		uint8_t active = 0;

		if (metadata.batteryLevel <= config->alertBatteryLevel1) bitSet(active, ALERT_BATTERY_LEVEL_1);
		if (metadata.batteryLevel <= config->alertBatteryLevel2) bitSet(active, ALERT_BATTERY_LEVEL_2);
		if (metadata.temperature == ALERT_SUSPICIOUS_RTC_TEMPERATURE) bitSet(active, ALERT_RTC_TEMPERATURE_FAILURE);
		if (!rtc::isAccurate()) bitSet(active, ALERT_RTC_CLOCK_FAILURE);

		return config->activeAlerts ^ active;
	}

	void add(uint8_t mask) {
		config_t* config = &config::main::value;
		uint8_t active = config->activeAlerts;

		active |= mask;
		if (config->activeAlerts == active) return; //save a write to eeprom if there is no change

		config->activeAlerts = active;
		config::main::save();
	}

	void clear(PositionEntryMetadata &metadata) {
		config_t* config = &config::main::value;
		uint8_t clearMask = 0;
		uint8_t active = config->activeAlerts;

		if (metadata.batteryLevel >= config->alertBatteryLevelClear) clearMask |= _BV(ALERT_BATTERY_LEVEL_1) | _BV(ALERT_BATTERY_LEVEL_2);
		if (metadata.temperature != ALERT_SUSPICIOUS_RTC_TEMPERATURE) bitSet(clearMask, ALERT_RTC_TEMPERATURE_FAILURE);
		if (rtc::isAccurate()) bitSet(clearMask, ALERT_RTC_CLOCK_FAILURE);

		active &= ~clearMask;
		if (config->activeAlerts == active) return; //save a write to eeprom if there is no change

		config->activeAlerts = active;
		config::main::save();
	}
}
