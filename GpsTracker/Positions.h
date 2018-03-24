#pragma once

#include <SIM808_Types.h>

#define POSITION_SIZE 115

struct PositionEntryMetadata {
	int8_t batteryLevel;			//sizeof = 1
	int16_t batteryVoltage;			//sizeof = 2
	float temperature;				//sizeof = 4
	uint16_t timeToFix;				//sizeof = 2
	SIM808_GPS_STATUS status;		//sizeof = 1
};									//sizeof = 10

struct PositionEntry {
	PositionEntryMetadata metadata;	//sizeof = 10
	char position[POSITION_SIZE];	//sizeof = 115
};									//sizeof = 125

namespace positions {

	void setup();
	bool acquire(PositionEntryMetadata &metadata);
	void appendLast(const PositionEntryMetadata &metadata);

	bool get(uint16_t index, PositionEntry &entry);
	bool moveNext(uint16_t &index);
	uint16_t count(uint16_t fromIndex);

	void prepareBackup();
	void doBackup();
}