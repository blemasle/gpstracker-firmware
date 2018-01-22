#include "GpsTracker.h"


void setup() {
#ifdef _DEBUG
	debug::waitForSerial();
	Log.begin(LOG_LEVEL_VERBOSE, &Serial);
#endif

	core::setCurrentState(GPS_TRACKER_STATES::SETUP);

	rtc::powerOn();
	rtc::setup();
	rtc::powerOff();
}

void loop() {

	gps::powerOn();
	SIM808_GPS_STATUS gpsStatus = gps::acquireCurrentPosition();
	gps::powerOff();

	if (gpsStatus > SIM808_GPS_STATUS::NO_FIX) {
		time_t time = gps::getTime();
		rtc::powerOn();
		rtc::setTime(time);
		rtc::powerOff();

		positions::appendLast();
	}

	if (positions::needsToSend()) {
		positions::send();
	}

	mainunit::deepSleep(10); //duration TBD
}
