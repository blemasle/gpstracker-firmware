#include "Config.h"

#ifdef BACKUP_ENABLE_SDCARD
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
#endif