#include "Config.h"
#include "Hardware.h"
#include "Logging.h"
#include "Flash.h"

const char VERSION_STRING[] PROGMEM = VERSION;
const config_t CONFIG PROGMEM = {
	CONFIG_SEED,
	VERSION,
	0xFFFF,
	0xFFFF,
#if BACKUP_ENABLE_NETWORK
	{
		POSITIONS_CONFIG_NET_SAVE_THRESHOLD,
		0xFFFF,
		POSITIONS_CONFIG_NET_APN,
		POSITIONS_CONFIG_NET_URL
	},
#endif
	CONFIG_BATTERY_ALERT_LEVEL1,
	CONFIG_BATTERY_ALERT_LEVEL2,
	CONFIG_BATTERY_ALERT_CLEAR,
	CONFIG_ACTIVE_ALERTS,
	CONFIG_CONTACT_PHONE
};

namespace config {
	#define CURRENT_LOGGER "config"

	namespace main {

		config_t value;

		namespace details {

			void read() {
				#define CURRENT_LOGGER_FUNCTION "read"
				NOTICE;

				hardware::i2c::powerOn();
				hardware::i2c::eeprom.readBlock(CONFIG_ADDR, value);
				print();
				if (CONFIG_SEED != value.seed) reset(); //todo : reset network if seed for network is not right
				hardware::i2c::powerOff();

#if BACKUP_ENABLE_NETWORK
				//networkConfig_t c = {
				//	POSITIONS_CONFIG_NET_SAVE_THRESHOLD,
				//	0xFFFF,
				//	POSITIONS_CONFIG_NET_APN,
				//	POSITIONS_CONFIG_NET_URL,
				//};
				//value.network = c;
#endif
				/*strcpy_P(value.version, VERSION_STRING);
				value.alertBatteryLevel1 = CONFIG_BATTERY_ALERT_LEVEL1;
				value.alertBatteryLevel2 = CONFIG_BATTERY_ALERT_LEVEL2;
				value.alertBatteryLevelClear = CONFIG_BATTERY_ALERT_CLEAR;
				value.activeAlerts = CONFIG_ACTIVE_ALERTS;
				strcpy_P(config.contactPhone, PSTR(CONFIG_CONTACT_PHONE));*/
			}

			void write() {
				#define CURRENT_LOGGER_FUNCTION "write"
				NOTICE;

				print();
				hardware::i2c::powerOn();
				int written = hardware::i2c::eeprom.writeBlock(CONFIG_ADDR, value);
				hardware::i2c::powerOff();
			}
		}

		void setup() {
			details::read();

			if(strcasecmp_P(value.version, VERSION_STRING)) {
				strcpy_P(value.version, VERSION_STRING);
				details::write();
			}
		}

		void save() {
			details::write();
		}

		void reset() {
			#define CURRENT_LOGGER_FUNCTION "reset"
			NOTICE;

			utils::flash::read(&CONFIG, value);
			save();
		}

		void print() {
			#define CURRENT_LOGGER_FUNCTION "print"

			NOTICE_FORMAT("%d, %s, %d, %d, %d, %d, %d, %B, %s", value.seed, value.version, value.firstEntry, value.lastEntry, value.alertBatteryLevel1, value.alertBatteryLevel2, value.alertBatteryLevelClear, value.activeAlerts, value.contactPhone);
#if BACKUP_ENABLE_NETWORK
			NOTICE_FORMAT("%d, %d, %s, %s", value.network.saveThreshold, value.network.lastSavedEntry, value.network.apn, value.network.url);
#endif
		}

	}
}