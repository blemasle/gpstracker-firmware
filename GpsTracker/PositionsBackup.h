#pragma once

namespace positions {
	namespace backup {

		class PositionsBackup {
		public:
			~PositionsBackup();
			virtual void setup()=0;
			virtual void backup()=0;
		};

	}
}