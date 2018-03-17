#include "Log.h"

namespace log {

	namespace details {

		void waitForSerial() {
			while (!Serial);
			Serial.begin(DEBUG_SERIAL_SPEED);
		}

	}
	void setup() {
#if _DEBUG
		details::waitForSerial();
		Log.begin(LOG_LEVEL_VERBOSE, &Serial);
#else
		if (Serial) {
			Serial.begin(DEBUG_SERIAL_SPEED);
			Log.begin(LOG_LEVEL_NOTICE, &Serial);
		}
#endif

		if (Serial) Serial.println(F("============================="));
	}

}