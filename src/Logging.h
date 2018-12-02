#pragma once

//#define DISABLE_LOGGING 1
#include <ArduinoLog.h>
#include <SIMComAT.Common.h> //reusing flash string utilities
#include "Config.h"

#define LOG_SERIAL_SPEED 115200
#if _DEBUG
	#define LOG_LEVEL LOG_LEVEL_VERBOSE
	#define LOG_LEVEL_VERBOSE_ENABLED
	#define LOG_LEVEL_NOTICE_ENABLED
#else
	#define LOG_LEVEL LOG_LEVEL_NOTICE
	#define LOG_LEVEL_NOTICE_ENABLED
#endif

#define NL 						"\n"
#define LOG(level) 					LOG_MSG(level, "")
#define LOG_MSG(level, msg) 		Log.level(F("[" CURRENT_LOGGER "::" CURRENT_LOGGER_FUNCTION "]" msg NL))
#define LOG_FORMAT(level, msg, ...) Log.level(F("[" CURRENT_LOGGER "::" CURRENT_LOGGER_FUNCTION "]" msg NL), __VA_ARGS__)

#if defined(LOG_LEVEL_VERBOSE_ENABLED)
	#define VERBOSE LOG(verbose)
	#define VERBOSE_MSG(msg) LOG_MSG(verbose, msg)
	#define VERBOSE_FORMAT(msg, ...) LOG_FORMAT(verbose, msg, __VA_ARGS__)
#else
	#define VERBOSE
	#define VERBOSE_MSG(msg)
	#define VERBOSE_FORMAT(msg, ...)
#endif

#if defined(LOG_LEVEL_NOTICE_ENABLED)
	#define NOTICE LOG(notice)
	#define NOTICE_MSG(msg) LOG_MSG(notice, msg)
	#define NOTICE_FORMAT(msg, ...) LOG_FORMAT(notice, msg, __VA_ARGS__)
#else
	#define NOTICE
	#define NOTICE_MSG(msg)
	#define NOTICE_FORMAT(msg, ...)
#endif

namespace logging {
	void setup();
}