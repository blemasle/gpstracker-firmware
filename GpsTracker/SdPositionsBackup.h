#pragma once

#include "PositionsBackup.h"

namespace positions {
	namespace backup {

		class SdPositionsbackup : public PositionsBackup {
		public:
			void backup();
		};
	}
}