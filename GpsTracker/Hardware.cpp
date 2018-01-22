#include "Hardware.h"
#include "Pins.h"

#include <SIM808.h>

namespace hardware {

	namespace sim808 {
		SoftwareSerial simSerial = SoftwareSerial(SIM_TX, SIM_RX);
		SIM808 sim = SIM808(0, SIM_PWR, SIM_STATUS);
		//idea : int powered
		//gps::powerOn() => +1
		//network::powerOn() => +1
		//gps::powerOff() => -1
		//network::powerOff() => -1

		//sim808:powerOff() => force powerOff of both
		//gps/network::powerOff() => powered == 1 => sim808::powerOff()
		//idea : gps power on = +1, network power on = +1 => powerOff forces power off of all, powerOff one will lead to actual powerOff if 
		void powerOn() {

		}

		void powerOff() {

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