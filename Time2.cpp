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

/*==============================================================================*/
/* Utility functions */

namespace utils {
	namespace time {

		__attribute__((__optimize__("O2")))
			timestamp_t makeTimestamp(const tmElements_t &time) {
			timestamp_t timestamp;

			timestamp += (time.Day - 1) * SECS_PER_DAY;
			timestamp += time.Hour * SECS_PER_HOUR;
			timestamp += time.Minute * SECS_PER_MIN;
			timestamp += time.Second;

			return timestamp;
		}

		__attribute__((__optimize__("O2")))
			void breakTime(timestamp_t timestamp, tmElements_t &time) {
			time.Year = 0;
			time.Month = 0;

			time.Day = 0;
			time.Second = timestamp % 60;
			timestamp /= 60; // now it is minutes
			time.Minute = timestamp % 60;
			timestamp /= 60; // now it is hours
			time.Hour = timestamp % 24;
			timestamp /= 24; // now it is days
			time.Day = timestamp; //this is purely for indication / computation only as it might get over the number of days in a month
		}

	}
}