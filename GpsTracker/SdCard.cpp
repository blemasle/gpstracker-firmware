#include "SdCard.h"

namespace hardware {
	namespace sdcard {

		SdFat filesystem;
		bool available = false;

		void setup() {
			available = filesystem.begin(SD_SS);
		}

	}
}