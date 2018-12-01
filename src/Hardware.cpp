#include <Wire.h>
#include "Hardware.h"
#include "config/Pins.h"
#include "Logging.h"

namespace hardware {

#define LOGGER_NAME "Hardware::sim808"

	namespace sim808 {
		SIM_SERIAL_TYPE simSerial = SIM_SERIAL;
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
			//does not rely on count for safety
			//if there is a bug somewhere, the device will consume more battery,
			//but will not fail due to an over aggressive battery saving strategy
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
			if(gpsPoweredCount) {
				gpsPoweredCount++;
				return;
			}

			VERBOSE("gpsPowerOn");
			powerOn();
			if(!networkPoweredCount) {
				//SIM808 turns phone on by default but we don't need it for gps only
				device.setPhoneFunctionality(SIM808_PHONE_FUNCTIONALITY::MINIMUM);
			}
			device.powerOnOffGps(true);
		}

		void gpsPowerOff() {
			if (!device.powered()) {
				networkPoweredCount = gpsPoweredCount = 0; //just to be sure counts == 0
				return;
			}

			if(gpsPoweredCount > 1) {
				gpsPoweredCount--;
				return;
			}

			VERBOSE("gpsPowerOff");
			device.powerOnOffGps(false);
			powerOffIfUnused();
		}

		void networkPowerOn() {
			if(networkPoweredCount) {
				networkPoweredCount++;
				return;
			}

			VERBOSE("networkPowerOn");
			powerOn();
			device.setPhoneFunctionality(SIM808_PHONE_FUNCTIONALITY::FULL);
		}

		void networkPowerOff() {
			if (!device.powered()) {
				networkPoweredCount = gpsPoweredCount = 0; //just to be sure counts == 0
				return;
			}

			if(networkPoweredCount > 1) {
				networkPoweredCount--;
				return;
			}

			VERBOSE("networkPowerOff");
			device.disableGprs();
			device.setPhoneFunctionality(SIM808_PHONE_FUNCTIONALITY::MINIMUM);
			powerOffIfUnused();
		}
	}

#define LOGGER_NAME "Hardware::i2c"

	namespace i2c {

		E24 eeprom = E24(E24_SIZE, E24_ADDRESS);
		uint8_t poweredCount = 0;

		void powerOn() {
			if(poweredCount) {
				poweredCount++;
				return;
			}

			VERBOSE("powerOn");
			digitalWrite(I2C_PWR, HIGH);
			pinMode(I2C_PWR, OUTPUT);

			Wire.begin();
			poweredCount = 1;
		}

		void powerOff(bool forced = false) {
			if(poweredCount > 1 && !forced) {
				poweredCount--;
				return;
			}

			VERBOSE("powerOff");
			pinMode(I2C_PWR, INPUT);
			digitalWrite(I2C_PWR, LOW);

			//turn off i2c
			TWCR &= ~(bit(TWEN) | bit(TWIE) | bit(TWEA));

			//disable i2c internal pull ups
			digitalWrite(A4, LOW);
			digitalWrite(A5, LOW);

			poweredCount = 0;
		}
	}
}
