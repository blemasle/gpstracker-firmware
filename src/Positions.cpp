#include "Config.h"
#include "Positions.h"
#include "Gps.h"
#include "Rtc.h"
#include "Logging.h"

#if BACKUP_ENABLE_SDCARD || BACKUP_ENABLE_NETWORK
#define BACKUPS_ENABLED (BACKUP_ENABLE_SDCARD + BACKUP_ENABLE_NETWORK)
#endif

#if BACKUP_ENABLE_SDCARD
#include "SdPositionsBackup.h"
#endif

#if BACKUP_ENABLE_NETWORK
#include "NetworkPositionsBackup.h"
#endif

#define ENTRY_RESERVED_SIZE	128
#define ENTRIES_ADDR		CONFIG_RESERVED_SIZE

namespace positions {
	#define CURRENT_LOGGER "positions"

#if BACKUPS_ENABLED > 1
	backup::PositionsBackup **_backups;
#elif BACKUPS_ENABLED == 1
	backup::PositionsBackup * _backup;
#endif

	namespace details {
		uint16_t maxEntryIndex = 0;

		uint16_t getEntryAddress(uint16_t index) {
			if (index > maxEntryIndex) return -1;
			return ENTRIES_ADDR + (ENTRY_RESERVED_SIZE * index);
		}
	}

	void setup() {
		details::maxEntryIndex = (E24_MAX_ADDRESS(hardware::i2c::eeprom.getSize()) - ENTRIES_ADDR) / ENTRY_RESERVED_SIZE;

#if BACKUPS_ENABLED > 0
		backup::PositionsBackup * backup = NULL;
#if BACKUPS_ENABLED > 1
		uint8_t backupIdx = 0;
		_backups = new backup::PositionsBackup*[BACKUPS_ENABLED];
#endif //BACKUPS_ENABLED > 1

#if BACKUP_ENABLE_SDCARD
		backup = new backup::sd::SdPositionsBackup();
		backup->setup();
#if BACKUPS_ENABLED > 1
		_backups[backupIdx] = backup;
		backupIdx++;
#endif //BACKUPS_ENABLED > 1
#endif //BACKUP_ENABLE_SDCARD

#if BACKUP_ENABLE_NETWORK
		backup = new backup::net::NetworkPositionsBackup();
		backup->setup();
#if BACKUPS_ENABLED > 1
		_backups[backupIdx] = backup;
		backupIdx++;
#endif //BACKUPS_ENABLED > 1
#endif //BACKUP_ENABLE_NETWORK

#if BACKUPS_ENABLED == 1
	_backup = backup;
#endif //BACKUPS_ENABLED == 1
#endif //BACKUPS_ENABLED > 0

	}

	bool acquire(PositionEntryMetadata &metadata) {
		#define CURRENT_LOGGER_FUNCTION "acquire"
		NOTICE;

		timestamp_t before;

		gps::powerOn();
		before = rtc::getTime();
		SIM808_GPS_STATUS gpsStatus = gps::acquireCurrentPosition(GPS_DEFAULT_TOTAL_TIMEOUT_MS);
		uint16_t timeToFix = rtc::getTime() - before;
		SIM808ChargingStatus battery = hardware::sim808::device.getChargingState();
		gps::powerOff();

		bool acquired = gpsStatus >= SIM808_GPS_STATUS::FIX; //prety useless wins 14 bytes on the hex size rather than return gpStatus >= ...
		NOTICE_FORMAT("Status : %d", gpsStatus);

		metadata = {
			battery.level,
			battery.voltage,
			rtc::getTemperature(),
			timeToFix,
			gpsStatus
		};

		return acquired;
	}

	void appendLast(const PositionEntryMetadata &metadata) {
		#define CURRENT_LOGGER_FUNCTION "appendLast"
		VERBOSE;

		uint16_t entryIndex;
		uint16_t entryAddress;
		PositionEntry entry = { metadata };
		strlcpy(entry.position, gps::lastPosition, POSITION_SIZE);

		config_t* config = &config::main::value;
		entryIndex = config->lastEntry + 1;

		entryAddress = details::getEntryAddress(entryIndex);

		hardware::i2c::powerOn();
		hardware::i2c::eeprom.writeBlock(entryAddress, entry);

		NOTICE_FORMAT("Saved @ %X : %d,%d,%d,%d,%d,%s",
			entryAddress,
			entry.metadata.batteryLevel,
			entry.metadata.batteryVoltage,
			entry.metadata.temperature,
			static_cast<uint8_t>(entry.metadata.status),
			entry.metadata.timeToFix,
			entry.position);

		config->lastEntry++;
		if (config->lastEntry > details::maxEntryIndex) config->lastEntry = 0;
		if (config->lastEntry == config->firstEntry) config->firstEntry++;
		if (config->firstEntry > details::maxEntryIndex) config->firstEntry = 0;

		config::main::save();
		hardware::i2c::powerOff();
	}

	bool get(uint16_t index, PositionEntry &entry) {
		#define CURRENT_LOGGER_FUNCTION "get"
		VERBOSE;

		uint16_t entryAddress = details::getEntryAddress(index);
		if (entryAddress == -1) return false;

		VERBOSE_FORMAT("Reading entry %d @ %X", index, entryAddress);

		hardware::i2c::powerOn();
		hardware::i2c::eeprom.readBlock(entryAddress, entry);
		hardware::i2c::powerOff();

		NOTICE_FORMAT("Read from EEPROM @ %X : %d,%d,%d,%d,%d,%s",
			entryAddress,
			entry.metadata.batteryLevel,
			entry.metadata.batteryVoltage,
			entry.metadata.temperature,
			static_cast<uint8_t>(entry.metadata.status),
			entry.metadata.timeToFix,
			entry.position);

		return true;
	}

	bool moveNext(uint16_t &index) {
		if (index == config::main::value.lastEntry) return false;

		if (index == details::maxEntryIndex) index = 0; //could use a modulo but easier to understand that way
		else index++;

		return true;
	}

	uint16_t count(uint16_t fromIndex) {
		config_t *config = &config::main::value;
		uint16_t lastEntry = config->lastEntry;

		if (lastEntry < fromIndex) { lastEntry += details::maxEntryIndex; }

		return lastEntry - fromIndex;
	}

	void prepareBackup() {
#if BACKUPS_ENABLED > 1
		for (int i = 0; i < BACKUPS_ENABLED; i++) {
			_backups[i]->prepare();
		}
#elif BACKUPS_ENABLED == 1
		_backup->prepare();
#endif
	}

	void doBackup(bool force) {
#if BACKUPS_ENABLED > 1
		for (int i = 0; i < BACKUPS_ENABLED; i++) {
			_backups[i]->backup(force);
		}
#elif BACKUPS_ENABLED == 1
		_backup->backup(force);
#endif
	}
}