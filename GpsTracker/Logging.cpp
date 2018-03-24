#include "Logging.h"

namespace logging {

	void setup() {
#if _DEBUG
		while (!Serial);
#endif

		if (Serial) {
			Serial.begin(LOG_SERIAL_SPEED);
			Log.begin(LOG_LEVEL, &Serial);

			Log.notice(F("Starting...\n"));
		}
	}

}