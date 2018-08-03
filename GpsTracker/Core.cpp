#include "Core.h"
#include "Config.h"
#include "Flash.h"
#include "Alerts.h"

#define LOGGER_NAME "Core"
#define SMS_BUFFER_SIZE 100

using namespace utils;

namespace core {

	uint16_t sleepTime = SLEEP_DEFAULT_TIME_SECONDS;
	uint8_t stoppedInARow = SLEEP_DEFAULT_STOPPED_THRESHOLD - 1;

	void main() {
		bool forceBackup = false;
		bool acquired = false;
		PositionEntryMetadata metadata;

		positions::prepareBackup();
		acquired = positions::acquire(metadata);

		if (acquired) {
			positions::appendLast(metadata);
			
			forceBackup = updateSleepTime();
			gps::preserveCurrentCoordinates();
		}

		notifyFailures(metadata);
		alerts::clear(metadata);
		positions::doBackup(forceBackup);

		if (acquired) updateRtcTime();
		mainunit::deepSleep(sleepTime);
	}

	void notifyFailures(PositionEntryMetadata &metadata) {
		uint8_t triggered = alerts::getTriggered(metadata);
		uint8_t notified = 0;
		SIM808RegistrationStatus networkStatus;
		char buffer[SMS_BUFFER_SIZE] = "Alerts !\n";

		if (!triggered) return;

		network::powerOn();
		networkStatus = network::waitForRegistered(NETWORK_DEFAULT_TOTAL_TIMEOUT_MS);

		if (!network::isAvailable(networkStatus.stat)) return;

		if (bitRead(triggered, ALERT_BATTERY_LEVEL_1) || bitRead(triggered, ALERT_BATTERY_LEVEL_2)) {
			sprintf_P(buffer + strlen(buffer), PSTR(" - Battery at %d%%.\n"), metadata.batteryLevel);
		}
		
		if (bitRead(triggered, ALERT_RTC_TEMPERATURE_FAILURE)) {
			sprintf_P(buffer + strlen(buffer), PSTR(" - Temperature is %.2f°C. Backup battery failure ?\n"), metadata.batteryLevel);
		}

		//TODO : send sms, return if failed
		alerts::add(notified); //only add the successly notified failures
		//TODO : network::powerOff(); count "handles" like for i2c ?
	}

	void updateRtcTime() {
		tmElements_t time;
		gps::getTime(time);
		rtc::setTime(time);
	}

	bool updateSleepTime() {
		uint8_t velocity = gps::getVelocity();
		uint16_t result = mapSleepTime(velocity);
		bool goingLongSleep = false;

		if (velocity < SLEEP_TIMING_MIN_MOVING_VELOCITY) {
			float distance = gps::getDistanceFromPrevious(); //did we missed positions because we were sleeping ?
			if (distance > GPS_DEFAULT_MISSED_POSITION_GAP_KM) stoppedInARow = 0;
			else stoppedInARow = min(stoppedInARow + 1, SLEEP_DEFAULT_STOPPED_THRESHOLD + 1); //avoid overflow on REALLY long stops
			
			if (stoppedInARow < SLEEP_DEFAULT_STOPPED_THRESHOLD) {
				result = SLEEP_DEFAULT_PAUSING_TIME_SECONDS;
			}
			else if (stoppedInARow == SLEEP_DEFAULT_STOPPED_THRESHOLD) goingLongSleep = true;
		}
		else stoppedInARow = 0;

		sleepTime = result;
		NOTICE_FORMAT("updateSleepTime", "%dkmh => %d seconds", velocity, sleepTime);

		return goingLongSleep;
	}

	uint16_t mapSleepTime(uint8_t velocity) {
		uint16_t result;
		uint16_t currentTime = 0xFFFF;
		
		if (rtc::isAccurate()) {
			tmElements_t time;
			rtc::getTime(time);

			currentTime = SLEEP_TIMING_TIME(time.Hour, time.Minute);
		}
		
		for (uint8_t i = flash::getArraySize(config::defaultSleepTimings); i--;) {
			sleepTimings_t timing;
			flash::read(&config::defaultSleepTimings[i], timing);

			if (velocity < timing.speed) continue;
			if (currentTime != 0xFFFF && (currentTime < timing.timeMin || currentTime > timing.timeMax)) continue;

			result = timing.seconds;
			break;

		}

		VERBOSE_FORMAT("computeSleepTime", "%d,%d", velocity, result);
		return result;
	}
}