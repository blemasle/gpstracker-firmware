#pragma once

namespace positions {
	namespace backup {

		class PositionsBackup {
		public:
			virtual void setup();
			virtual void backup();
		};

	}
}