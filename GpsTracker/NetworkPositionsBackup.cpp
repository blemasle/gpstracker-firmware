#pragma once

#include "Config.h"

#if BACKUP_ENABLE_NETWORK
#include "NetworkPositionsBackup.h"
#include "Debug.h"
#include "Positions.h"
#include "Hardware.h"
#include "Network.h"

#define LOGGER_NAME "Positions::backup::network"
#define BUFFER_SIZE 170

namespace positions {
	namespace backup {
		namespace net {

			namespace details {

				bool isBackupNeeded() {
					config_t *config = &config::main::value;
					return (config->network.lastSavedEntry == 0xFFFF && config ->lastEntry != 0xFFFF) ||
						positions::count(config->network.lastSavedEntry) > config->network.saveThreshold;
				}

				bool appendPosition(PositionEntry &entry) {
					VERBOSE("appendPosition");
					debug::displayFreeRam();

					char buffer[BUFFER_SIZE];
					snprintf(buffer, BUFFER_SIZE, "%d,%d,%d,%d,",
						entry.metadata.batteryLevel,
						entry.metadata.batteryVoltage,
						static_cast<uint16_t>(entry.metadata.temperature * 100),
						static_cast<uint8_t>(entry.metadata.status));

					strcat(buffer, entry.position);
					debug::displayFreeRam();

					return hardware::sim808::device.httpPost(
						config::main::value.network.url,
						F("text/csv"),
						buffer,
						buffer,
						BUFFER_SIZE
					) == POSITIONS_CONFIG_NET_DEFAULT_EXPECTED_RESPONSE;
				}

				void appendPositions() {
					VERBOSE("appendPositions");
					debug::displayFreeRam();

					uint16_t currentEntryIndex = config::main::value.network.lastSavedEntry + 1;
					PositionEntry currentEntry;
					SIM808RegistrationStatus networkStatus;

					network::powerOn();
					networkStatus = network::waitForRegistered(NETWORK_DEFAULT_TOTAL_TIMEOUT_MS);

					if (!network::isAvailable(networkStatus.stat)) VERBOSE_MSG("appendPositions", "network unavailable");
					else if (!network::enableGprs()) VERBOSE_MSG("appendPositions", "gprs unavailable");
					else {
						hardware::i2c::powerOn();
						do {
							if (!positions::get(currentEntryIndex, currentEntry)) break;
							debug::displayFreeRam();

							if (!appendPosition(currentEntry)) break;

							config::main::value.network.lastSavedEntry = currentEntryIndex;
							config::main::save();

						} while (positions::moveNext(currentEntryIndex));
						debug::displayFreeRam();
						hardware::i2c::powerOff();
					}

					network::powerOff();
					debug::displayFreeRam();
				}

			}

			void NetworkPositionsBackup::setup() {
				VERBOSE("setup");
			}

			void NetworkPositionsBackup::backup() {
				VERBOSE("backup");
				debug::displayFreeRam();

				if (!details::isBackupNeeded()) return;
				debug::displayFreeRam();

				details::appendPositions();
			}
		}
	}
}
#endif