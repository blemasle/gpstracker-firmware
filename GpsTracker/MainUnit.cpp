#include "MainUnit.h"
#include "Rtc.h"
#include "Pins.h"

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
		LowPower.powerDown(period, ADC_OFF, BOD_OFF);
	}

	void deepSleep(uint16_t seconds) {
		interruptIn(seconds);
		LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
	}
}