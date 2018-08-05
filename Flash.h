#pragma once

#include <Arduino.h>

namespace utils {
	namespace flash {

		template<typename T, size_t N> size_t getArraySize(T(&)[N]) { return N; }
		template<typename T> void read(const T *source, T &dest) {
			memcpy_P(&dest, source, sizeof(T));
		}
	}
}