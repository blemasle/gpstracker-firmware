#include "SdPositionsConfig.h"
#include "SdCard.h"
#include "Logging.h"


#if BACKUP_ENABLE_SDCARD
namespace config {
	#define CURRENT_LOGGER "config::backup::sd"

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
						#define CURRENT_LOGGER_FUNCTION "read"
						VERBOSE;

						ensureOpened();
						configFile.read((void*)&value, sizeof(value));
						if (value.seed != POSITIONS_CONFIG_SEED) reset();
					}

					void write() {
						#define CURRENT_LOGGER_FUNCTION "write"
						VERBOSE;

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
					#define CURRENT_LOGGER_FUNCTION "reset"
					VERBOSE;

					SdPositionConfig_t config = {
						POSITIONS_CONFIG_SEED,
						POSITIONS_CONFIG_SAVE_THRESHOLD,
						POSITIONS_CONFIG_MAX_RECORDS_PER_FILE,
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