#pragma once

#include "NetworkPositionsBackup.h"
#include "Debug.h"
#include "Positions.h"
#include "Config.h"
#include "Hardware.h"
#include "Network.h"

#define LOGGER_NAME "Positions::backup::network"
#define BUFFER_SIZE 160

namespace positions {
	namespace backup {
		namespace net {

			namespace details {

				bool isBackupNeeded(config_t &config) {
					config = config::main::get();

					return config.network.lastSavedEntry == 0xFFFF ||
						positions::count(config.network.lastSavedEntry) > config.network.saveThreshold;
				}

				bool appendPosition(config_t &config, PositionEntry &entry) {
					debug::displayFreeRam();
					return false;
					/*char buffer[BUFFER_SIZE];
					snprintf_P(buffer, BUFFER_SIZE, PSTR("%d,%d,%.2f,%d,%s,%d"),
						entry.metadata.batteryLevel,
						entry.metadata.batteryVoltage,
						entry.metadata.temperature,
						static_cast<uint8_t>(entry.metadata.status),
						entry.position);

					return hardware::sim808::device.httpPost(
						config.network.url,
						PSTR("text/csv"),
						buffer,
						buffer,
						BUFFER_SIZE
					) == POSITIONS_CONFIG_NET_DEFAULT_EXPECTED_RESPONSE;*/
				}

				void appendPositions(config_t &config) {
					VERBOSE("appendPositions");

					uint16_t currentEntryIndex = config.network.lastSavedEntry + 1;
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
							if (!appendPosition(config, currentEntry)) break;

							config.network.lastSavedEntry = currentEntryIndex;
							config::main::set(config);

						} while (positions::moveNext(currentEntryIndex));
						hardware::i2c::powerOff();
					}

					network::powerOff();
				}

			}

			void NetworkPositionsBackup::setup() {
				VERBOSE("setup");
			}

			void NetworkPositionsBackup::backup() {
				VERBOSE("backup");

				config_t config;
				if (!details::isBackupNeeded(config)) return;
				details::appendPositions(config);
			}
		}
	}
}
