#pragma once

#include <Arduino.h>
#include <Time3.h>



/*============================================================================*/

//typedef unsigned long timestamp_t;

//typedef struct {
//	uint8_t Second;
//	uint8_t Minute;
//	uint8_t Hour;
//	//uint8_t Wday;   // day of week, sunday is day 1
//	uint8_t Day;
//	uint8_t Month;
//	uint8_t Year;   // offset from 1970;
//} 	tmElements_t;

/* low level functions to convert to and from system time                     */
void breakTime(const timestamp_t time, tmElements_t &tm);  // break timestamp_t into elements
timestamp_t makeTimestamp(const tmElements_t &tm);  // convert time elements into timestamp_t
