#include "SdPositionsConfig.h"
#include "SdCard.h"
#include "Debug.h"

#define LOGGER_NAME "Config::backup::sd"

#if BACKUP_ENABLE_SDCARD
namespace config {
	namespace backup {
		namespace sd {

				SdPositionConfig_t value;
				File configFile;

				namespace details {

					void ensureOpened() {
						if (!configFile.isOpen()) {
							hardware::sdcard::filesystem.chdir();
							configFile.open(POSITIONS_CONFIG_FILENAME, O_RDWR | O_CREAT);
						}

						configFile.rewind();
					}

					void read() {
						VERBOSE("read");
						ensureOpened();
						configFile.read((void*)&value, sizeof(value));
						if (value.seed != POSITIONS_CONFIG_SEED) reset();
					}

					void write() {
						VERBOSE("write");
						ensureOpened();
						configFile.write((void*)&value, sizeof(value));
					}

				}

				SdPositionConfig_t get() {
					if (value.seed != POSITIONS_CONFIG_SEED) details::read();

					return value;
				}

				void set(SdPositionConfig_t config) {
					value = config;
					details::write();
				}

				void reset() {
					VERBOSE("reset");
					SdPositionConfig_t config = {
						POSITIONS_CONFIG_SEED,
						POSITIONS_CONFIG_DEFAULT_SAVE_THRESHOLD,
						POSITIONS_CONFIG_DEFAULT_MAX_RECORDS_PER_FILE,
						0xFFFF,
						0,
						0,
						0
					};

					value = config;
					details::write();
				}

		}
	}
}
#endif