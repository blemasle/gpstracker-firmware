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

	namespace i2c {
		void powerOn();
		void powerOff();

		void rtcPowerOn();
		void rtcPowerOff();

		void eepromPowerOn();
		void eepromPowerOff();
	}
}