/**
 * Controls how and when the device goes to sleep.
 */

#pragma once

#define SLEEP_TIMING_TIME(hours, minutes) hours * 60 + minutes

#define SLEEP_DEFAULT_TIME_SECONDS						1800 	///< Default sleep time
#define SLEEP_TIMING_MIN_MOVING_VELOCITY				5		///< Speed under which to consider the tracker as not moving
#define SLEEP_DEFAULT_PAUSING_TIME_SECONDS				270		///< Sleep time to use when not moving
#define SLEEP_DEFAULT_STOPPED_THRESHOLD					5		///< Number of successive positions acquired as not moving before considering the tracker as stopped

#define SLEEP_TIMING_MIN SLEEP_TIMING_TIME(0, 0)
#define SLEEP_TIMING_MAX SLEEP_TIMING_TIME(23, 59)


/**
 * Represents a single entry in a larger parametization
 * of how long the tracker will go the sleep based on its speed
 * and the time of the day.
 */
struct sleepTimings_t {
	uint8_t speed;		///< Minimum speed (NOT maximum, the logic is reversed for code efficiency)
	uint16_t timeMin;	///< Minimum time of the day (UTC)
	uint16_t timeMax;	///< Maximum time of the day (UTC)
	uint16_t seconds;	///< Sleep duration in seconds
};

namespace config {

	/**
	 * Describes for how long the tracker should sleep before the next position
	 * acquisition based on the sleepTimings_t structure
	 */
	static const sleepTimings_t defaultSleepTimings[] PROGMEM = {
		// Sleep timings when not moving
		{ 0, SLEEP_TIMING_TIME(16, 00), SLEEP_TIMING_TIME(19, 59), 3600 },				///< 1 hour between 16:00 and 20:00 UTC (04:00 to 08:00 UTC+12)
		{ 0, SLEEP_TIMING_TIME(20, 00), SLEEP_TIMING_MAX, SLEEP_DEFAULT_TIME_SECONDS },	///< default (30 minutes) between 20:00 and 00:00 UTC (08:00 to 12:00 UTC+12)
		{ 0, SLEEP_TIMING_MIN, SLEEP_TIMING_TIME(8, 29), SLEEP_DEFAULT_TIME_SECONDS },	///< default (30 minutes) between 00:00 and 8:30 UTC (12:00 to 20:30 UTC+12)
		{ 0, SLEEP_TIMING_TIME(8, 30), SLEEP_TIMING_TIME(15, 59), 10800 },				///< 3 hours between 20:30 and 16:00 UTC (20:30 to 04:00 UTC+12)

		// Sleep timings while moving
		{ SLEEP_TIMING_MIN_MOVING_VELOCITY, SLEEP_TIMING_MIN, SLEEP_TIMING_MAX, 540 }, ///< 540 seconds when speed > SLEEP_TIMING_MIN_MOVING_VELOCITY (5km/h)
		{ 10, SLEEP_TIMING_MIN, SLEEP_TIMING_MAX, 270 },	///< 270 seconds when speed > 10 km/h
		{ 20, SLEEP_TIMING_MIN, SLEEP_TIMING_MAX, 225 },	///< 225 seconds when speed > 20 km/h
		{ 30, SLEEP_TIMING_MIN, SLEEP_TIMING_MAX, 240 },	///< 240 seconds when speed > 30 km/h
		{ 45, SLEEP_TIMING_MIN, SLEEP_TIMING_MAX, 280 },	///< 280 seconds when speed > 45 km/h
		{ 65, SLEEP_TIMING_MIN, SLEEP_TIMING_MAX, 276 },	///< 276 seconds when speed > 65 km/h
		{ 85, SLEEP_TIMING_MIN, SLEEP_TIMING_MAX, 225 }		///< 2225 seconds when speed > 85 km/h
	};

}