#include "GpsTracker.h"

bool bypassMenu = false;

void setup() {
#ifdef _DEBUG
	debug::waitForSerial();
	Log.begin(LOG_LEVEL_VERBOSE, &Serial);	
#else
	if(Serial) Log.begin(LOG_LEVEL_NOTICE, &Serial);
#endif
 
	rtc::powerOn();
	rtc::setup();
	rtc::powerOff();

	hardware::sim808::setup();
}

void loop() {
	debug::GPSTRACKER_DEBUG_COMMAND command = debug::GPSTRACKER_DEBUG_COMMAND::RUN;
	if(!bypassMenu) command = debug::menu();

	bypassMenu = command == debug::GPSTRACKER_DEBUG_COMMAND::RUN;

	switch (command) {
		case debug::GPSTRACKER_DEBUG_COMMAND::RUN:
		case debug::GPSTRACKER_DEBUG_COMMAND::ONCE:
			core::main();
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::RAM:
			debug::displayFreeRam();
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::BATTERY:
			debug::getAndDisplayBattery();
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::GPS_ON:
			gps::powerOn();
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::GPS_OFF:
			gps::powerOff();
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::GPS_GET:
			debug::getAndDisplayGpsPosition();
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::GPS_SET:
			debug::setFakeGpsPosition();
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::I2C_ON:
			rtc::powerOn();
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::I2C_OFF:
			rtc::powerOff();
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::RTC_GET:
			debug::getAndDisplayRtcTime();
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::RTC_SET:
			debug::setRtcTime();
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::EEPROM_GET_CONFIG:
			debug::getAndDisplayEepromConfig();
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::EEPROM_RESET_CONFIG:
			config::reset();
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::EEPROM_GET_ENTRIES:
			debug::getAndDisplayEepromPositions();
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::EEPROM_ADD_ENTRY:
			debug::addLastPositionToEeprom();
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::SD_WRITE_TEST:
		default:
			Serial.println(F("Unsupported command !"));
	}
}
