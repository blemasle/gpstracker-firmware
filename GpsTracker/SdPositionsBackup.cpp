#include "SdPositionsBackup.h"
#include "SdPositionsConfig.h"
#include "SdCard.h"
#include "Hardware.h"
#include "Config.h"
#include "SdFat.h"
#include "Positions.h"

namespace positions {
	namespace backup {
		namespace sd {

			namespace details {

				bool isBackupNeeded(SdPositionConfig_t &sdConfig) {
					Config_t referenceConfig = config::main::get();
					sdConfig = config::backup::sd::get();

					return sdConfig.lastSavedEntry == 0xFFFF ||
						positions::count(sdConfig.lastSavedEntry) > sdConfig.saveThreshold;
				}

				void appendPositions(const File &file, SdPositionConfig_t &sdConfig) {
					uint16_t currentEntryIndex = sdConfig.lastSavedEntry + 1;
					PositionEntry currentEntry;

					hardware::i2c::powerOn();
					do {
						if (!positions::get(currentEntryIndex, currentEntry)) break;

						sdConfig.filePosition = file.position();
					} while (true);
					hardware::i2c::powerOff();

				}
			}

			void SdPositionsBackup::setup() {
				config::backup::sd::setup();
			}

			void SdPositionsBackup::backup() {
				SdPositionConfig_t sdConfig;

				if (!details::isBackupNeeded(sdConfig)) return;

				char *filename;
				hardware::sdcard::filesystem.chdir("positions");
				File file = hardware::sdcard::filesystem.open(filename, O_RDWR | O_CREAT);

				file.seek(sdConfig.filePosition);
				file.find('\n');

				details::appendPositions(file, sdConfig);
			}

		}
	}
}