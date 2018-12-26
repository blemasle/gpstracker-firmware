#pragma once

/**
 * Configure the values used for alerts triggering.
 * Note that the battery level percentage are quite high,
 * because the reading provided by the SIM808 module are
 * not accurate and the device shuts itself off way before reaching 0%.
 * The clearance level is also well above the first alert level, because
 * the reading is so inaccurate that sometimes the battery can gain 5%
 * between readings. Setting a clearance level much higher avoid
 * clearing the levels and retriggering them the next time.
 */

#define ALERTS_ON_SERIAL _DEBUG								///< Display alerts on serial when connected rather than sending an SMS.
#define CONFIG_BATTERY_ALERT_LEVEL1			45				///< Battery percentage at which to trigger the first low battery alert.
#define CONFIG_BATTERY_ALERT_LEVEL2			38				///< Battery percentage at which to trigger the final low battery alert.
#define CONFIG_BATTERY_ALERT_CLEAR			60				///< Battery percentage at which to clear all battery alerts.
#define CONFIG_ACTIVE_ALERTS				0				///< Default active alerts
#define CONFIG_CONTACT_PHONE				"+642568452" 	///< Default phone number to send the alert SMS to.


#define ALERT_SUSPICIOUS_RTC_TEMPERATURE	0	///< Temperature at which to consider the RTC module as failling.
												///< When the backup battery is dead or nearly dead, the reading
												///< of the temperature fails and returns 0.