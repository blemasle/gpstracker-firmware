#pragma once

#include <Arduino.h>
#include <SIM808.h>
#include <E24.h>
#include "config/Hardware.h"

namespace hardware {

	namespace sim808 {
		extern SIM_SERIAL_TYPE simSerial;
		extern SIM808 device;

		void powerOn();
		void powerOff();

		void setup();

		void gpsPowerOn();
		void gpsPowerOff();

		void networkPowerOn();
		void networkPowerOff();
	}

	namespace i2c {
		extern E24 eeprom;

		void powerOn();
		void powerOff(bool forced = false);
	}
}