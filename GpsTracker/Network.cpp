#include "Config.h"

#if BACKUP_ENABLE_NETWORK
#include "Debug.h"
#include "Network.h"
#include "Hardware.h"
#include "MainUnit.h"

#define LOGGER_NAME "Network"

namespace network {

	__attribute__((__optimize__("O2")))
	SIM808RegistrationStatus waitForRegistered(uint32_t timeout) {

		SIM808RegistrationStatus currentStatus;
		SIM808SignalQualityReport report;
		uint8_t noReliableNetwork = 0;

		do {
			currentStatus = hardware::sim808::device.getNetworkRegistrationStatus();
			if (isAvailable(currentStatus.stat)) break;

			report = hardware::sim808::device.getSignalQuality();
			NOTICE_FORMAT("waitForRegistered", "%d, [%d %ddBm]", currentStatus.stat, report.ssri, report.attenuation);
			
			if (report.ssri < NETWORK_DEFAULT_NO_NETWORK_QUALITY_THRESHOLD) noReliableNetwork++;
			else noReliableNetwork = 0;
			if (noReliableNetwork > NETWORK_DEFAULT_NO_NETWORK_TRIES) {
				NOTICE_MSG("waitForRegistered", "No reliable signal");
				break; //after a while, not network really means no network. Bailing out
			}

			mainunit::deepSleep(NETWORK_DEFAULT_INTERMEDIATE_TIMEOUT_MS / 1000);
			timeout -= NETWORK_DEFAULT_INTERMEDIATE_TIMEOUT_MS;

		} while (timeout > 1);

		report = hardware::sim808::device.getSignalQuality();
		NOTICE_FORMAT("waitForRegistered", "%d, [%d %ddBm]", currentStatus.stat, report.ssri, report.attenuation);
		return currentStatus;
	}

	bool isAvailable(SIM808_NETWORK_REGISTRATION_STATE state) {
		return state == SIM808_NETWORK_REGISTRATION_STATE::REGISTERED ||
			state == SIM808_NETWORK_REGISTRATION_STATE::ROAMING;
	}

	bool enableGprs() {
		return hardware::sim808::device.enableGprs(config::main::value.network.apn);
	}
	
	
}
#endif