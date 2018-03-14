#pragma once

#include <Arduino.h>

#define BACKUP_ENABLE_SDCARD 0
#define BACKUP_ENABLE_NETWORK 1

#if BACKUP_ENABLE_NETWORK
#include "NetworkPositionsConfig.h"
#endif

#define CONFIG_ADDR 0
#define CONFIG_RESERVED_SIZE 128
#define CONFIG_SEED 13
#define VERSION "1.00"

#define SIM808_BAUDRATE 4800

#define SLEEP_DEFAULT_TIME_SECONDS	1800

#define GPS_DEFAULT_INTERMEDIATE_TIMEOUT_MS	10000
#define GPS_DEFAULT_TOTAL_TIMEOUT_MS		180000

#define NETWORK_DEFAULT_INTERMEDIATE_TIMEOUT_MS 6000
#define NETWORK_DEFAULT_TOTAL_TIMEOUT_MS		180000

struct sleepTimings_t {
	uint8_t speed;
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
	{ 5, SLEEP_DEFAULT_TIME_SECONDS },
	{ 10, 1200 },
	{ 20, 600 },
	{ 30, 540 },
	{ 50, 480 },
	{ 80, 240 },
	{ 100, 210 },
	{ 180, 180 },
	};

	namespace main {
		extern config_t value;

		void setup();
		void save();

		void reset();
	}
}