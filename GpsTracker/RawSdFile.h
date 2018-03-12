#pragma once

#include <SdFat.h>

namespace sd {
	class RawSdFile {
	private:
		SdFat * _sd;
		const char *_filename;
		File _file;

		void open(const uint32_t pos);

	public:
		RawSdFile(SdFat *sd, const char *filename);
		~RawSdFile();

		void flush();

		void write(const uint32_t pos, const void *val, const size_t size);
		template <typename T> void write(const uint32_t pos, const T &val)
		{
			write(pos, (void*)&val, sizeof(val));
		}

		void read(const uint32_t pos, void *val, const size_t size);
		template <typename T> void read(const uint32_t pos, const T &val)
		{
			read(pos, (void*)&val, sizeof(val));
		}
	};
}
