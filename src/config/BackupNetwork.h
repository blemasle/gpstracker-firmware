/**
 * Configure how the positions are backuped with the network
 */

#pragma once

#define POSITIONS_CONFIG_NET_DEFAULT_SAVE_THRESHOLD		30										///< Determines how many positions will be saved before a network backup is needed (only when not moving though).
#define POSITIONS_CONFIG_NET_DEFAULT_APN				"Vodafone"								///< APN used for GPRS context
#define POSITIONS_CONFIG_NET_DEFAULT_URL				"http://yourserver.com/endpoint"	///< URL to which positions data will be send through an HTTP POST request
#define POSITIONS_CONFIG_NET_DEFAULT_EXPECTED_RESPONSE	201										///< Expected response code from the server that indicates that the positions has been successfully backuped.
/**
 \def POSITIONS_CONFIG_NET_DEFAULT_UNAVAILABLE_NETWORK_POSTPONE_THRESHOLD
 Determines how many times to deal with an unreliable network before postponing the backup.
 In an area where cell reception isn't good, this avoid to try to backup the positions
 every single time, which would rapidly consumes all the batty.
*/
#define POSITIONS_CONFIG_NET_DEFAULT_UNAVAILABLE_NETWORK_POSTPONE_THRESHOLD		5

struct networkConfig_t {
	uint8_t saveThreshold;		//sizeof = 1
	uint16_t lastSavedEntry;	//sizeof = 2
	char apn[20];				//sizeof = 20
	char url[50];				//sizeof = 50
};								//sizeof = 73
