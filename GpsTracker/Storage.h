#pragma once

#include "Hardware.h"

namespace storage {
	inline void powerOn() {
		hardware::i2c::powerOn();
	}

	inline void powerOff() {
		hardware::i2c::powerOff();
	}


}