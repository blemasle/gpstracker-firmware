#pragma once

#include "PositionsBackup.h"

namespace positions {
	namespace backup {
		namespace sd {

			class SdPositionsBackup : public PositionsBackup {
			private:
			public:
				void setup(bool force);
				void backup(bool force);
			};

		}
	}
}