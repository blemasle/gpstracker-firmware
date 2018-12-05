#pragma once

namespace positions {
	namespace backup {

		class PositionsBackup {
		public:
			~PositionsBackup();
			virtual void setup() = 0;

			virtual void prepare(bool force) = 0;
			virtual void backup(bool force) = 0;
		};

	}
}