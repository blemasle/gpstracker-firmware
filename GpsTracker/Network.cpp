#include "Network.h"
#include "Hardware.h"
#define LOGGER_NAME "Network"

namespace network {

	/*inline */void powerOn() { hardware::sim808::networkPowerOn(); }
	/*inline */void powerOff() { hardware::sim808::networkPowerOff(); }
}