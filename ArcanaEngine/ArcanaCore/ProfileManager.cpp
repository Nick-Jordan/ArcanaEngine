#include "ProfileManager.h"

#include "ArcanaLog.h"
#include <fstream>

namespace Arcana
{
	ProfileManager& ProfileManager::instance()
	{
		static ProfileManager manager;
		return manager;
	}

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

			stream << "Frame,";

			for (int32 i = 0; i < (*_storedSamples.begin()).second.size(); i++)
			{
				stream << i << ",";
			}

			stream << std::endl;

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

		std::map<const char*, std::vector<int64>>::iterator i;
		for (i = _storedSamples.begin(); i != _storedSamples.end(); i++)
		{
			double avg = 0.0;
			for (int j = 0; j < (*i).second.size(); j++)
			{
				avg += (*i).second[j];
			}
			avg /= (double)(*i).second.size();
			LOGF(Info, CoreEngine, "Profile: %s, %f", (*i).first, avg / 1000000.0);
		}
	}

	void ProfileManager::storeSample(const char* name, int64 elapsedTime)
	{
		_sampleMutex.lock();
		_storedSamples[name].push_back(elapsedTime);
		_sampleMutex.unlock();
	}
}