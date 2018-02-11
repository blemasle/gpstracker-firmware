#include "Debug.h"
#include "Flash.h"
#include "Positions.h"

#define MENU_ENTRY(name, text) const char MENU_##name[] PROGMEM = text

MENU_ENTRY(HEADER,			"-- Debug Menu --");
MENU_ENTRY(SEPARATOR,		"----");

MENU_ENTRY(RUN,					"[0] Run");
MENU_ENTRY(RUN_ONCE,			"[1] Run once");
MENU_ENTRY(RAM,					"[2] Free RAM");
MENU_ENTRY(READ_BATTERY,		"[3] Read battery");
MENU_ENTRY(GPS_ON,				"[4] GPS On");
MENU_ENTRY(GPS_OFF,				"[5] GPS Off");
MENU_ENTRY(GPS_GET,				"[6] Get GPS position");
MENU_ENTRY(RTC_ON,				"[7] RTC On");
MENU_ENTRY(RTC_OFF,				"[8] RTC Off");
MENU_ENTRY(RTC_SET,				"[9] Get RTC time");
MENU_ENTRY(RTC_GET,				"[10] Set RTC time");
MENU_ENTRY(SD_WRITE_TEST,		"[11] Write to test file");
MENU_ENTRY(EEPROM_GET_CONFIG,	"[16] Get EEPROM config");
MENU_ENTRY(EEPROM_GET_ENTRIES,	"[17] Get EEPROM entries");
MENU_ENTRY(QUESTION,			"?");

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

	MENU_SEPARATOR,

	MENU_RTC_ON,
	MENU_RTC_OFF,
	MENU_RTC_SET,
	MENU_RTC_GET,

	MENU_SEPARATOR,

	MENU_SD_WRITE_TEST,

	MENU_SEPARATOR,

	MENU_EEPROM_GET_CONFIG,
	MENU_EEPROM_GET_ENTRIES,

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
		Serial.println("Starting !");
	}

	int freeRam() {
		return freeRam2();
	}

	GPSTRACKER_DEBUG_COMMAND menu() {
		if (!Serial) return GPSTRACKER_DEBUG_COMMAND::NONE;

		uint8_t command;
		size_t menuSize = flash::getArraySize(MENU_ENTRIES);
		
		do {		
			for (uint8_t i = 0; i < menuSize; i++) {
				Serial.println(reinterpret_cast<const __FlashStringHelper *>(pgm_read_word(&MENU_ENTRIES[i])));
			}

			while (!Serial.available());
			command = static_cast<uint8_t>(Serial.parseInt());
			while (Serial.available()) Serial.read();
		} while (command > static_cast<uint8_t>(GPSTRACKER_DEBUG_COMMAND::EEPROM_GET_ENTRIES));
		
		return static_cast<GPSTRACKER_DEBUG_COMMAND>(command);
	}

	void getAndDisplayGpsPosition() {
		SIM808_GPS_STATUS gpsStatus = gps::acquireCurrentPosition(GPS_DEFAULT_TOTAL_TIMEOUT_MS);

		Log.notice(F("%d %s\n"), gpsStatus, gps::lastPosition);
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
		hardware::i2c::eepromPowerOn();
		config::read();
		hardware::i2c::eepromPowerOff(); 
		
		Log.notice(F("%s, %s, %d, %d\n"), config::value.seed, config::value.version, config::value.firstEntry, config::value.lastEntry);
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

	void setRtcTime() {
		tmElements_t time;
		gps::getTime(time);
		rtc::setTime(time);

		Log.notice(F("OK"));
	}
}