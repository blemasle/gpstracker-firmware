#include "Config.h"
#include "Debug.h"
#include "Hardware.h"

#define LOGGER_NAME "Config"

namespace config {
	Config value;

	void format() {
		VERBOSE("format");
		hardware::i2c::eepromPowerOn();
		for (int i = 0; i < 1024; i++)
		{
			hardware::i2c::eeprom.write(i, 0);
		}
		hardware::i2c::eepromPowerOff();
	}

	void write() {
		VERBOSE_FORMAT("write", "%s, %s, %d, %d", value.seed, value.version, value.firstEntry, value.lastEntry);

		hardware::i2c::eepromPowerOn();
		int written = hardware::i2c::eeprom.writeBlock(CONFIG_ADDR, value);
		hardware::i2c::eepromPowerOff();
	}

	void reset() {
		VERBOSE("reset");
		Config config = {
			CONFIG_SEED,
			VERSION,
			0xFFFF,
			0xFFFF
		};

		value = config;
		//format();
		write();

		VERBOSE_FORMAT("reset", "value : %s, %s, %d, %d", value.seed, value.version, value.firstEntry, value.lastEntry);
	}

	void read() {
		hardware::i2c::eepromPowerOn();
		hardware::i2c::eeprom.readBlock(CONFIG_ADDR, value);
		if (!String(CONFIG_SEED).equals(value.seed)) reset();
		hardware::i2c::eepromPowerOff();

		VERBOSE_FORMAT("read", "%s, %s, %d, %d", value.seed, value.version, value.firstEntry, value.lastEntry);

	}	
}