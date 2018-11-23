#include "MainUnit.h"
#include "Rtc.h"
#include "Hardware.h"
#include "config/Pins.h"
#include "Logging.h"

#define LOGGER_NAME "MainUnit"

extern int __heap_start, *__brkval;

namespace mainunit {

	namespace details {

		void prepareSleep() {
			hardware::sim808::simSerial.end(); //avoid woke up by SoftwareSerial interrupt
			delay(5); //ensure log messages have been printed out
		}

		void wokeUp() {
			tmElements_t wokeUpTime;
			rtc::getTime(wokeUpTime);
			VERBOSE_FORMAT("wokeUp", "%d:%d:%d", wokeUpTime.hour, wokeUpTime.minute, wokeUpTime.second);

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

	void deepSleep(uint16_t seconds) {
		NOTICE_FORMAT("deepSleep", "%d seconds", seconds);
		interruptIn(seconds);
		details::prepareSleep();
		LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
		details::wokeUp();
	}

	int freeRam() {
		int v;
		return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
	}
}