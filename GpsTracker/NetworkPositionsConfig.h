#pragma once


#define POSITIONS_CONFIG_NET_DEFAULT_SAVE_THRESHOLD		2
#define POSITIONS_CONFIG_NET_DEFAULT_APN				"Vodafone"
#define POSITIONS_CONFIG_NET_DEFAULT_URL				"http://requestbin.fullcontact.com/16q71o61"
#define POSITIONS_CONFIG_NET_DEFAULT_EXPECTED_RESPONSE	200


struct networkConfig_t {
	uint8_t saveThreshold;
	uint16_t lastSavedEntry;
	char apn[20];
	char url[50];
};