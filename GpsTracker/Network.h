#pragma once

#include "Hardware.h"

namespace network {

	inline void powerOn() { hardware::sim808::networkPowerOn(); }
	inline void powerOff() { hardware::sim808::networkPowerOff(); }

	SIM808RegistrationStatus waitForRegistered(int32_t timeout);
	bool isAvailable(SIM808_NETWORK_REGISTRATION_STATE state);
	bool enableGprs();
}