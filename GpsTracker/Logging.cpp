#include "Logging.h"

namespace logging {

	void setup() {
#if _DEBUG
		while (!Serial);
#endif

		if (Serial) {
			Serial.begin(LOG_SERIAL_SPEED);
			Serial.println(F("============================="));

			Log.begin(LOG_LEVEL, &Serial);
		}
	}

}