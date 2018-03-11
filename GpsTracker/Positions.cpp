#include "Positions.h"

#include "Debug.h"
#include "Config.h"
#include "Gps.h"
#include "Network.h"
#include "Storage.h"

#define LOGGER_NAME "Positions"

#define ENTRY_RESERVED_SIZE	128
#define ENTRIES_ADDR		ENTRY_RESERVED_SIZE

namespace positions {

	uint16_t _maxEntryIndex = (E24_MAX_ADDRESS(hardware::i2c::eeprom.getSize()) - ENTRIES_ADDR) / ENTRY_RESERVED_SIZE;

	uint16_t getEntryAddress(uint16_t index) {
		if (index > _maxEntryIndex) return -1;
		return ENTRIES_ADDR + (ENTRY_RESERVED_SIZE * index);
	}

	void appendLast(const SIM808ChargingStatus battery, const SIM808_GPS_STATUS gpsStatus) {
		VERBOSE("appendLast");

		uint16_t lastEntryIndex;
		uint16_t firstEntryIndex;
		uint16_t entryAddress;
		PositionEntry entry = { battery, gpsStatus };
		strlcpy(entry.position, gps::lastPosition, POSITION_SIZE);

		storage::powerOn();
		config::read();
		
		firstEntryIndex = config::value.firstEntry;
		lastEntryIndex = config::value.lastEntry;

		lastEntryIndex++;
		if (lastEntryIndex > _maxEntryIndex) lastEntryIndex = 0;
		if (lastEntryIndex == firstEntryIndex) firstEntryIndex++;
		if (firstEntryIndex > _maxEntryIndex) firstEntryIndex = 0;

		entryAddress = getEntryAddress(lastEntryIndex);
		bool success = hardware::i2c::eeprom.writeBlock(entryAddress, entry);
		if (success) {
			VERBOSE_FORMAT("appendLast", "Written to EEPROM @ %X : [%d%% @ %dmV] [%d, %s]", entryAddress, battery.level, battery.voltage, gpsStatus, entry.position);
			
			config::value.firstEntry = firstEntryIndex;
			config::value.lastEntry = lastEntryIndex;
			config::write();
		}

		storage::powerOff();
	}

	void get(uint16_t index, PositionEntry &entry) {
		uint16_t entryAddress = getEntryAddress(index);
		if (entryAddress == -1) return;

		VERBOSE_FORMAT("get", "Reading entry n°%d @ %X", index, entryAddress);

		storage::powerOn();
		hardware::i2c::eeprom.readBlock(entryAddress, entry);
		storage::powerOff();

		VERBOSE_FORMAT("get", "Read from EEPROM @ %X : [%d%% @ %dmV] [%d, %s]", entryAddress, entry.battery.level, entry.battery.voltage, entry.status, entry.position);
	}

	bool moveNext(uint16_t &index) {
		config::read();
		if (index == config::value.lastEntry) return false;
		
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