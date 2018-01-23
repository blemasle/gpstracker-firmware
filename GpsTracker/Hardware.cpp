#include "Hardware.h"
#include "Pins.h"

#include <SoftwareSerial.h>
#include <SIM808.h>
#include <SIM808_Types.h>

namespace hardware {

	namespace sim808 {
		SoftwareSerial simSerial = SoftwareSerial(SIM_TX, SIM_RX);
		SIM808 device = SIM808(SIM_RST, SIM_PWR, SIM_STATUS);
		//idea : int powered
		//gps::powerOn() => +1
		//network::powerOn() => +1
		//gps::powerOff() => -1
		//network::powerOff() => -1

		//sim808:powerOff() => force powerOff of both
		//gps/network::powerOff() => powered == 1 => sim808::powerOff()
		//idea : gps power on = +1, network power on = +1 => powerOff forces power off of all, powerOff one will lead to actual powerOff if 
		void powerOn() {
			bool poweredOn = device.powerOnOff(true);
			if (!poweredOn) return;

			device.init();
		}

		void powerOff() {
			bool poweredOff = device.powerOnOff(false);
		}

		void init() {
			device.powerOnOff(true);
			simSerial.begin(4800);

			device.begin(simSerial);
			device.init();
		}

		void powerOffIfUnused() {
			bool gpsPowered = false;
			bool gprsPowered = false;
			if ((device.getGpsPowerState(&gpsPowered) && !gpsPowered) &&
				(device.getGprsPowerState(&gprsPowered) && !gprsPowered)) {
				powerOff();
			}
		}

		void gpsPowerOn() {
			powerOn();
			device.enableGps();
		}

		void gpsPowerOff() {
			device.disableGps();
			powerOffIfUnused();
		}

		void networkPowerOn() {
			powerOn();
			device.setPhoneFunctionality(SIM808_PHONE_FUNCTIONALITY::FULL);
			device.enableGprs("Free"); //TODO : configure
		}

		void networkPowerOff() {
			device.setPhoneFunctionality(SIM808_PHONE_FUNCTIONALITY::MINIMUM);
			device.disableGprs();

			powerOffIfUnused();
		}
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
