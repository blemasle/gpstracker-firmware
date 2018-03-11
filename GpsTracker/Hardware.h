#pragma once

#include <SIM808.h>
#include <E24.h>

namespace hardware {

	namespace sim808 {
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