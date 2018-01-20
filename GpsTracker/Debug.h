#pragma once

#ifdef _DEBUG

#include <ArduinoLog.h>

#define VERBOSE(f) Log.verbose(F("[" LOGGER_NAME "::" f "]\n"))
#define VERBOSE_MSG(f, msg) Log.verbose(F("[" LOGGER_NAME "::" f "] " msg "\n"))
#define VERBOSE_FORMAT(f, msg, ...) Log.verbose(F["[" LOGGER_NAME "::" f "] " msg "\n"), __VA_ARGS__)

#else

#define DISABLE_LOGGING 1

#define VERBOSE(f)
#define VERBOSE(f, msg)
#define VERBOSE(f, msg, ...)

#endif

void waitForSerial();
