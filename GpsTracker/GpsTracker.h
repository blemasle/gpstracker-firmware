#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <SIM808.h>

#include "Debug.h"
#include "Gps.h"
#include "Network.h"
#include "Storage.h"

#define LOGGER_NAME "GpsTracker"

#define SIM_RX		5
#define SIM_TX		6
#define SIM_PWR		9
#define SIM_STATUS	8

#define IC_PWR		A0
#define SD_SS		SS

#define SIM_RI		2
#define RTC_WAKE	3

