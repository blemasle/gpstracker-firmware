#pragma once

#include "PositionsBackup.h"

namespace positions {
	namespace backup {
		namespace net {

			class NetworkPositionsBackup : public PositionsBackup {
			private:
			public:
				void setup();
				void backup();
			};

		}
	}
}