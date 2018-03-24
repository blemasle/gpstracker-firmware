#pragma once

#include <Arduino.h>

#define BACKUP_ENABLE_SDCARD 0
#define BACKUP_ENABLE_NETWORK 1

#if BACKUP_ENABLE_NETWORK
#include "NetworkPositionsConfig.h"
#endif

#define SIM808_BAUDRATE 4800

#define CONFIG_ADDR 0
#define CONFIG_RESERVED_SIZE 128
#define CONFIG_SEED 13
#define VERSION "1.00"

#pragma region Default configuration values

#define MENU_TIMEOUT	10000
/**
 \def SLEEP_DEFAULT_TIME_SECONDS
 Hard coded value for default sleep time between position acquisitions.
 Exprimed in seconds
*/
#define SLEEP_DEFAULT_TIME_SECONDS						1800
#define SLEEP_DEFAULT_INCREASE_THRESHOLD				3

#define GPS_DEFAULT_INTERMEDIATE_TIMEOUT_MS				10000
#define GPS_DEFAULT_TOTAL_TIMEOUT_MS					80000

#define NETWORK_DEFAULT_INTERMEDIATE_TIMEOUT_MS			6000
#define NETWORK_DEFAULT_TOTAL_TIMEOUT_MS				80000
#define NETWORK_DEFAULT_NO_NETWORK_QUALITY_THRESHOLD	8
#define NETWORK_DEFAULT_NO_NETWORK_TRIES				5

#pragma endregion

#define SLEEP_TIMING_TIME(hours, minutes) hours * 60 + minutes
#define SLEEP_TIMING_MIN SLEEP_TIMING_TIME(0, 0)
#define SLEEP_TIMING_MAX SLEEP_TIMING_TIME(23, 59)

struct sleepTimings_t {
	uint8_t speed;
	uint16_t timeMin;
	uint16_t timeMax;
	uint16_t seconds;
};

struct config_t {
	uint8_t seed;
	char version[5];
	uint16_t firstEntry;
	uint16_t lastEntry;
#if BACKUP_ENABLE_NETWORK
	networkConfig_t network;
#endif
};

namespace config {

	static const sleepTimings_t defaultSleepTimings[] PROGMEM = {
	{ 3, SLEEP_TIMING_TIME(16, 00), SLEEP_TIMING_TIME(19, 59), 3600 },
	{ 3, SLEEP_TIMING_TIME(20, 00), SLEEP_TIMING_MAX, SLEEP_DEFAULT_TIME_SECONDS },
	{ 3, SLEEP_TIMING_MIN, SLEEP_TIMING_TIME(8, 29), SLEEP_DEFAULT_TIME_SECONDS },
	{ 3, SLEEP_TIMING_TIME(8, 30), SLEEP_TIMING_TIME(15, 59), 10800 },

	{ 5, SLEEP_TIMING_MIN, SLEEP_TIMING_MAX, 900 },
	{ 20, SLEEP_TIMING_MIN, SLEEP_TIMING_MAX, 600 },
	{ 30, SLEEP_TIMING_MIN, SLEEP_TIMING_MAX, 540 },
	{ 50, SLEEP_TIMING_MIN, SLEEP_TIMING_MAX, 480 },
	{ 80, SLEEP_TIMING_MIN, SLEEP_TIMING_MAX, 240 },
	{ 100, SLEEP_TIMING_MIN, SLEEP_TIMING_MAX, 210 },
	{ 180, SLEEP_TIMING_MIN, SLEEP_TIMING_MAX, 180 },
	};

	namespace main {
		extern config_t value;

		void setup();
		void save();

		void reset();
	}
}