#include "GpsTracker.h"
#include "Positions.h"

#if _DEBUG
#define MENU_DEFAULT_TIMEOUT 0
#else
#define MENU_DEFAULT_TIMEOUT 5000

#endif
bool bypassMenu = false;
uint16_t menuTimeout = MENU_DEFAULT_TIMEOUT;

void setup() {
	logging::setup();

	config::main::setup();
	rtc::setup();
	hardware::sim808::setup();

	positions::setup();
}

void loop() {

	debug::GPSTRACKER_DEBUG_COMMAND command = debug::GPSTRACKER_DEBUG_COMMAND::RUN;
	if (Serial && !bypassMenu) command = debug::menu(menuTimeout);

	if (command == debug::GPSTRACKER_DEBUG_COMMAND::RUN) bypassMenu = true;
	else menuTimeout = 0; //disable timeout once a command has been entered

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
			config::main::reset();
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::EEPROM_GET_CONTENT:
			debug::getAndDisplayEepromContent();
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::EEPROM_GET_ENTRIES:
			debug::getAndDisplayEepromPositions();
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::EEPROM_GET_LAST_ENTRY:
			debug::getAndDisplayEepromLastPosition();
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::EEPROM_ADD_ENTRY:
			debug::addLastPositionToEeprom();
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::EEPROM_BACKUP_ENTRIES:
			positions::doBackup();
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::SLEEP:
			mainunit::sleep(period_t::SLEEP_8S);
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::SLEEP_DEEP:
			mainunit::deepSleep(10);
			break;
		case debug::GPSTRACKER_DEBUG_COMMAND::SD_WRITE_TEST:
		default:
			NOTICE_MSG("loop", "Unsupported");
	}
}
