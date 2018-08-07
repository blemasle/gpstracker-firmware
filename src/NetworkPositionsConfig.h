#pragma once


#define POSITIONS_CONFIG_NET_DEFAULT_SAVE_THRESHOLD								30
#define POSITIONS_CONFIG_NET_DEFAULT_APN										"Vodafone"
#define POSITIONS_CONFIG_NET_DEFAULT_URL										"https://yourserver.com/endpoint"
#define POSITIONS_CONFIG_NET_DEFAULT_EXPECTED_RESPONSE							201
#define POSITIONS_CONFIG_NET_DEFAULT_UNAVAILABLE_NETWORK_POSTPONE_THRESHOLD		5


struct networkConfig_t {
	uint8_t saveThreshold;		//sizeof = 1
	uint16_t lastSavedEntry;	//sizeof = 2
	char apn[20];				//sizeof = 20
	char url[50];				//sizeof = 50
};								//sizeof = 73