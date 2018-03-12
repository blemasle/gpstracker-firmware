#pragma once

#include "PositionsBackup.h"
#include "RawSdFile.h"

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