#include "Config.h"

#if BACKUP_ENABLE_SDCARD
#include "SdCard.h"
#include "config/Pins.h"

namespace hardware {
	namespace sdcard {

		SdFat filesystem;
		bool available = false;

		void setup() {
			available = filesystem.begin(SD_SS);
		}

	}
}
#endif