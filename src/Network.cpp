#include "Config.h"

#include "Network.h"
#include "Hardware.h"
#include "MainUnit.h"
#include "Rtc.h"
#include "Logging.h"

#define LOGGER_NAME "Network"

namespace network {

	timestamp_t _poweredOnTime;

	void powerOn() {
		hardware::sim808::networkPowerOn();
		_poweredOnTime = rtc::getTime();
	}

	void powerOff() {
		hardware::sim808::networkPowerOff();
		_poweredOnTime = 0;
	}

	__attribute__((__optimize__("O2")))
	SIM808RegistrationStatus waitForRegistered(uint32_t timeout, bool relativeToPowerOnTime = true) {

		SIM808RegistrationStatus currentStatus;
		SIM808SignalQualityReport report;
		uint8_t noReliableNetwork = 0;

		if (relativeToPowerOnTime) timeout -= (rtc::getTime() - _poweredOnTime) * 1000;

		currentStatus = hardware::sim808::device.getNetworkRegistrationStatus();
		report = hardware::sim808::device.getSignalQuality();

		do {
			if (isAvailable(currentStatus.stat)) break;

			NOTICE_FORMAT("waitForRegistered", "%d, [%d %ddBm]", currentStatus.stat, report.ssri, report.attenuation);

			if (report.ssri < NETWORK_DEFAULT_NO_NETWORK_QUALITY_THRESHOLD) noReliableNetwork++;
			else noReliableNetwork = 0;
			if (noReliableNetwork > NETWORK_DEFAULT_NO_NETWORK_TRIES) {
				NOTICE_MSG("waitForRegistered", "No reliable signal");
				break; //after a while, no network really means no network. Bailing out
			}

			mainunit::deepSleep(NETWORK_DEFAULT_INTERMEDIATE_TIMEOUT_MS / 1000);
			timeout -= NETWORK_DEFAULT_INTERMEDIATE_TIMEOUT_MS;

			currentStatus = hardware::sim808::device.getNetworkRegistrationStatus();
			report = hardware::sim808::device.getSignalQuality();
		} while (timeout > 1);

		NOTICE_FORMAT("waitForRegistered", "%d, [%d %ddBm]", currentStatus.stat, report.ssri, report.attenuation);
		return currentStatus;
	}

	bool isAvailable(SIM808_NETWORK_REGISTRATION_STATE state) {
		return static_cast<int8_t>(state) &
			(static_cast<int8_t>(SIM808_NETWORK_REGISTRATION_STATE::REGISTERED) | static_cast<int8_t>(SIM808_NETWORK_REGISTRATION_STATE::ROAMING))
			!= 0;
	}

	bool enableGprs() {
		return hardware::sim808::device.enableGprs(config::main::value.network.apn);
	}

	bool sendSms(const char * msg) {
		const char * phoneNumber = config::main::value.contactPhone;

		NOTICE_FORMAT("sendSms", "%s, %s", phoneNumber, msg);
		return hardware::sim808::device.sendSms(phoneNumber, msg);
	}
}