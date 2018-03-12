#pragma once

#include <Arduino.h>

struct SdPositionConfig_t {
	uint16_t fileIndex;
	uint32_t position;
	size_t size;
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