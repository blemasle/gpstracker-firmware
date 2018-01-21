#include "Rtc.h"

#include <Wire.h>
#include <MD_DS3231.h>

#define LOGGER_NAME "Rtc"

namespace rtc {

	void powerOn() {
		digitalWrite(RTC_PWR, HIGH);
		pinMode(RTC_PWR, OUTPUT);

		Wire.begin();
	}

	void powerOff() {
		pinMode(RTC_PWR, INPUT);
		digitalWrite(RTC_PWR, LOW);

		//turn off i2c
		TWCR &= ~(bit(TWEN) | bit(TWIE) | bit(TWEA));

		//disable i2c internal pull ups
		digitalWrite(A4, LOW);
		digitalWrite(A5, LOW);
	}

	void setup() {
		RTC.control(DS3231_12H, DS3231_OFF); //24 hours clock
		RTC.control(DS3231_INT_ENABLE, DS3231_ON); //INTCN ON
	}

	Time getTime() {
		RTC.readTime();

		return {
			RTC.yyyy,
			RTC.mm,
			RTC.dd,
			RTC.h,
			RTC.m,
			RTC.s
		};
	}

	void setTime(Time time) {
		RTC.yyyy = time.yyyy;
		RTC.mm = time.mm;
		RTC.dd = time.dd;
		RTC.h = time.h;
		RTC.m = time.m;
		RTC.s = time.s;

		RTC.writeTime();
	}

}