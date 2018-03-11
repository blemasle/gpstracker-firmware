#include "Config.h"
#include "Debug.h"
#include "Hardware.h"

#define LOGGER_NAME "Config"

namespace config {
	Config value;

	namespace details {

		void read() {
			VERBOSE("read");
			hardware::i2c::powerOn();
			hardware::i2c::eeprom.readBlock(CONFIG_ADDR, value);
			if (!String(CONFIG_SEED).equals(value.seed)) reset();
			hardware::i2c::powerOff();

		}

		void write() {
			VERBOSE_FORMAT("write", "%s, %s, %d, %d", value.seed, value.version, value.firstEntry, value.lastEntry);

			hardware::i2c::powerOn();
			int written = hardware::i2c::eeprom.writeBlock(CONFIG_ADDR, value);
			hardware::i2c::powerOff();
		}
	}

	Config get() {
		if (value.seed[0] == '\0') details::read();

		VERBOSE_FORMAT("get", "%s, %s, %d, %d", value.seed, value.version, value.firstEntry, value.lastEntry);
		return value;
	}

	void set(Config config) {
		value = config;
		details::write();
	}

	void reset() {
		VERBOSE("reset");
		Config config = {
			CONFIG_SEED,
			VERSION,
			0xFFFF,
			0xFFFF,
			"Vodafone" //TODO : read from SD
		};

		value = config;
		details::write();

		VERBOSE_FORMAT("reset", "value : %s, %s, %d, %d", value.seed, value.version, value.firstEntry, value.lastEntry);
	}

	
}