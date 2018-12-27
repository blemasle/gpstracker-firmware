#include "MainUnit.h"
#include "Rtc.h"
#include "Hardware.h"
#include "config/Pins.h"
#include "Logging.h"


extern int __heap_start, *__brkval;

namespace mainunit {
	#define CURRENT_LOGGER "mainunit"

	namespace details {

		void prepareSleep() {
			hardware::sim808::simSerial.end(); //avoid woke up by SoftwareSerial interrupt
			delay(5); //ensure log messages have been printed out
		}

		void wokeUp() {
			#define CURRENT_LOGGER_FUNCTION "wokeUp"

			tmElements_t wokeUpTime;
			rtc::getTime(wokeUpTime);
			VERBOSE_FORMAT("%d:%d:%d", wokeUpTime.hour, wokeUpTime.minute, wokeUpTime.second);

			hardware::sim808::simSerial.listen();
		}

	}

	void interrupt() {
		detachInterrupt(digitalPinToInterrupt(RTC_WAKE));
	}

	void interruptIn(uint16_t seconds) {
		pinMode(RTC_WAKE, INPUT);

		EIFR |= 1 << INTF1; //forcing interrupt clear as sometimes, it does not seems to work (1 second appart positions)
		rtc::setAlarm(seconds);
		attachInterrupt(digitalPinToInterrupt(RTC_WAKE), interrupt, FALLING);
	}

	void deepSleep(uint16_t seconds) {
		#define CURRENT_LOGGER_FUNCTION "deepSleep"
		NOTICE_FORMAT("%ds", seconds);

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