#pragma once


#define POSITIONS_CONFIG_DEFAULT_SAVE_THRESHOLD 10
#define POSITIONS_CONFIG_DEFAULT_APN			"Vodafone"
#define POSITIONS_CONFIG_DEFAULT_URL			"http://default.url"

struct networkConfig_t {
	uint8_t saveThreshold;
	uint16_t lastSavedEntry;
	char apn[20];
	char url[50];
};