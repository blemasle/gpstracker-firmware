#pragma once

/**
 * \def USE_UDS3231
 * When defined, use the modified uDS3231 library instead of
 * the stock MD_DS3231 one.
 */
// #define USE_UDS3231
//#define STOCK_MD_DS3231

#ifdef USE_UDS3231
	#include <uDS3231.h>

	#define RTC_A_CLASS uDS3231
	#define WRITE_ALARM_1(t) RTC_A.writeAlarm1(DS3231_ALM_HMS, t)
#else
	#ifdef STOCK_MD_DS3231
		#include <MD_DS3231.h>

		typedef unsigned long timestamp_t;

		typedef struct tmElements_t {
			uint8_t Second;
			uint8_t Minute;
			uint8_t Hour;
			uint8_t Day;
			uint8_t month;
			uint8_t year;   // year from 2000
		};

		class MD_DS3231_Ext : public MD_DS3231
		{
		private:
			void unpack(tmElements_t &time);
			void pack(const tmElements_t &time);
		public:
			boolean readTime(tmElements_t &time);
			boolean writeTime(const tmElements_t &time);

			boolean readAlarm1(almType_t &almType, tmElements_t &time);
			boolean writeAlarm1(almType_t almType, const tmElements_t &time);
		};

		#define RTC_A_CLASS MD_DS3231_Ext
		#define WRITE_ALARM_1(t) RTC_A.writeAlarm1(DS3231_ALM_HMS, t)
	#else
		#include <MD_DS3231.h>
		#define RTC_A_CLASS MD_DS3231
		#define WRITE_ALARM_1(t) RTC_A.writeAlarm1(alm1Type_t::ALM_HMS, t)

		typedef unsigned long timestamp_t;
	#endif
#endif