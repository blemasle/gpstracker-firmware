#pragma once

#include "Hardware.h"

namespace network {

	inline void powerOn() { hardware::sim808::networkPowerOn(); }
	inline void powerOff() { hardware::sim808::networkPowerOff(); }
}