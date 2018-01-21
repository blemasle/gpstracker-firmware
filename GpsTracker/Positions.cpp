#include "Positions.h"

#include "Gps.h"
#include "Network.h"
#include "Storage.h"

#define LOGGER_NAME "Positions"

namespace positions {

	void appendLast() {
		//write gps::lastPosition to eeprom and eventually to sd card
	}

	bool needsToSend() {
		return false;
	}

	void send() {

	}
}