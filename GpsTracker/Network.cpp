#include "Config.h"
#include "Network.h"
#include "Hardware.h"
#include "MainUnit.h"

#define LOGGER_NAME "Network"

namespace network {

	SIM808RegistrationStatus waitForRegistered(uint16_t timeout) {
		SIM808RegistrationStatus currentStatus;

		do {
			currentStatus = hardware::sim808::device.getNetworkRegistrationStatus();
			if (isAvailable(currentStatus.stat)) break;

			mainunit::deepSleep(NETWORK_DEFAULT_INTERMEDIATE_TIMEOUT_MS / 1000);
			timeout -= NETWORK_DEFAULT_INTERMEDIATE_TIMEOUT_MS;
		} while (timeout > 1);

		return currentStatus;
	}

	bool isAvailable(SIM808_NETWORK_REGISTRATION_STATE state) {
		return state == SIM808_NETWORK_REGISTRATION_STATE::REGISTERED ||
			state == SIM808_NETWORK_REGISTRATION_STATE::ROAMING;
	}

	bool enableGprs() {
		return hardware::sim808::device.enableGprs(config::main::get().network.apn);
	}
	
	
}