#pragma once

#include <Arduino.h>

struct sleepTimings_t {
	uint8_t speed;
	uint16_t seconds;
};

struct Config {
	char seed[5];
	char version[7];
	uint16_t firstEntry;
	uint16_t lastEntry;
};

#define CONFIG_ADDR 0
#define CONFIG_SEED "UIYB"
#define VERSION "1.00"

#define SLEEP_DEFAULT_TIME_SECONDS	1800

#define GPS_DEFAULT_INTERMEDIATE_TIMEOUT_MS	10000
#define GPS_DEFAULT_TOTAL_TIMEOUT_MS		30000

namespace config {

	extern Config value;

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

	void read();
	void reset();
	void write();

}