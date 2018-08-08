/**
 * Defines Arduino pins number connected to external systems.
 */
#pragma once

#include <E24.h>
#include <SoftwareSerial.h>

#define SIM_RST		5	///< SIM808 RESET
#define SIM_RX		6	///< SIM808 RXD
#define SIM_TX		7	///< SIM808 TXD
#define SIM_PWR		9	///< SIM808 PWRKEY
#define SIM_STATUS	8	///< SIM808 STATUS

#define I2C_PWR		A0	///< Used to drive I2C power supply
#define SD_SS		SS

#define SIM_RI		2	///< Interrupt pin connected to SIM RI.
#define RTC_WAKE	3	///< Interrupt pin connected to DS3231 !INT/SQW


#define E24_ADDRESS 	E24_DEFAULT_ADDR				///< I2C address of the 24xxx chip
#define SIM_SERIAL_TYPE	SoftwareSerial					///< Type of variable that holds the Serial communication with SIM808
#define SIM_SERIAL		SIM_SERIAL_TYPE(SIM_TX, SIM_RX)	///< Definition of the instance that holds the Serial communication with SIM808