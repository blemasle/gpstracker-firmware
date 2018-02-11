#pragma once

#include <Arduino.h>

namespace flash {

	template<typename T, size_t N> size_t getFlashArraySize(T(&)[N]) { return N; }
	template<typename T> void readFromFlash(const T *source, T &dest) {
		memcpy_P(&dest, source, sizeof(T));
	}
}