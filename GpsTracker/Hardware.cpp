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
		uint8_t networkPoweredCount = 0;
		uint8_t gpsPoweredCount = 0;

		void powerOn() {
			VERBOSE("powerOn");
			bool poweredOn = device.powerOnOff(true);
			if (!poweredOn) return;

			device.init();
			networkPoweredCount = gpsPoweredCount = 0;
		}

		void powerOff() {
			VERBOSE("powerOff");
			device.powerOnOff(false);
			networkPoweredCount = gpsPoweredCount = 0;
		}

		void powerOffIfUnused() {
			bool gpsPowered = false;

			if ((!device.getGpsPowerState(&gpsPowered) || !gpsPowered) &&
				(device.getPhoneFunctionality() != SIM808_PHONE_FUNCTIONALITY::FULL)) {
				powerOff();
			}
		}

		void setup() {
			NOTICE("setup");
			simSerial.begin(SIM808_BAUDRATE);
			device.begin(simSerial);
			powerOff(); //ensure powerOff on start
		}

		void gpsPowerOn() {
			if (!gpsPoweredCount) {
				VERBOSE("gpsPowerOn");
				powerOn();
				device.enableGps();
			}

			gpsPoweredCount++;
		}

		void gpsPowerOff() {
			if (!device.powered()) {
				networkPoweredCount = gpsPoweredCount = 0; //just to be sure counts == 0
				return;
			}

			if (gpsPoweredCount == 1) {
				VERBOSE("gpsPowerOff");
				device.disableGps();
				powerOffIfUnused();
			}

			if (gpsPoweredCount) gpsPoweredCount--; //avoid 255 if 0--
		}

		void networkPowerOn() {
			if (!networkPoweredCount) {
				VERBOSE("networkPowerOn");
				powerOn();
				device.setPhoneFunctionality(SIM808_PHONE_FUNCTIONALITY::FULL);
			}

			networkPoweredCount++;
		}

		void networkPowerOff() {
			if (!device.powered()) {
				networkPoweredCount = gpsPoweredCount = 0; //just to be sure counts == 0
				return;
			}


			if (networkPoweredCount == 1) {
				VERBOSE("networkPowerOff");
				device.disableGprs();
				device.setPhoneFunctionality(SIM808_PHONE_FUNCTIONALITY::MINIMUM);

				powerOffIfUnused();
			}

			if (networkPoweredCount) networkPoweredCount--; //avoid 255 if 0--
		}
	}

#define LOGGER_NAME "Hardware::i2c"

	namespace i2c {

		E24 eeprom = E24(E24Size_t::E24_512K);
		uint8_t poweredCount = 0;

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
