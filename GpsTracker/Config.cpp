#include "Config.h"
#include "Debug.h"
#include "Hardware.h"

#define LOGGER_NAME "Config"

namespace config {
	namespace main {

		config_t value;

		namespace details {

			void read() {
				VERBOSE("read");

				hardware::i2c::powerOn();
				hardware::i2c::eeprom.readBlock(CONFIG_ADDR, value);
				if (CONFIG_SEED != value.seed) reset(); //todo : reset network if seed for network is not right
				hardware::i2c::powerOff();

				NOTICE_FORMAT("read", "%d, %s, %d, %d, %d, %d, %d, %B, %s", value.seed, value.version, value.firstEntry, value.lastEntry, value.alertBatteryLevel1, value.alertBatteryLevel2, value.alertBatteryLevelClear, value.activeAlerts, value.contactPhone);
#if BACKUP_ENABLE_NETWORK
				NOTICE_FORMAT("read", "%d, %d, %s, %s", value.network.saveThreshold, value.network.lastSavedEntry, value.network.apn, value.network.url);
				//networkConfig_t c = {
				//	POSITIONS_CONFIG_NET_DEFAULT_SAVE_THRESHOLD,
				//	0xFFFF,
				//	POSITIONS_CONFIG_NET_DEFAULT_APN,
				//	POSITIONS_CONFIG_NET_DEFAULT_URL,
				//};
				//value.network = c;
#endif
				/*strcpy_P(value.version, PSTR(VERSION));
				value.alertBatteryLevel1 = CONFIG_DEFAULT_BATTERY_ALERT_LEVEL1;
				value.alertBatteryLevel2 = CONFIG_DEFAULT_BATTERY_ALERT_LEVEL2;
				value.alertBatteryLevelClear = CONFIG_DEFAULT_BATTERY_ALERT_CLEAR;
				value.activeAlerts = CONFIG_DEFAULT_ACTIVE_ALERTS;
				strcpy_P(config.contactPhone, PSTR(CONFIG_DEFAULT_CONTACT_PHONE));*/
			}

			void write() {
				NOTICE_FORMAT("write", "%d, %s, %d, %d, %d, %d, %d, %B, %s", value.seed, value.version, value.firstEntry, value.lastEntry, value.alertBatteryLevel1, value.alertBatteryLevel2, value.alertBatteryLevelClear, value.activeAlerts, value.contactPhone);
#if BACKUP_ENABLE_NETWORK
				NOTICE_FORMAT("write", "%d, %d, %s, %s", value.network.saveThreshold, value.network.lastSavedEntry, value.network.apn, value.network.url);
#endif
				hardware::i2c::powerOn();
				int written = hardware::i2c::eeprom.writeBlock(CONFIG_ADDR, value);
				hardware::i2c::powerOff();
			}
		}

		void setup() {
			details::read();
			//details::write();
		}

		void save() {
			details::write();
		}

		void reset() {
			VERBOSE("reset");

			config_t config = {};
			config.seed = CONFIG_SEED;
			strcpy_P(config.version, PSTR(VERSION));
			config.firstEntry = config.lastEntry = 0xFFFF;
			config.alertBatteryLevel1 = CONFIG_DEFAULT_BATTERY_ALERT_LEVEL1;
			config.alertBatteryLevel2 = CONFIG_DEFAULT_BATTERY_ALERT_LEVEL2;
			config.alertBatteryLevelClear = CONFIG_DEFAULT_BATTERY_ALERT_CLEAR;
			config.activeAlerts = CONFIG_DEFAULT_ACTIVE_ALERTS;
			strcpy_P(config.contactPhone, PSTR(CONFIG_DEFAULT_CONTACT_PHONE));

#if BACKUP_ENABLE_NETWORK
			config.network.saveThreshold = POSITIONS_CONFIG_NET_DEFAULT_SAVE_THRESHOLD;
			config.network.lastSavedEntry = 0xFFFF;
			strcpy_P(config.network.apn, PSTR(POSITIONS_CONFIG_NET_DEFAULT_APN));
			strcpy_P(config.network.url, PSTR(POSITIONS_CONFIG_NET_DEFAULT_URL));
#endif

			value = config;
			save();
		}

	}
}