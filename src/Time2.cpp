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

			timestamp += (time.day - 1) * SECS_PER_DAY;
			timestamp += time.hour * SECS_PER_HOUR;
			timestamp += time.minute * SECS_PER_MIN;
			timestamp += time.second;

			return timestamp;
		}

		__attribute__((__optimize__("O2")))
			void breakTime(timestamp_t timestamp, tmElements_t &time) {
			time.year = 0;
			time.month = 0;

			time.day = 0;
			time.second = timestamp % 60;
			timestamp /= 60; // now it is minutes
			time.minute = timestamp % 60;
			timestamp /= 60; // now it is hours
			time.hour = timestamp % 24;
			timestamp /= 24; // now it is days
			time.day = timestamp; //this is purely for indication / computation only as it might get over the number of days in a month
		}

	}
}