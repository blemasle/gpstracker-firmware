#pragma once

#include <Arduino.h>
#include "Hardware.h"

namespace network {

	void powerOn();
	void powerOff();

	SIM808_NETWORK_REGISTRATION_STATE waitForRegistered(uint32_t timeout, bool relativeToPowerOnTime = true);
	bool isAvailable(SIM808_NETWORK_REGISTRATION_STATE state);
#if BACKUP_ENABLE_NETWORK
	bool enableGprs();
#endif

	bool sendSms(const char * msg);
}