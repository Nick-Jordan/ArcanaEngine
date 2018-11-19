#ifndef PROFILE_MANAGER_H_
#define PROFILE_MANAGER_H_

#include "CoreDefines.h"

#include "Types.h"
#include <string>
#include <map>
#include <vector>

namespace Arcana
{
	class ARCANA_CORE_API ProfileManager
	{
	public:

		ProfileManager();

		~ProfileManager();

		void exportSamples(const std::string& outputFile);

		void storeSample(const char* name, int64 elapsedTime);

	private:

		std::map<const char*, std::vector<int64>> _storedSamples;
	};
}

#endif // !PROFILE_MANAGER_H_