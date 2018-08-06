#pragma once

#include "Hardware.h"

namespace network {

	void powerOn();
	void powerOff();

	SIM808RegistrationStatus waitForRegistered(uint32_t timeout, bool relativeToPowerOnTime = true);
	bool isAvailable(SIM808_NETWORK_REGISTRATION_STATE state);
	bool enableGprs();

	bool sendSms(const char * msg);
}