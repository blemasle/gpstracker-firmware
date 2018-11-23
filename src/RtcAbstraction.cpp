#include "RtcAbstraction.h"

#ifndef USE_UDS3231
#ifdef STOCK_MD_DS3231

void MDS3231_Ext::unpack(tmElements_t &time) {
	time.second = s;
	time.minute = m;
	time.hour = h;
	time.day = dd;
	time.month = mm;
	time.year = yyyy;
}

void MDS3231_Ext::pack(const tmElements_t &time) {
	s = time.second;
	m = time.minute;
	h = time.hour;
	dd = time.day;
	mm = time.month;
	yyyy = time.year;
}

boolean MDS3231_Ext::readTime(tmElements_t &time) {
	bool result = MD_DS3231::readTime();
	unpack(time);

	return result;
}

boolean MDS3231_Ext::writeTime(const tmElements_t &time) {
	pack(time);
	return MD_DS3231::writeTime();
}

boolean MDS3231_Ext::readAlarm1(alm1Type_t &almType, tmElements_t &time) {
	almType = MD_DS3231::getAlarm1Type();
	bool result = MD_DS3231::readAlarm1();

	return result;
}

boolean MDS3231_Ext::writeAlarm1(alm1Type_t almType, const tmElements_t &time) {
	pack(time);
	return MD_DS3231::writeAlarm1(almType);
}

#endif
#endif