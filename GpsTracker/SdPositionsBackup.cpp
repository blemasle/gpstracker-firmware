#include "SdPositionsBackup.h"
#include "SdPositionsConfig.h"
#include "SdCard.h"

#include "Config.h"

namespace positions {
	namespace backup {
		namespace sd {

			void SdPositionsBackup::setup() {
				config::backup::sd::setup();
				_config = new RawSdFile(&hardware::sdcard::filesystem, POSITIONS_CONFIG_FILENAME);
			}

			void SdPositionsBackup::backup() {
				Config referenceConfig = config::main::get();

			}

		}
	}
}