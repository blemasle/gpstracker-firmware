#pragma once

#include "PositionsBackup.h"
#include "RawSdFile.h"

#define POSITIONS_FOLDER	"positions"
#define POSITIONS_FILENAME	"positions-%05d.csv"
#define POSITIONS_FILENAME_LENGTH 19


using namespace sd;

namespace positions {
	namespace backup {
		namespace sd {

			class SdPositionsBackup : public PositionsBackup {
			private:
			public:
				void setup();
				void backup();
			};

		}
	}
}