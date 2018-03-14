#include "MainUnit.h"
#include "Rtc.h"
#include "Pins.h"
#include "Debug.h"

#define LOGGER_NAME "MainUnit"

namespace mainunit {

	namespace details {

		void prepareSleep() {
			hardware::sim808::simSerial.end(); //avoid woke up by SoftwareSerial interrupt
			delay(5); //ensure message have been printed out
		}

		void wokeUp() {
			tmElements_t wokeUpTime;
			rtc::getTime(wokeUpTime);
			NOTICE_FORMAT("wokeUp", "%d:%d:%d", wokeUpTime.Hour, wokeUpTime.Minute, wokeUpTime.Second);

			hardware::sim808::simSerial.listen();
		}

	}

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
		details::prepareSleep();
		LowPower.powerDown(period, ADC_OFF, BOD_OFF);
		details::wokeUp();

	}

	void deepSleep(uint16_t seconds) {
		NOTICE_FORMAT("deepSleep", "Deep sleeping for %d seconds", seconds);
		interruptIn(seconds);
		details::prepareSleep();
		LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
		details::wokeUp();
	}
}