#include "Config.h"
#include "Debug.h"
#include "Hardware.h"

#define LOGGER_NAME "Config"

namespace config {
	Config value;

	void write() {
		VERBOSE_FORMAT("writeConfig", "%s, %s, %d, %d", value.seed, value.version, value.firstEntry, value.lastEntry);

		hardware::i2c::eepromPowerOn();
		int written = hardware::i2c::eeprom.writeBlock(CONFIG_ADDR, value);
		hardware::i2c::eepromPowerOff();
	}

	void reset() {
		VERBOSE("resetConfig");
		Config config = {
			CONFIG_SEED,
			VERSION,
			0,
			0
		};

		value = config;
		write();

		VERBOSE_FORMAT("resetConfig", "value : %s, %s, %d, %d", value.seed, value.version, value.firstEntry, value.lastEntry);
	}

	void read() {
		hardware::i2c::eepromPowerOn();
		hardware::i2c::eeprom.readBlock(CONFIG_ADDR, value);
		if (!String(CONFIG_SEED).equals(value.seed)) reset();
		hardware::i2c::eepromPowerOff();

		VERBOSE_FORMAT("readConfig", "%s, %s, %d, %d", value.seed, value.version, value.firstEntry, value.lastEntry);

	}	
}