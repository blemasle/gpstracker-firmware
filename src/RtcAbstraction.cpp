#include "RtcAbstraction.h"

void uDS3231_Ext::unpack(tmElements_t &time) {
	time.Second = s;
	time.Minute = m;
	time.Hour = h;
	time.Day = dd;
	time.Month = mm;
	time.Year = yyyy;
}

void uDS3231_Ext::pack(const tmElements_t &time) {
	s = time.Second;
	m = time.Minute;
	h = time.Hour;
	dd = time.Day;
	mm = time.Month;
	yyyy = time.Year;
}

boolean uDS3231_Ext::readTime(tmElements_t &time) {
	bool result = MD_DS3231::readTime();
	unpack(time);

	return result;
}

boolean uDS3231_Ext::writeTime(const tmElements_t &time) {
	pack(time);
	return MD_DS3231::writeTime();
}

boolean uDS3231_Ext::readAlarm1(almType_t &almType, tmElements_t &time) {
	almType = MD_DS3231::getAlarm1Type();
	bool result = MD_DS3231::readAlarm1();

	return result;
}

boolean uDS3231_Ext::writeAlarm1(almType_t almType, const tmElements_t &time) {
	pack(time);
	return MD_DS3231::writeAlarm1(almType);
}