#pragma once

#include <SdFat.h>

namespace hardware {
	namespace sdcard {

		extern SdFat filesystem;
		extern bool available;

		void setup();
	}
}