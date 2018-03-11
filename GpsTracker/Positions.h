#pragma once

#include <SIM808_Types.h>

#define POSITION_SIZE 115

struct PositionEntry {
	SIM808ChargingStatus battery;	//sizeof = 4
	SIM808_GPS_STATUS status;		//sizeof = 1
	char position[POSITION_SIZE];	//sizeof = 115
};									//sizeof = 119

namespace positions {
	void appendLast(const SIM808ChargingStatus battery, const SIM808_GPS_STATUS gpsStatus);

	void get(uint16_t index, PositionEntry &entry);
	bool moveNext(uint16_t &index);

	bool needsToSend();
	void send();
}