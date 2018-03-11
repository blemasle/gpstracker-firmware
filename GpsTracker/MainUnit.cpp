#include "MainUnit.h"
#include "Rtc.h"
#include "Pins.h"
#include "Debug.h"

#define LOGGER_NAME "MainUnit"

namespace mainunit {

	void interrupt() {
		detachInterrupt(digitalPinToInterrupt(RTC_WAKE));
	}

	void interruptIn(uint16_t seconds) {
		rtc::setAlarm(seconds);

		pinMode(RTC_WAKE, INPUT);
		attachInterrupt(digitalPinToInterrupt(RTC_WAKE), interrupt, FALLING);
	}

	void sleep(period_t period) {
		NOTICE_FORMAT("sleep", "Sleeping for period : %d", period);
		delay(5);
		LowPower.powerDown(period, ADC_OFF, BOD_OFF);
		NOTICE_MSG("sleep", "Woke up");

	}

	void deepSleep(uint16_t seconds) {
		NOTICE_FORMAT("deepSleep", "Deep sleeping for %d seconds", seconds);
		interruptIn(seconds);
		delay(5);
		LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
		NOTICE_MSG("deepSleep", "Woke up");
	}
}