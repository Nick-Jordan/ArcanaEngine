#include "ProfileManager.h"

#include <fstream>

namespace Arcana
{
	ProfileManager::ProfileManager()
	{

	}

	ProfileManager::~ProfileManager()
	{
		_storedSamples.clear();
	}

	void ProfileManager::exportSamples(const std::string& outputFile)
	{
		if (_storedSamples.size() > 0)
		{
			std::fstream stream;
			stream.open(outputFile.c_str(), std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);

			std::map<const char*, std::vector<int64>>::iterator i;
			for (i = _storedSamples.begin(); i != _storedSamples.end(); i++)
			{
				stream << (*i).first << ",";
				std::vector<int64>::iterator j;
				for (j = (*i).second.begin(); j != (*i).second.end(); j++)
				{
					stream << *j << ",";
				}
				stream << std::endl;
			}
			stream.flush();
		}
	}

	void ProfileManager::storeSample(const char* name, int64 elapsedTime)
	{
		_storedSamples[name].push_back(elapsedTime);
	}
}