#pragma once

#include "PositionsBackup.h"
#include "Time2.h"
#include "Positions.h"

namespace positions {
	namespace backup {
		namespace net {

			class NetworkPositionsBackup : public PositionsBackup {
			private:
				timestamp_t _prepareTime;

				bool isBackupNeeded(bool forPrepare);
				bool appendPosition(PositionEntry &entry);
				void appendPositions();

			public:
				void setup();

				void prepare();
				void backup(bool force);
			};

		}
	}
}