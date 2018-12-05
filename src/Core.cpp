#include "Core.h"
#include "Config.h"
#include "Flash.h"
#include "MainUnit.h"
#include "Gps.h"
#include "Network.h"
#include "Rtc.h"
#include "Alerts.h"
#include "Logging.h"

#define SMS_BUFFER_SIZE		140
#define NO_ALERTS_NOTIFIED	0

using namespace utils;

namespace core {
	#define CURRENT_LOGGER "core"

	uint16_t sleepTime = SLEEP_DEFAULT_TIME_SECONDS;
	uint8_t stoppedInARow = SLEEP_DEFAULT_STOPPED_THRESHOLD - 1;
	TRACKER_MOVING_STATE movingState = TRACKER_MOVING_STATE::MOVING;

	namespace details {

		void appendToSmsBuffer(char * buffer, const char * fmt, ...) {
			va_list args;
			va_start(args, fmt);

			size_t bufferLeft = SMS_BUFFER_SIZE - strlen(buffer);
			char * p = buffer + strlen(buffer);
			vsnprintf_P(p, bufferLeft, fmt, args);

			va_end(args);
		}

	}

	void main() {
		bool acquired = false;
		PositionEntryMetadata metadata;

		if(movingState >= TRACKER_MOVING_STATE::ABOUT_TO_STOP) {
			//forcing when the tracker is about to stop (which should result in STOPPED a few lines below)
			positions::prepareBackup(movingState == TRACKER_MOVING_STATE::ABOUT_TO_STOP);
		}

		acquired = positions::acquire(metadata);

		if (acquired) {
			positions::appendLast(metadata);

			movingState = updateSleepTime();
			gps::preserveCurrentCoordinates();
		}

		alerts::clear(metadata);
		alerts::add(notifyFailures(metadata));

		if(movingState >= TRACKER_MOVING_STATE::STOPPED) {
			positions::doBackup(movingState == TRACKER_MOVING_STATE::STOPPED); //forcing at the moment the tracker stop
		}

		if (acquired) updateRtcTime();
		mainunit::deepSleep(sleepTime);
	}

	uint8_t notifyFailures(PositionEntryMetadata &metadata) {
		#define CURRENT_LOGGER_FUNCTION "notifyFailures"

		SIM808_NETWORK_REGISTRATION_STATE networkStatus;
		char buffer[SMS_BUFFER_SIZE];
		const __FlashStringHelper * backupFailureString = F(" Backup battery failure ?");
		bool notified = false;

		uint8_t triggered = alerts::getTriggered(metadata);
		if (!triggered) return NO_ALERTS_NOTIFIED;

		NOTICE_FORMAT("triggered : %B", triggered);

		network::powerOn();
		networkStatus = network::waitForRegistered(NETWORK_DEFAULT_TOTAL_TIMEOUT_MS);

		if (network::isAvailable(networkStatus)) {
			strncpy_P(buffer, PSTR("Alerts !"), SMS_BUFFER_SIZE);
			if (bitRead(triggered, ALERT_BATTERY_LEVEL_1) || bitRead(triggered, ALERT_BATTERY_LEVEL_2)) {
				details::appendToSmsBuffer(buffer, PSTR("\n- Battery at %d%%."), metadata.batteryLevel);
			}

			if (bitRead(triggered, ALERT_RTC_TEMPERATURE_FAILURE)) {
				details::appendToSmsBuffer(buffer, PSTR("\n- Temperature is %dC.%S"), metadata.temperature, backupFailureString);
			}

			if (bitRead(triggered, ALERT_RTC_CLOCK_FAILURE)) {
				details::appendToSmsBuffer(buffer, PSTR("\n- RTC was stopped.%S"), backupFailureString);
			}

#if ALERTS_ON_SERIAL
			NOTICE_FORMAT("%s", buffer);
			notified = true;
#else
			notified = network::sendSms(buffer);
#endif
			if (!notified) NOTICE_MSG("SMS not sent !");
		}

		network::powerOff();
		return notified ? triggered : NO_ALERTS_NOTIFIED; //If not notified, the alerts state should not be persisted (so we can retry to notify them)
	}

	void updateRtcTime() {
		tmElements_t time;
		gps::getTime(time);
		rtc::setTime(time);
	}

	TRACKER_MOVING_STATE updateSleepTime() {
		#define CURRENT_LOGGER_FUNCTION "updateSleepTime"

		TRACKER_MOVING_STATE state = TRACKER_MOVING_STATE::MOVING;
		uint8_t velocity = gps::getVelocity();

		sleepTime = mapSleepTime(velocity);

		if (velocity < SLEEP_TIMING_MIN_MOVING_VELOCITY) {
			float distance = gps::getDistanceFromPrevious(); //did we missed positions because we were sleeping ?
			if (distance > GPS_DEFAULT_MISSED_POSITION_GAP_KM) stoppedInARow = 0;
			else stoppedInARow = min(stoppedInARow + 1, SLEEP_DEFAULT_STOPPED_THRESHOLD + 1); //avoid overflow on REALLY long stops

			if (stoppedInARow < SLEEP_DEFAULT_STOPPED_THRESHOLD) {
				sleepTime = SLEEP_DEFAULT_PAUSING_TIME_SECONDS;
				state = stoppedInARow == SLEEP_DEFAULT_STOPPED_THRESHOLD - 1 ?
					TRACKER_MOVING_STATE::ABOUT_TO_STOP :
					TRACKER_MOVING_STATE::PAUSED;
			}
			else if (stoppedInARow == SLEEP_DEFAULT_STOPPED_THRESHOLD) state = TRACKER_MOVING_STATE::STOPPED;
			else state = TRACKER_MOVING_STATE::STATIC;
		}
		else stoppedInARow = 0;

		NOTICE_FORMAT("stop %d, state %d, %dkmh => %ds", stoppedInARow, state, velocity, sleepTime);
		return state;
	}

	uint16_t mapSleepTime(uint8_t velocity) {
		#define CURRENT_LOGGER_FUNCTION "mapSleepTime"

		uint16_t result;
		uint16_t currentTime = 0xFFFF;

		if (rtc::isAccurate()) {
			tmElements_t time;
			rtc::getTime(time);

			currentTime = SLEEP_TIMING_TIME(time.hour, time.minute);
		}

		for (uint8_t i = flash::getArraySize(config::defaultSleepTimings); i--;) {
			sleepTimings_t timing;
			flash::read(&config::defaultSleepTimings[i], timing);

			if (velocity < timing.speed) continue;
			if (currentTime != 0xFFFF && (currentTime < timing.timeMin || currentTime > timing.timeMax)) continue;

			result = timing.seconds;
			break;

		}

		VERBOSE_FORMAT("%d,%d", velocity, result);
		return result;
	}
}