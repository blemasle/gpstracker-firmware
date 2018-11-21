#pragma once

/**
 * \def USE_UDS3231
 * When defined, use the modified uDS3231 library instead of
 * the stock MD_DS3231 one.
 */
//#define USE_UDS3231

#ifdef USE_UDS3231
#include <uDS3231.h>
#else
#include <MD_DS3231.h>
#endif

#ifndef USE_UDS3231

//convenience macros to convert to and from tm years
#define  tmYearToCalendar(Y) ((Y) + 2000)  // full four digit year
#define  CalendarYrToTm(Y)   ((Y) - 2000)

typedef unsigned long timestamp_t;

typedef struct tmElements_t {
	uint8_t Second;
	uint8_t Minute;
	uint8_t Hour;
	uint8_t Day;
	uint8_t Month;
	uint8_t Year;   // year from 2000
};

class uDS3231_Ext : public MD_DS3231
{
private:
	void unpack(tmElements_t &time)
	void pack(const tmElements_t &time);
public:
	boolean readTime(tmElements_t &time);
	boolean writeTime(const tmElements_t &time);

	boolean readAlarm1(almType_t &almType, tmElements_t &time);
	boolean writeAlarm1(almType_t almType, const tmElements_t &time);
};

#define RTC_A_CLASS uDS3231_Ext

#else

#define RTC_A_CLASS uDS3231

#endif
