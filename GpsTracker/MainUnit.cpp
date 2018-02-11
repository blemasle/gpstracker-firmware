#include "MainUnit.h"
#include "Rtc.h"
#include "Pins.h"
#include "Debug.h"

const char WOKE_UP[] PROGMEM = "Woke up from sleep";

namespace mainunit {

	void interrupt() {
		detachInterrupt(digitalPinToInterrupt(RTC_WAKE));
	}

	void interruptIn(uint16_t seconds) {
		rtc::powerOn();
		rtc::setAlarm(seconds);
		rtc::powerOff();

		pinMode(RTC_WAKE, INPUT);
		attachInterrupt(digitalPinToInterrupt(RTC_WAKE), interrupt, FALLING);
	}

	void sleep(period_t period) {
		Log.notice(F("Sleeping for period : %d"), period);
		
		LowPower.powerDown(period, ADC_OFF, BOD_OFF);
		
		Log.verbose(reinterpret_cast<const __FlashStringHelper *>(pgm_read_word(WOKE_UP)));

	}

	void deepSleep(uint16_t seconds) {
		Log.notice(F("Deep sleeping for %d seconds"), seconds);

		interruptIn(seconds);
		Log.verbose("interruptIn done !");
		LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
		Log.verbose("hola");
		Log.verbose(reinterpret_cast<const __FlashStringHelper *>(pgm_read_word(WOKE_UP)));
	}
}