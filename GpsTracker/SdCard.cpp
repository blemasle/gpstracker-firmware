#include "SdCard.h"

namespace hardware {
	namespace sdcard {

		SdFat filesystem;

		void setup() {
			filesystem.begin(SD_SS);
		}

	}
}