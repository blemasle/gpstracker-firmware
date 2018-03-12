#include "Config.h"
#include "Debug.h"
#include "Hardware.h"

#define LOGGER_NAME "Config"

namespace config {
	namespace main {

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
				VERBOSE_FORMAT("write", "%s, %s, %s, %d, %d", value.seed, value.version, value.apn, value.firstEntry, value.lastEntry);

				hardware::i2c::powerOn();
				int written = hardware::i2c::eeprom.writeBlock(CONFIG_ADDR, value);
				hardware::i2c::powerOff();
			}
		}

		Config get() {
			if (value.seed[0] == '\0') details::read();

			VERBOSE_FORMAT("get", "%s, %s, %s, %d, %d", value.seed, value.version, value.apn, value.firstEntry, value.lastEntry);
			return value;
		}

		void set(const Config config) {
			value = config;
			details::write();
		}

		void reset() {
			VERBOSE("reset");
			Config config = {
				CONFIG_SEED,
				VERSION,
				"Vodafone", //TODO : read from SD
				0xFFFF,
				0xFFFF,
			};

			value = config;
			details::write();
		}

	}
}