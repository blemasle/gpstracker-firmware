#include "RawSdFile.h"

namespace sd {

	RawSdFile::RawSdFile(SdFat *sd, const char *filename) {
		_sd = sd;
		_filename = filename;
	}

	RawSdFile::~RawSdFile() {
		flush();
	}

	void RawSdFile::open(const uint32_t pos) {
		if (!_file.isOpen()) {
			_sd->chdir();
			_file = _sd->open(_filename, O_RDWR | O_CREAT);
		}

		_file.seek(pos);
	}

	void RawSdFile::flush() {
		if (!_file.isOpen()) return;

		_file.close();
	}

	void RawSdFile::write(const uint32_t pos, const void *val, const size_t size) {
		open(pos);
		_file.write(val, size);
	}

	void RawSdFile::read(const uint32_t pos, void *val, const size_t size) {
		open(pos);
		if (!_file.available()) return;

		_file.read(val, size);
	}
}