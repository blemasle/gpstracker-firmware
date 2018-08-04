#include "Buffer.h"
#include "string.h"


namespace buffer {
	char value[BUFFER_SIZE];

	void clear() { memset(buffer::value, NULL, BUFFER_SIZE); }
}