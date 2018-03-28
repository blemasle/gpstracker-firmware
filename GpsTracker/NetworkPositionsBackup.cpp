#pragma once

#include "Config.h"

#if BACKUP_ENABLE_NETWORK
#include "NetworkPositionsBackup.h"
#include "Debug.h"
#include "Hardware.h"
#include "Network.h"

#define LOGGER_NAME "Positions::backup::network"
#define BUFFER_SIZE 170

namespace positions {
	namespace backup {
		namespace net {

			bool NetworkPositionsBackup::isBackupNeeded(bool forPrepare) {
				config_t *config = &config::main::value;
				return (config->network.lastSavedEntry == 0xFFFF && config->lastEntry != 0xFFFF) ||
					positions::count(config->network.lastSavedEntry) > config->network.saveThreshold - (forPrepare ? 1 : 0);
			}

			bool NetworkPositionsBackup::appendPosition(PositionEntry &entry) {
				char buffer[BUFFER_SIZE];
				snprintf(buffer, BUFFER_SIZE, "%d,%d,%d,%d,%d,%d,%d,",
					debug::freeRam(),
					hardware::sim808::device.getSignalQuality().attenuation,
					entry.metadata.batteryLevel,
					entry.metadata.batteryVoltage,
					static_cast<uint16_t>(entry.metadata.temperature * 100),
					static_cast<uint8_t>(entry.metadata.status),
					entry.metadata.timeToFix);

				strcat(buffer, entry.position);

				NOTICE_FORMAT("appendPosition", "Sending : %s", buffer);
				uint16_t responseCode = hardware::sim808::device.httpPost(
					config::main::value.network.url,
					F("text/gpstracker"),
					buffer,
					buffer,
					BUFFER_SIZE
				) == POSITIONS_CONFIG_NET_DEFAULT_EXPECTED_RESPONSE;

				NOTICE_FORMAT("appendPosition", "Response : %d", responseCode);
				return responseCode;
			}

			//__attribute__((__optimize__("O2")))
			void NetworkPositionsBackup::appendPositions() {
				uint16_t currentEntryIndex = config::main::value.network.lastSavedEntry + 1;
				uint32_t networkTimeout = 0;
				PositionEntry currentEntry;
				SIM808RegistrationStatus networkStatus;

				network::powerOn();
				networkTimeout = NETWORK_DEFAULT_TOTAL_TIMEOUT_MS;
				if (_prepareTime > 0) networkTimeout -= (rtc::getTime() - _prepareTime) * 1000;

				networkStatus = network::waitForRegistered(networkTimeout);

				if (!network::isAvailable(networkStatus.stat)) NOTICE_MSG("appendPositions", "network unavailable");
				else if (!network::enableGprs()) NOTICE_MSG("appendPositions", "gprs unavailable");
				else {
					hardware::i2c::powerOn();
					do {
						if (!positions::get(currentEntryIndex, currentEntry)) break;
						if (!appendPosition(currentEntry)) break;

						config::main::value.network.lastSavedEntry = currentEntryIndex;
						config::main::save();

					} while (positions::moveNext(currentEntryIndex));
					hardware::i2c::powerOff();
				}

				network::powerOff();
			}

			void NetworkPositionsBackup::setup() {
				NOTICE("setup");
			}

			void NetworkPositionsBackup::prepare() {
				NOTICE("prepare");

				if (!isBackupNeeded(true)) {
					_prepareTime = 0;
					return;
				}

				network::powerOn();
				_prepareTime = rtc::getTime();
			}

			void NetworkPositionsBackup::backup() {
				NOTICE("backup");

				if (!isBackupNeeded(false)) return;
				appendPositions();
			}
		}
	}
}
#endif