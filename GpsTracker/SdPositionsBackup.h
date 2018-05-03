#pragma once

#include "PositionsBackup.h"

namespace positions {
	namespace backup {
		namespace sd {

			class SdPositionsBackup : public PositionsBackup {
			private:
			public:
				void setup();
				void backup(bool force);
			};

		}
	}
}