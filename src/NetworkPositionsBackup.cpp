#pragma once

#include "Config.h"

#if BACKUP_ENABLE_NETWORK
#include "NetworkPositionsBackup.h"
#include "MainUnit.h"
#include "Hardware.h"
#include "Network.h"
#include "Logging.h"

#define BUFFER_SIZE 170


namespace positions {
	#define CURRENT_LOGGER "Positions::backup::network"

	namespace backup {
		namespace net {

			uint8_t networkUnavailableInARow = 0;
			uint8_t networkUnavailablePostpone = 1;

			bool NetworkPositionsBackup::isBackupNeeded(bool forPrepare) {
				config_t *config = &config::main::value;
				return (config->network.lastSavedEntry == 0xFFFF && config->lastEntry != 0xFFFF) ||
					positions::count(config->network.lastSavedEntry) > (config->network.saveThreshold * networkUnavailablePostpone) - (forPrepare ? 1 : 0);
			}

			bool NetworkPositionsBackup::appendPosition(PositionEntry &entry, int8_t signalAttenuation = 0) {
				#define CURRENT_LOGGER_FUNCTION "appendPosition"

				char buffer[BUFFER_SIZE];
				if(signalAttenuation == 0) signalAttenuation = hardware::sim808::device.getSignalQuality().attenuation;

				snprintf_P(buffer, BUFFER_SIZE, PSTR("%d,%d,%d,%d,%d,%d,%d,%s"),
					mainunit::freeRam(),
					signalAttenuation,
					entry.metadata.batteryLevel,
					entry.metadata.batteryVoltage,
					entry.metadata.temperature,
					static_cast<uint8_t>(entry.metadata.status),
					entry.metadata.timeToFix,
					entry.position);

				NOTICE_FORMAT("Sending : %s", buffer);
				uint16_t responseCode = hardware::sim808::device.httpPost(
					config::main::value.network.url,
					F("text/gpstracker"),
					buffer,
					buffer,
					BUFFER_SIZE
				);

				NOTICE_FORMAT("Response : %d", responseCode);
				return responseCode == POSITIONS_CONFIG_NET_EXPECTED_RESPONSE;
			}

			void NetworkPositionsBackup::appendPositions() {
				#define CURRENT_LOGGER_FUNCTION "appendPositions"

				uint16_t currentEntryIndex = config::main::value.network.lastSavedEntry + 1;
				PositionEntry currentEntry;
				SIM808_NETWORK_REGISTRATION_STATE networkStatus;

				//avoid edge case where if 0, whole set of positions will be sent again
				if (!positions::count(config::main::value.network.lastSavedEntry)) return;

				network::powerOn();
				networkStatus = network::waitForRegistered(NETWORK_TOTAL_TIMEOUT_MS);

				if (networkStatus == SIM808_NETWORK_REGISTRATION_STATE::ERROR ||
					(!network::isAvailable(networkStatus) || !network::enableGprs())) {
					networkUnavailableInARow = min(networkUnavailableInARow + 1, POSITIONS_CONFIG_NET_UNAVAILABLE_NETWORK_POSTPONE_THRESHOLD + 1); //avoid increment overflow
					NOTICE_MSG("network or gprs unavailable");

					if (networkUnavailableInARow > POSITIONS_CONFIG_NET_UNAVAILABLE_NETWORK_POSTPONE_THRESHOLD) {
						networkUnavailablePostpone++;
					}
				}
				else {
					networkUnavailableInARow = 0;
					networkUnavailablePostpone = 1;

					int8_t signalAttenuation = hardware::sim808::device.getSignalQuality().attenuation;
					hardware::i2c::powerOn();
					do {
						if (!positions::get(currentEntryIndex, currentEntry)) break;
						if (!appendPosition(currentEntry, signalAttenuation)) break;

						config::main::value.network.lastSavedEntry = currentEntryIndex;
						config::main::save();

					} while (positions::moveNext(currentEntryIndex));
					hardware::i2c::powerOff();
				}

				network::powerOff();
			}

			void NetworkPositionsBackup::setup() {}

			void NetworkPositionsBackup::prepare(bool force) {
				#define CURRENT_LOGGER_FUNCTION "prepare"

				if (!(force || isBackupNeeded(true))) return;

				NOTICE;
				network::powerOn();
			}

			void NetworkPositionsBackup::backup(bool force) {
				#define CURRENT_LOGGER_FUNCTION "backup"

				if (!(force || isBackupNeeded(false))) return;

				NOTICE;
				appendPositions();
				network::powerOff();
			}
		}
	}
}
#endif