#include "Debug.h"

void waitForSerial() {
#ifdef _DEBUG
	while (!Serial);
	Serial.begin(115200);
#endif // _DEBUG
}