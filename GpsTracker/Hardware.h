#pragma once

#include <SIM808.h>

namespace hardware {

	namespace sim808 {
		extern SIM808 device;

		void powerOn();
		void powerOff();

		void init();

		void gpsPowerOn();
		void gpsPowerOff();

		void networkPowerOn();
		void networkPowerOff();
	}

	namespace rtc {
		void powerOn();
		void powerOff();
	}

	namespace eeprom {
		void powerOn();
		void powerOff();
	}
}