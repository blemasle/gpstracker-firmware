#include "Time2.h"

/*==============================================================================*/
/* Useful Constants */
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24UL)
#define DAYS_PER_WEEK (7UL)
#define SECS_PER_WEEK (SECS_PER_DAY * DAYS_PER_WEEK)
#define SECS_PER_YEAR (SECS_PER_WEEK * 52UL)
#define SECS_YR_2000  (946684800UL) // the time at the start of y2k

/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)  
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN) 
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define dayOfWeek(_time_)  ((( _time_ / SECS_PER_DAY + 4)  % DAYS_PER_WEEK)+1) // 1 = Sunday
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)  // this is number of days since Jan 1 2000
#define elapsedSecsToday(_time_)  (_time_ % SECS_PER_DAY)   // the number of seconds since last midnight 
// The following macros are used in calculating alarms and assume the clock is set to a date later than Jan 1 1971
// Always set the correct time before settting alarms
#define previousMidnight(_time_) (( _time_ / SECS_PER_DAY) * SECS_PER_DAY)  // time at the start of the given day
#define nextMidnight(_time_) ( previousMidnight(_time_)  + SECS_PER_DAY )   // time at the end of the given day 
#define elapsedSecsThisWeek(_time_)  (elapsedSecsToday(_time_) +  ((dayOfWeek(_time_)-1) * SECS_PER_DAY) )   // note that week starts on day 1
#define previousSunday(_time_)  (_time_ - elapsedSecsThisWeek(_time_))      // time at the start of the week for the given time
#define nextSunday(_time_) ( previousSunday(_time_)+SECS_PER_WEEK)          // time at the end of the week for the given time


/* Useful Macros for converting elapsed time to a timestamp_t */
#define minutesTotimestamp_t ((M)) ( (M) * SECS_PER_MIN)  
#define hoursTotimestamp_t   ((H)) ( (H) * SECS_PER_HOUR)  
#define daysTotimestamp_t    ((D)) ( (D) * SECS_PER_DAY) // fixed on Jul 22 2011
#define weeksTotimestamp_t   ((W)) ( (W) * SECS_PER_WEEK)

// leap year calulator expects year argument as years offset from 2000
#define LEAP_YEAR(Y)     ( ((2000+Y)>0) && !((2000+Y)%4) && ( ((2000+Y)%100) || !((2000+Y)%400) ) )

static  const uint8_t monthDays[] = { 31,28,31,30,31,30,31,31,30,31,30,31 }; // API starts months from 1, this array starts from 0

__attribute__((__optimize__("O2")))
void breakTime(const timestamp_t timeInput, tmElements_t &tm) {
		// break the given timestamp_t into time components
		// this is a more compact version of the C library localtime function
		// note that year is offset from 2000 !!!

		uint8_t year;
		uint8_t month, monthLength;
		uint32_t time;
		unsigned long days;

		time = (uint32_t)timeInput;
		tm.Second = time % 60;
		time /= 60; // now it is minutes
		tm.Minute = time % 60;
		time /= 60; // now it is hours
		tm.Hour = time % 24;
		time /= 24; // now it is days
		//tm.Wday = ((time + 4) % 7) + 1;  // Sunday is day 1 

		year = 0;
		days = 0;
		while ((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
			year++;
		}
		tm.Year = year; // year is offset from 2000 

		days -= LEAP_YEAR(year) ? 366 : 365;
		time -= days; // now it is days in this year, starting at 0

		days = 0;
		month = 0;
		monthLength = 0;
		for (month = 0; month<12; month++) {
			if (month == 1) { // february
				if (LEAP_YEAR(year)) {
					monthLength = 29;
				}
				else {
					monthLength = 28;
				}
			}
			else {
				monthLength = monthDays[month];
			}

			if (time >= monthLength) {
				time -= monthLength;
			}
			else {
				break;
			}
		}
		tm.Month = month + 1;  // jan is month 1  
		tm.Day = time + 1;     // day of month
}

__attribute__((__optimize__("O2")))
timestamp_t makeTimestamp(const tmElements_t &tm) {
	// assemble time elements into timestamp_t 
	// note year argument is offset from 2000 (see macros in time.h to convert to other formats)
	// previous version used full four digit year (or digits since 2000),i.e. 2009 was 2009 or 9

	int i;
	uint32_t seconds;

	// seconds from 2000 till 1 jan 00:00:00 of the given year
	seconds = tm.Year*(SECS_PER_DAY * 365);
	for (i = 0; i < tm.Year; i++) {
		if (LEAP_YEAR(i)) {
			seconds += SECS_PER_DAY;   // add extra days for leap years
		}
	}

	// add days for this year, months start from 1
	for (i = 1; i < tm.Month; i++) {
		if ((i == 2) && LEAP_YEAR(tm.Year)) {
			seconds += SECS_PER_DAY * 29;
		}
		else {
			seconds += SECS_PER_DAY * monthDays[i - 1];  //monthDay array starts from 0
		}
	}
	seconds += (tm.Day - 1) * SECS_PER_DAY;
	seconds += tm.Hour * SECS_PER_HOUR;
	seconds += tm.Minute * SECS_PER_MIN;
	seconds += tm.Second;
	return (timestamp_t)seconds;
}
/*=====================================================*/
/* Low level system time functions  */

//TODO : + and - operator between tmElements_t and timestamp_t