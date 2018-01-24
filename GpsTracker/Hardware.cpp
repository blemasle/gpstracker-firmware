#include "Hardware.h"
#include "Pins.h"

#include <SoftwareSerial.h>
#include <SIM808.h>
#include <SIM808_Types.h>

#include <Wire.h>
 
namespace hardware {

	namespace sim808 {
		SoftwareSerial simSerial = SoftwareSerial(SIM_TX, SIM_RX);
		SIM808 device = SIM808(SIM_RST, SIM_PWR, SIM_STATUS);

		void powerOn() {
			bool poweredOn = device.powerOnOff(true);
			if (!poweredOn) return;

			device.init();
		}

		void powerOff() {
			device.powerOnOff(false);
		}

		void powerOffIfUnused() {
			bool gpsPowered = false;
			bool gprsPowered = false;
			if ((device.getGpsPowerState(&gpsPowered) && !gpsPowered) &&
				(device.getGprsPowerState(&gprsPowered) && !gprsPowered)) {
				powerOff();
			}
		}

		void init() {
			device.powerOnOff(true);
			simSerial.begin(4800);

			device.begin(simSerial);
			device.init();
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

	namespace i2c {

		#define DEVICE_RTC 1
		#define DEVICE_EEPROM 2

		uint8_t powered = 0;

		void powerOn() {
			if (powered > 0) return;

			digitalWrite(I2C_PWR, HIGH);
			pinMode(I2C_PWR, OUTPUT);

			Wire.begin();
		}

		void powerOff() {
			pinMode(I2C_PWR, INPUT);
			digitalWrite(I2C_PWR, LOW);

			//turn off i2c
			TWCR &= ~(bit(TWEN) | bit(TWIE) | bit(TWEA));

			//disable i2c internal pull ups
			digitalWrite(A4, LOW);
			digitalWrite(A5, LOW);
		}

		inline void powerOffIfUnused() {
			if (!powered) powerOff();
		}

		void rtcPowerOn() {
			powerOn();
			powered |= DEVICE_RTC;
		}

		void rtcPowerOff() {
			powered &= ~DEVICE_RTC;
			powerOffIfUnused();
		}

		void eepromPowerOn() {
			powerOn();
			powered |= DEVICE_EEPROM;
		}

		void eepromPowerOff() {
			powered &= ~DEVICE_EEPROM;
			powerOffIfUnused();
		}
	}
}
