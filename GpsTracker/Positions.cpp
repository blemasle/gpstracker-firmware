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

	uint16_t getEntryAddress(uint16_t &index) {
		uint16_t address = ENTRIES_ADDR + (ENTRY_RESERVED_SIZE * index);
		if (address + ENTRY_RESERVED_SIZE > E24_MAX_ADDRESS(hardware::i2c::eeprom.getSize())) {
			address = ENTRIES_ADDR;
			index = 0;
		}

		return address;
	}

	void setEntriesIndexes(uint16_t lastEntry) {
		uint16_t firstEntry = config::value.firstEntry;

		while (config::value.lastEntry < config::value.firstEntry) {
			config::value.firstEntry++;
			getEntryAddress(firstEntry);
		}

		config::value.lastEntry = lastEntry;
		config::value.firstEntry = firstEntry;
	}

	void appendLast(const SIM808ChargingStatus battery, const SIM808_GPS_STATUS gpsStatus) {
		VERBOSE("appendLast");

		uint16_t entryIndex;
		uint16_t entryAddress;
		
		PositionEntry entry = { battery, gpsStatus };
		strncpy(entry.position, gps::lastPosition, POSITION_SIZE);

		storage::powerOn();
		config::read();
		entryIndex = config::value.lastEntry + 1;
		entryAddress = getEntryAddress(entryIndex);
		bool success = hardware::i2c::eeprom.writeBlock(entryAddress, entry);

		if (success) {
			VERBOSE_MSG("appendLast", "written to EEPROM");
			setEntriesIndexes(entryIndex);
			config::write();
		}

		storage::powerOff();
	}

	void get(uint16_t &index, PositionEntry &entry) {
		uint16_t entryAddress = getEntryAddress(index);

		storage::powerOn();
		hardware::i2c::eeprom.readBlock(entryAddress, entry);
		storage::powerOff();
	}

	bool needsToSend() {
		return false;
	}

	void send() {

	}
}