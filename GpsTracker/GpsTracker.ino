#include "GpsTracker.h"


void setup() {
#ifdef _DEBUG
	debug::waitForSerial();
	Log.begin(LOG_LEVEL_VERBOSE, &Serial);
#endif

	rtc::powerOn();
	rtc::setup();
	rtc::powerOff();
}

void loop() {
	gps::powerOn();
	SIM808_GPS_STATUS gpsStatus = gps::acquireCurrentPosition(GPS_DEFAULT_TOTAL_TIMEOUT_MS);
	gps::powerOff();

	if (gpsStatus > SIM808_GPS_STATUS::NO_FIX) {
		tmElements_t time;
		gps::getTime(time);
		rtc::powerOn();
		rtc::setTime(time);
		rtc::powerOff();

		positions::appendLast();

		uint8_t velocity;
		gps::getVelocity(velocity);
		core::setSleepTime(velocity);
	}

	if (positions::needsToSend()) {
		positions::send();
	}

	mainunit::deepSleep(core::sleepTime);
}
