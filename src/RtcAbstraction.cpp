#include "RtcAbstraction.h"

#ifndef USE_UDS3231

void MD_DS3231_Ext::unpack(tmElements_t &time) {
	time.second = MD_DS3231::s;
	time.minute = MD_DS3231::m;
	time.hour = MD_DS3231::h;
	time.day = MD_DS3231::dd;
	time.month = MD_DS3231::mm;
	time.year = MD_DS3231::yyyy;
}

void MD_DS3231_Ext::pack(const tmElements_t &time) {
	MD_DS3231::s = time.second;
	MD_DS3231::m = time.minute;
	MD_DS3231::h = time.hour;
	MD_DS3231::dd = time.day;
	MD_DS3231::mm = time.month;
	MD_DS3231::yyyy = time.year;
}

boolean MD_DS3231_Ext::readTime(tmElements_t &time) {
	bool result = MD_DS3231::readTime();
	unpack(time);

	return result;
}

boolean MD_DS3231_Ext::writeTime(const tmElements_t &time) {
	pack(time);
	return MD_DS3231::writeTime();
}

boolean MD_DS3231_Ext::readAlarm1(almType_t &almType, tmElements_t &time) {
	almType = MD_DS3231::getAlarm1Type();
	bool result = MD_DS3231::readAlarm1();

	return result;
}

boolean MD_DS3231_Ext::writeAlarm1(almType_t almType, const tmElements_t &time) {
	pack(time);
	return MD_DS3231::writeAlarm1(almType);
}

#endif