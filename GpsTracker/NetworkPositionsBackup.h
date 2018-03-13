#pragma once

#include "PositionsBackup.h"

namespace positions {
	namespace backup {
		namespace network {

			class NetworkPositionsBackup : public PositionsBackup {
			private:
			public:
				void setup();
				void backup();
			};

		}
	}
}