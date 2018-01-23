#include "Rtc.h"
#include "Pins.h"

#include <Wire.h>
#include <MD_DS3231.h>

#define LOGGER_NAME "Rtc"

namespace rtc {

	namespace details {

		timestamp_t readTimeFromRegisters() {
			tmElements_t tmElements = {
				RTC.s,
				RTC.m,
				RTC.h,
				RTC.dow,
				RTC.dd,
				RTC.mm,
				CalendarYrToTm(RTC.yyyy)
			};

			return makeTime(tmElements);
		}

		void writeTimeToRegisters(timestamp_t &time) {
			tmElements_t tmElements;
			breakTime(time, tmElements);

			RTC.s = tmElements.Second;
			RTC.m = tmElements.Minute;
			RTC.h = tmElements.Hour;
			RTC.dow = tmElements.Wday;
			RTC.dd = tmElements.Day;
			RTC.mm = tmElements.Month;
			RTC.yyyy = tmYearToCalendar(tmElements.Year);
		}

	}

	
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
		RTC.control(DS3231_INT_ENABLE, DS3231_OFF); //INTCN OFF
	}

	timestamp_t getTime() {
		RTC.readTime();
		return details::readTimeFromRegisters();		
	}

	void setTime(timestamp_t &time) {
		details::writeTimeToRegisters(time);
		RTC.writeTime();
	}

	void setAlarm(uint16_t seconds) {
		timestamp_t t = getTime();
		t = t + seconds;
		setAlarm(t);
	}

	void setAlarm(timestamp_t &time) {
		details::writeTimeToRegisters(time);
		RTC.writeAlarm1(DS3231_ALM_S);

		RTC.control(DS3231_A1_FLAG, DS3231_OFF);
		RTC.control(DS3231_A1_INT_ENABLE, DS3231_ON); //Alarm 1 ON
		RTC.control(DS3231_INT_ENABLE, DS3231_ON); //INTCN ON
	}

}