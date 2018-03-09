#include "Debug.h"
#include "Flash.h"
#include "Positions.h"

#define MENU_ENTRY(name, text) const char MENU_##name[] PROGMEM = text

const char FAKE_GPS_ENTRY[] PROGMEM = "1,1,20170924074842.000,49.454862,1.144537,71.900,67.99,172.6,1,,1.3,2.2,1.8,,11,7,,,37,,";

MENU_ENTRY(HEADER,			"-- Debug Menu --");
MENU_ENTRY(SEPARATOR,		"----");

MENU_ENTRY(RUN,					"[R] Run");
MENU_ENTRY(RUN_ONCE,			"[r] Run once");
MENU_ENTRY(RAM,					"[f] Free RAM");
MENU_ENTRY(READ_BATTERY,		"[b] Read battery");
MENU_ENTRY(GPS_ON,				"[G] GPS On");
MENU_ENTRY(GPS_OFF,				"[g] GPS Off");
MENU_ENTRY(GPS_GET,				"[L] Get GPS position");
MENU_ENTRY(GPS_SET,				"[l] Set last GPS position");
MENU_ENTRY(I2C_ON,				"[I] I2C devices On");
MENU_ENTRY(I2C_OFF,				"[i] I2C devices Off");
MENU_ENTRY(RTC_SET,				"[T] Get RTC time");
MENU_ENTRY(RTC_GET,				"[t] Set RTC time");
MENU_ENTRY(SD_WRITE_TEST,		"[W] Write to test file");
MENU_ENTRY(EEPROM_GET_CONFIG,	"[C] Get EEPROM config");
MENU_ENTRY(EEPROM_RESET_CONFIG, "[c] Reset EEPROM config");
MENU_ENTRY(EEPROM_GET_ENTRIES,	"[E] Get EEPROM entries");
MENU_ENTRY(EEPROM_ADD_ENTRY,	"[e] Add last entry to EEPROM");
MENU_ENTRY(QUESTION,			"?");

const PROGMEM uint8_t commandIdMapping[] = {
	'R', static_cast<uint8_t>(debug::GPSTRACKER_DEBUG_COMMAND::RUN),
	'r', static_cast<uint8_t>(debug::GPSTRACKER_DEBUG_COMMAND::ONCE),
	'f', static_cast<uint8_t>(debug::GPSTRACKER_DEBUG_COMMAND::RAM),
	'b', static_cast<uint8_t>(debug::GPSTRACKER_DEBUG_COMMAND::BATTERY),
	'G', static_cast<uint8_t>(debug::GPSTRACKER_DEBUG_COMMAND::GPS_ON),
	'g', static_cast<uint8_t>(debug::GPSTRACKER_DEBUG_COMMAND::GPS_OFF),
	'L', static_cast<uint8_t>(debug::GPSTRACKER_DEBUG_COMMAND::GPS_GET),
	'l', static_cast<uint8_t>(debug::GPSTRACKER_DEBUG_COMMAND::GPS_SET),
	'I', static_cast<uint8_t>(debug::GPSTRACKER_DEBUG_COMMAND::I2C_ON),
	'i', static_cast<uint8_t>(debug::GPSTRACKER_DEBUG_COMMAND::I2C_OFF),
	'T', static_cast<uint8_t>(debug::GPSTRACKER_DEBUG_COMMAND::RTC_GET),
	't', static_cast<uint8_t>(debug::GPSTRACKER_DEBUG_COMMAND::RTC_SET),
	'W', static_cast<uint8_t>(debug::GPSTRACKER_DEBUG_COMMAND::SD_WRITE_TEST),
	'C', static_cast<uint8_t>(debug::GPSTRACKER_DEBUG_COMMAND::EEPROM_GET_CONFIG),
	'c', static_cast<uint8_t>(debug::GPSTRACKER_DEBUG_COMMAND::EEPROM_RESET_CONFIG),
	'E', static_cast<uint8_t>(debug::GPSTRACKER_DEBUG_COMMAND::EEPROM_GET_ENTRIES),
	'e', static_cast<uint8_t>(debug::GPSTRACKER_DEBUG_COMMAND::EEPROM_ADD_ENTRY),
};

const char * const MENU_ENTRIES[] PROGMEM = {
	MENU_HEADER,
	MENU_RUN,
	MENU_RUN_ONCE,

	MENU_SEPARATOR,

	MENU_RAM,
	MENU_READ_BATTERY,

	MENU_SEPARATOR,

	MENU_GPS_ON,
	MENU_GPS_OFF,
	MENU_GPS_GET,
	MENU_GPS_SET,

	MENU_SEPARATOR,

	MENU_I2C_ON,
	MENU_I2C_OFF,
	MENU_RTC_SET,
	MENU_RTC_GET,

	MENU_SEPARATOR,

	MENU_SD_WRITE_TEST,

	MENU_SEPARATOR,

	MENU_EEPROM_GET_CONFIG,
	MENU_EEPROM_RESET_CONFIG,
	MENU_EEPROM_GET_ENTRIES,
	MENU_EEPROM_ADD_ENTRY,

	MENU_QUESTION
};

int freeRam2() { // dirty hack because putting it in namespace doesn't compile
	extern int __heap_start, *__brkval;
	int v;
	return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

namespace debug {

	void waitForSerial() {
		while (!Serial);
		Serial.begin(DEBUG_SERIAL_SPEED);
		Serial.println("Starting !");
	}

	int freeRam() {
		return freeRam2();
	}

	GPSTRACKER_DEBUG_COMMAND parseCommand(char id) {
		size_t mappingArraySize = flash::getArraySize(commandIdMapping);
		char commandId;

		for (uint8_t i = 0; i < mappingArraySize; i += 2) {			
			commandId = pgm_read_byte_near(commandIdMapping + i);
			if (commandId == id) return static_cast<GPSTRACKER_DEBUG_COMMAND>(pgm_read_byte_near(commandIdMapping + i + 1));
		}

		return GPSTRACKER_DEBUG_COMMAND::NONE;
	}

	GPSTRACKER_DEBUG_COMMAND menu() {
		if (!Serial) return GPSTRACKER_DEBUG_COMMAND::RUN;

		GPSTRACKER_DEBUG_COMMAND command;
		size_t menuSize = flash::getArraySize(MENU_ENTRIES);
		
		do {		
			for (uint8_t i = 0; i < menuSize; i++) {
				Serial.println(reinterpret_cast<const __FlashStringHelper *>(pgm_read_word_near(&MENU_ENTRIES[i])));
			}

			while (!Serial.available());		
			command = parseCommand(Serial.read());
			while (Serial.available()) Serial.read();
		} while (command == GPSTRACKER_DEBUG_COMMAND::NONE);
		
		return command;
	}

	void getAndDisplayGpsPosition() {
		SIM808_GPS_STATUS gpsStatus = gps::acquireCurrentPosition(GPS_DEFAULT_TOTAL_TIMEOUT_MS);

		Log.notice(F("%d %s\n"), gpsStatus, gps::lastPosition);
	}

	void setFakeGpsPosition() {
		strlcpy_P(gps::lastPosition, FAKE_GPS_ENTRY, GPS_POSITION_SIZE);
		Log.notice(F("Last position set to : %s\n"), gps::lastPosition);
	}

	void getAndDisplayBattery() {
		hardware::sim808::powerOn();
		SIM808ChargingStatus status = hardware::sim808::device.getChargingState();
		hardware::sim808::powerOff();

		Log.notice(F("%d %d%% %dmV\n"), status.state, status.level, status.voltage);
	}

	void getAndDisplayRtcTime() {
		tmElements_t time;
		rtc::getTime(time);

		Log.notice(F("%d/%d/%d %d:%d:%d\n"), tmYearToCalendar(time.Year), time.Month, time.Day, time.Hour, time.Minute, time.Second);
	}

	void getAndDisplayEepromConfig() {
		config::read();
	}

	void getAndDisplayEepromPositions() {
		uint16_t firstEntryIndex = config::value.firstEntry;
		uint16_t currentEntryIndex = firstEntryIndex;
		PositionEntry currentEntry;

		do {
			positions::get(currentEntryIndex, currentEntry);
			Log.notice(F("%d%%, %dmV, %d, %s\n"), currentEntry.battery.level, currentEntry.battery.voltage, currentEntry.status, currentEntry.position);

		} while (currentEntryIndex != config::value.lastEntry);
		
	}

	void addLastPositionToEeprom() {
		hardware::sim808::powerOn();
		SIM808ChargingStatus status = hardware::sim808::device.getChargingState();
		hardware::sim808::powerOff();

		positions::appendLast(status, SIM808_GPS_STATUS::OFF);
	}

	void setRtcTime() {
		tmElements_t time;
		gps::getTime(time);
		rtc::setTime(time);

		Log.notice(F("OK\n"));
	}
}