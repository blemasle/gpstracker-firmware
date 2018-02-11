#include "Config.h"
#include "Debug.h"
#include "Hardware.h"

#define LOGGER_NAME "Config"

namespace config {
	Config value;

	void write() {
		VERBOSE("writeConfig");
		hardware::i2c::eeprom.writeBlock(CONFIG_ADDR, value);

		VERBOSE_FORMAT("writeConfig", "%s, %s, %d, %d", value.seed, value.version, value.firstEntry, value.lastEntry);
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
	}

	void read() {
		VERBOSE("readConfig");
		hardware::i2c::eeprom.readBlock(CONFIG_ADDR, value);
		if (!String(CONFIG_SEED).equals(value.seed)) reset();

		VERBOSE_FORMAT("readConfig", "%s, %s, %d, %d", value.seed, value.version, value.firstEntry, value.lastEntry);
	}	
}