#pragma once

#include "PositionsBackup.h"
#include "Positions.h"

namespace positions {
	namespace backup {
		namespace net {

			class NetworkPositionsBackup : public PositionsBackup {
			private:
				bool isBackupNeeded(bool forPrepare);
				bool appendPosition(PositionEntry &entry, int8_t signalAttenuation = 0);
				void appendPositions();

			public:
				void setup();

				void prepare(bool force);
				void backup(bool force);
			};

		}
	}
}