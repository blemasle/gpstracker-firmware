#include "Positions.h"

#include "Debug.h"
#include "Config.h"
#include "Gps.h"
#include "Network.h"

#define LOGGER_NAME "Positions"

#define ENTRY_RESERVED_SIZE	128
#define ENTRIES_ADDR		ENTRY_RESERVED_SIZE

namespace positions {

	uint16_t _maxEntryIndex = (E24_MAX_ADDRESS(hardware::i2c::eeprom.getSize()) - ENTRIES_ADDR) / ENTRY_RESERVED_SIZE;

	uint16_t getEntryAddress(uint16_t index) {
		if (index > _maxEntryIndex) return -1;
		return ENTRIES_ADDR + (ENTRY_RESERVED_SIZE * index);
	}

	bool acquire(PositionEntryMetadata &metadata) {
		VERBOSE("acquire");

		timestamp_t before;

		gps::powerOn();
		before = rtc::getTime();
		SIM808_GPS_STATUS gpsStatus = gps::acquireCurrentPosition(GPS_DEFAULT_TOTAL_TIMEOUT_MS);
		SIM808ChargingStatus battery = hardware::sim808::device.getChargingState();
		gps::powerOff();

		if (gpsStatus < SIM808_GPS_STATUS::FIX) return false;

		uint16_t timeToFix = rtc::getTime() - before;

		tmElements_t time;
		gps::getTime(time);
		rtc::setTime(time);

		metadata = {
			battery.level,
			battery.voltage,
			rtc::getTemperature(),
			timeToFix,
			gpsStatus
		};

		return true;
	}

	void appendLast(const PositionEntryMetadata &metadata) {
		VERBOSE("appendLast");

		uint16_t entryAddress;
		PositionEntry entry = { metadata };
		strlcpy(entry.position, gps::lastPosition, POSITION_SIZE);

		hardware::i2c::powerOn();
		Config config = config::get();

		config.lastEntry++;
		if (config.lastEntry > _maxEntryIndex) config.lastEntry = 0;
		if (config.lastEntry == config.firstEntry) config.firstEntry++;
		if (config.firstEntry > _maxEntryIndex) config.firstEntry = 0;

		entryAddress = getEntryAddress(config.lastEntry);
		hardware::i2c::eeprom.writeBlock(entryAddress, entry);

		VERBOSE_FORMAT("appendLast", "Written to EEPROM @ %X : [%d%% @ %dmV] [%f°C] [TTF : %d, Status : %d, Position : %s]", entryAddress, entry.metadata.batteryLevel, entry.metadata.batteryVoltage, entry.metadata.temperature, entry.metadata.timeToFix, entry.metadata.status, entry.position);

		config::set(config);
		hardware::i2c::powerOff();
	}

	bool get(uint16_t index, PositionEntry &entry) {
		uint16_t entryAddress = getEntryAddress(index);
		if (entryAddress == -1) return false;

		VERBOSE_FORMAT("get", "Reading entry n°%d @ %X", index, entryAddress);

		hardware::i2c::powerOn();
		hardware::i2c::eeprom.readBlock(entryAddress, entry);
		hardware::i2c::powerOff();

		VERBOSE_FORMAT("get", "Read from EEPROM @ %X : [%d%% @ %dmV] [%f°C] [%d, %s]", entryAddress, entry.metadata.batteryLevel, entry.metadata.batteryVoltage, entry.metadata.temperature, entry.metadata.timeToFix, entry.metadata.status, entry.position);
		return true;
	}

	bool moveNext(uint16_t &index) {
		if (index == config::get().lastEntry) return false;
		
		if (index == _maxEntryIndex) index = 0; //could use a modulo but easier to understand that way
		else index++;

		return true;
	}

	bool needsToSend() {
		return false;
	}

	void send() {

	}
}