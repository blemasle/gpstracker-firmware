#include "Network.h"
#include "Hardware.h"
#include "MainUnit.h"
#include "Rtc.h"
#include "Config.h"
#include "Logging.h"

namespace network {
	#define CURRENT_LOGGER "network"
	timestamp_t _poweredOnTime;

	namespace details {

		void print(SIM808_NETWORK_REGISTRATION_STATE state, SIM808SignalQualityReport &report) {
			#define CURRENT_LOGGER_FUNCTION "print"
			NOTICE_FORMAT("%d, [%d %ddBm]", state, report.rssi, report.attenuation);
		}

	}
	void powerOn() {
		hardware::sim808::networkPowerOn();
		_poweredOnTime = rtc::getTime();
	}

	void powerOff() {
		hardware::sim808::networkPowerOff();
		_poweredOnTime = 0;
	}

	SIM808_NETWORK_REGISTRATION_STATE waitForRegistered(uint32_t timeout, bool relativeToPowerOnTime = true) {
		#define CURRENT_LOGGER_FUNCTION "waitForRegistered"
		NOTICE;

		SIM808_NETWORK_REGISTRATION_STATE currentStatus;
		SIM808SignalQualityReport report;
		uint8_t noReliableNetwork = 0;

		if (relativeToPowerOnTime) timeout -= (rtc::getTime() - _poweredOnTime) * 1000;

		currentStatus = hardware::sim808::device.getNetworkRegistrationStatus();
		report = hardware::sim808::device.getSignalQuality();

		do {
			if (isAvailable(currentStatus)) break;
			details::print(currentStatus, report);

			if (report.rssi < NETWORK_DEFAULT_NO_NETWORK_QUALITY_THRESHOLD) noReliableNetwork++;
			else noReliableNetwork = 0;
			if (noReliableNetwork > NETWORK_DEFAULT_NO_NETWORK_TRIES) {
				NOTICE_MSG("No reliable signal");
				break; //after a while, no network really means no network. Bailing out
			}

			mainunit::deepSleep(NETWORK_DEFAULT_INTERMEDIATE_TIMEOUT_MS / 1000);
			timeout -= NETWORK_DEFAULT_INTERMEDIATE_TIMEOUT_MS;

			currentStatus = hardware::sim808::device.getNetworkRegistrationStatus();
			report = hardware::sim808::device.getSignalQuality();
		} while (timeout > 1);

		details::print(currentStatus, report);
		return currentStatus;
	}

	bool isAvailable(SIM808_NETWORK_REGISTRATION_STATE state) {
		return static_cast<int8_t>(state) &
			(static_cast<int8_t>(SIM808_NETWORK_REGISTRATION_STATE::REGISTERED) | static_cast<int8_t>(SIM808_NETWORK_REGISTRATION_STATE::ROAMING))
			!= 0;
	}

#if BACKUP_ENABLE_NETWORK
	bool enableGprs() {
		return hardware::sim808::device.enableGprs(config::main::value.network.apn);
	}
#endif

	bool sendSms(const char * msg) {
		#define CURRENT_LOGGER_FUNCTION "sendSms"
		const char * phoneNumber = config::main::value.contactPhone;

		NOTICE_FORMAT("%s, %s", phoneNumber, msg);
		return hardware::sim808::device.sendSms(phoneNumber, msg);
	}
}