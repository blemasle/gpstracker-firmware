#include "Config.h"
#include "Hardware.h"
#include "Pins.h"
#include "Debug.h"

#include <SoftwareSerial.h>
#include <SIM808.h>
#include <SIM808_Types.h>

#include <Wire.h>
#include <E24.h>


namespace hardware {

#define LOGGER_NAME "Hardware::sim808"

	namespace sim808 {
		SoftwareSerial simSerial = SoftwareSerial(SIM_TX, SIM_RX);
		SIM808 device = SIM808(SIM_RST, SIM_PWR, SIM_STATUS);

		void powerOn() {
			VERBOSE("powerOn");
			bool poweredOn = device.powerOnOff(true);
			if (!poweredOn) return;

			device.init();
		}

		void powerOff() {
			VERBOSE("powerOff");
			device.powerOnOff(false);
		}

		void powerOffIfUnused() {
			bool gpsPowered = false;
			bool gprsPowered = false;
			if ((!device.getGpsPowerState(&gpsPowered) || !gpsPowered) &&
				(!device.getGprsPowerState(&gprsPowered) || !gprsPowered)) {
				powerOff();
			}
		}

		void setup() {
			VERBOSE("setup");
			simSerial.begin(SIM808_BAUDRATE);
			device.begin(simSerial);
		}

		void gpsPowerOn() {
			VERBOSE("gpsPowerOn");
			powerOn();
			device.enableGps();
		}

		void gpsPowerOff() {
			VERBOSE("gpsPowerOff");
			device.disableGps();
			powerOffIfUnused();
		}

		void networkPowerOn() {
			VERBOSE("networkPowerOn");
			powerOn();
			device.setPhoneFunctionality(SIM808_PHONE_FUNCTIONALITY::FULL);
		}

		void networkPowerOff() {
			VERBOSE("networkPowerOff");
			device.setPhoneFunctionality(SIM808_PHONE_FUNCTIONALITY::MINIMUM);
			device.disableGprs();

			powerOffIfUnused();
		}
	}

#define LOGGER_NAME "Hardware::i2c"

	namespace i2c {

		E24 eeprom = E24(E24Size_t::E24_512K);

		uint8_t poweredCount = 0;

		//inline void powered() { digitalRead(I2C_PWR) == HIGH; } //TODO = replace enum with just reading the output pin ?

		void powerOn() {
			if (!poweredCount) {
				VERBOSE("powerOn");
				digitalWrite(I2C_PWR, HIGH);
				pinMode(I2C_PWR, OUTPUT);

				Wire.begin();
			}

			poweredCount++;
		}

		void powerOff(bool forced = false) {
			if (poweredCount == 1 || forced) {
				VERBOSE("powerOff");
				pinMode(I2C_PWR, INPUT);
				digitalWrite(I2C_PWR, LOW);

				//turn off i2c
				TWCR &= ~(bit(TWEN) | bit(TWIE) | bit(TWEA));

				//disable i2c internal pull ups
				digitalWrite(A4, LOW);
				digitalWrite(A5, LOW);
			}

			poweredCount--;
		}
	}
}
