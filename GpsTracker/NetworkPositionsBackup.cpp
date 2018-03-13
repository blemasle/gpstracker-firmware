#pragma once

#include "NetworkPositionsBackup.h"
#include "Debug.h"
#include "Positions.h"
#include "Config.h"
#include "Hardware.h"

#define LOGGER_NAME "Positions::backup::network"
#define BUFFER_SIZE 160
#define EXPECTED_HTTP_RESPONSE_CODE 201

namespace positions {
	namespace backup {
		namespace network {

			namespace details {

				bool isBackupNeeded(config_t &config) {
					config = config::main::get();

					return config.network.lastSavedEntry == 0xFFFF ||
						positions::count(config.network.lastSavedEntry) > config.network.saveThreshold;
				}

				bool appendPosition(config_t &config, PositionEntry &entry) {
					char buffer[BUFFER_SIZE];
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
					) == EXPECTED_HTTP_RESPONSE_CODE;
				}

				void appendPositions(config_t &config) {
					VERBOSE("appendPositions");

					uint16_t currentEntryIndex = config.network.lastSavedEntry + 1;
					PositionEntry currentEntry;

					hardware::i2c::powerOn();
					hardware::sim808::networkPowerOn();
					do {
						if (!positions::get(currentEntryIndex, currentEntry)) break;
						if (!appendPosition(config, currentEntry)) break;

						config.network.lastSavedEntry = currentEntryIndex;
						config::main::set(config);

					} while (positions::moveNext(currentEntryIndex));
					hardware::sim808::networkPowerOff();
					hardware::i2c::powerOff();
				}

			}

			void NetworkPositionsBackup::setup() {
				VERBOSE("backup");
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
