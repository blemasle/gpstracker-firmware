#include "Config.h"
#include "Debug.h"
#include "Hardware.h"

#define LOGGER_NAME "Config"

namespace config {
	namespace main {

		Config_t value;

		namespace details {

			void read() {
				VERBOSE("read");
				hardware::i2c::powerOn();
				hardware::i2c::eeprom.readBlock(CONFIG_ADDR, value);
				if (CONFIG_SEED != value.seed) reset();
				hardware::i2c::powerOff();
			}

			void write() {
				VERBOSE_FORMAT("write", "%d, %s, %s, %d, %d", value.seed, value.version, value.firstEntry, value.lastEntry);

				hardware::i2c::powerOn();
				int written = hardware::i2c::eeprom.writeBlock(CONFIG_ADDR, value);
				hardware::i2c::powerOff();
			}
		}

		Config_t get() {
			if (value.seed == 0) details::read();

			VERBOSE_FORMAT("get", "%d, %s, %s, %d, %d", value.seed, value.version, value.firstEntry, value.lastEntry);
			return value;
		}

		void set(const Config_t config) {
			value = config;
			details::write();
		}

		void reset() {
			VERBOSE("reset");
			Config_t config = {
				CONFIG_SEED,
				VERSION,
				0xFFFF,
				0xFFFF,
			};

			value = config;
			details::write();
		}

	}
}