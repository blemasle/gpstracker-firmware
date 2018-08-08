#pragma once

#include <Arduino.h>
#include "config/BackupSd.h"

struct SdPositionConfig_t {
	uint8_t seed;
	uint8_t saveThreshold;
	uint8_t maxRecordsPerFile;
	uint16_t lastSavedEntry;
	uint16_t fileIndex;
	uint32_t filePosition;
	size_t fileRecords;
};

namespace config {
	namespace backup {
		namespace sd {

				void setup();

				SdPositionConfig_t get();
				void set(SdPositionConfig_t config);
				void reset();

		}
	}
}