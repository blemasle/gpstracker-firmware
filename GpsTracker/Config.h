#pragma once

struct sleepTimings_t {
	uint8_t speed;
	uint16_t seconds;
};

#define SLEEP_DEFAULT_TIME_SECONDS	1800

#define GPS_DEFAULT_INTERMEDIATE_TIMEOUT_MS	10000
#define GPS_DEFAULT_TOTAL_TIMEOUT_MS		30000

namespace config {

	static const sleepTimings_t defaultSleepTimings[] PROGMEM = {
		{ 5, SLEEP_DEFAULT_TIME_SECONDS },
		{ 10, 1200 },
		{ 20, 600 },
		{ 30, 540 },
		{ 50, 480 },
		{ 80, 240 },
		{ 100, 210 },
		{ 180, 180 },
	};

}