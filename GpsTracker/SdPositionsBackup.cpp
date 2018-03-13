#include "SdPositionsBackup.h"
#include "SdPositionsConfig.h"
#include "SdCard.h"
#include "Hardware.h"
#include "Config.h"
#include "Positions.h"
#include "Debug.h"

#define LOGGER_NAME "Positions::backup::sd"

#if BACKUP_ENABLE_SDCARD
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

				void getPositionsFileName(uint16_t fileIndex, char *buffer) {
					sprintf(buffer, POSITIONS_FILENAME, fileIndex);
				}

				void ensurePositionsFolder() {
					char positionsFolder[] = POSITIONS_FOLDER;

					hardware::sdcard::filesystem.chdir();
					if (!hardware::sdcard::filesystem.exists(positionsFolder)) {
						hardware::sdcard::filesystem.mkdir(positionsFolder, true);
					}

					hardware::sdcard::filesystem.chdir(positionsFolder);
				}

				void selectFile(SdPositionConfig_t &sdConfig, File &file) {
					char positionFileName[POSITIONS_FILENAME_LENGTH];

					if (sdConfig.fileRecords >= sdConfig.maxRecordsPerFile) {
						if (file.isOpen()) {
							file.close();
							config::backup::sd::set(sdConfig);
						}

						sdConfig.fileIndex++;
						sdConfig.filePosition = 0;
						sdConfig.fileRecords = 0;
					}

					if (!file.isOpen()) {
						ensurePositionsFolder();
						getPositionsFileName(sdConfig.fileIndex, positionFileName);
						file.open(positionFileName, O_RDWR | O_CREAT);
					}
				}

				void appendPosition(File &file, SdPositionConfig_t &sdConfig, PositionEntry &entry) {
					VERBOSE("appendPosition");

					const char fieldTerminator = ',';

					file.printField(entry.metadata.batteryLevel, fieldTerminator);
					file.printField(entry.metadata.batteryVoltage, fieldTerminator);
					file.printField(entry.metadata.temperature, fieldTerminator);
					file.printField(entry.metadata.timeToFix, fieldTerminator);
					file.printField(static_cast<uint8_t>(entry.metadata.status), fieldTerminator);
					file.println(entry.position);

					sdConfig.filePosition = file.position();
				}

				void appendPositions(SdPositionConfig_t &sdConfig) {
					VERBOSE("appendPositions");

					uint16_t currentEntryIndex = sdConfig.lastSavedEntry + 1;
					PositionEntry currentEntry;
					File file;

					hardware::i2c::powerOn();
					do {
						if (!positions::get(currentEntryIndex, currentEntry)) break;
						selectFile(sdConfig, file);
						appendPosition(file, sdConfig, currentEntry);
						sdConfig.lastSavedEntry = currentEntryIndex;
					} while (positions::moveNext(currentEntryIndex));
					hardware::i2c::powerOff();

					if (file.isOpen()) file.close();
					config::backup::sd::set(sdConfig);
				}
			}

			void SdPositionsBackup::setup() {
				hardware::sdcard::setup();
			}

			void SdPositionsBackup::backup() {
				VERBOSE("backup");

				if (!hardware::sdcard::available) {
					VERBOSE_MSG("backup", "not available");
					return;
				}

				SdPositionConfig_t sdConfig;

				if (!details::isBackupNeeded(sdConfig)) return;
				details::appendPositions(sdConfig);
			}

		}
	}
}
#endif