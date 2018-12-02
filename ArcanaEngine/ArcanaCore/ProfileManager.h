#ifndef PROFILE_MANAGER_H_
#define PROFILE_MANAGER_H_

#include "CoreDefines.h"

#include "Types.h"
#include <string>
#include <map>
#include <vector>
#include <mutex>

namespace Arcana
{
	class ARCANA_CORE_API ProfileManager
	{
	public:

		static ProfileManager& instance();

		void exportSamples(const std::string& outputFile);

		void storeSample(const char* name, int64 elapsedTime);

	private:

		ProfileManager();

		~ProfileManager();

	private:

		std::map<const char*, std::vector<int64>> _storedSamples;
		std::mutex _sampleMutex;
	};
}

#endif // !PROFILE_MANAGER_H_