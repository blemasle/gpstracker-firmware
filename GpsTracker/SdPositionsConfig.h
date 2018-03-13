#pragma once

#include <Arduino.h>

#define POSITIONS_FOLDER	"positions"
#define POSITIONS_FILENAME	"positions-%05d.csv"
#define POSITIONS_FILENAME_LENGTH 19
#define POSITIONS_CONFIG_FILENAME "positions.config"
#define POSITIONS_CONFIG_SEED 45
#define POSITIONS_CONFIG_DEFAULT_SAVE_THRESHOLD 10
#define POSITIONS_CONFIG_DEFAULT_MAX_RECORDS_PER_FILE 5

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