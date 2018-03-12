#pragma once

#include <SdFat.h>
#include "Pins.h"

namespace hardware {
	namespace sdcard {

		extern SdFat filesystem;
		extern bool available;

		void setup();
	}
}