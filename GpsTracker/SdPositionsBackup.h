#pragma once

#include "PositionsBackup.h"
#include "RawSdFile.h"

#define POSITIONS_CONFIG_FILENAME "positions.config"

using namespace sd;

namespace positions {
	namespace backup {
		namespace sd {

			class SdPositionsBackup : public PositionsBackup {
			private:
				RawSdFile * _config;
			public:
				void setup();
				void backup();
			};

		}
	}
}