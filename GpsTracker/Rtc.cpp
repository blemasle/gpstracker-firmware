#include "Debug.h"

#include "Rtc.h"
#include "Pins.h"

#include <Wire.h>
#include <MD_DS3231.h>

#define LOGGER_NAME "Rtc"

namespace rtc {

	namespace details {

		void readTimeFromRegisters(tmElements_t &time) {
			time.Second = RTC.s;
			time.Minute = RTC.m;
			time.Hour = RTC.h;
			time.Wday = RTC.dow;
			time.Day = RTC.dd;
			time.Month = RTC.mm;
			time.Year = CalendarYrToTm(RTC.yyyy);
		}

		void writeTimeToRegisters(tmElements_t &time) {
			RTC.s = time.Second;
			RTC.m = time.Minute;
			RTC.h = time.Hour;
			RTC.dow = time.Wday;
			RTC.dd = time.Day;
			RTC.mm = time.Month;
			RTC.yyyy = tmYearToCalendar(time.Year);
		}

	}
	
	void setup() {
		RTC.control(DS3231_12H, DS3231_OFF); //24 hours clock
		RTC.control(DS3231_INT_ENABLE, DS3231_OFF); //INTCN OFF
	}

	void getTime(tmElements_t &time) {
		RTC.readTime();
		details::readTimeFromRegisters(time);
	}

	void setTime(tmElements_t &time) {
		VERBOSE_FORMAT("setTime", "%d/%d/%d %d:%d:%d", tmYearToCalendar(time.Year), time.Month, time.Day, time.Hour, time.Minute, time.Second);
		details::writeTimeToRegisters(time);
		RTC.writeTime();
	}

	void setAlarm(uint16_t seconds) {
		tmElements_t currentTime;
		tmElements_t alarmTime;

		getTime(currentTime);

		breakTime(makeTimestamp(currentTime) + seconds, alarmTime);

		setAlarm(alarmTime);
	}

	void setAlarm(tmElements_t &time) {
		tmElements_t currentTime;
		
		getTime(currentTime);
		Log.verbose(F("Current time : %d/%d/%d %d:%d:%d\n"), tmYearToCalendar(currentTime.Year), currentTime.Month, currentTime.Day, currentTime.Hour, currentTime.Minute, currentTime.Second);
		
		details::writeTimeToRegisters(time);
		RTC.writeAlarm1(DS3231_ALM_DTHMS);
		getTime(currentTime);
		Log.notice(F("Set alarm to : %d/%d/%d %d:%d:%d\n"), tmYearToCalendar(time.Year), time.Month, time.Day, time.Hour, time.Minute, time.Second);

		RTC.control(DS3231_A1_FLAG, DS3231_OFF); //reset Alarm 1 flag
		RTC.control(DS3231_A1_INT_ENABLE, DS3231_ON); //Alarm 1 ON
		RTC.control(DS3231_INT_ENABLE, DS3231_ON); //INTCN ON
	}

}