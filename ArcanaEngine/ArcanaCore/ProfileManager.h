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
	/** \brief Manager for profile samples.
	 *
	 *  Stores profile samples and exports a csv.
	 */
	class ARCANA_CORE_API ProfileManager
	{
	public:

		/** \brief Singleton instance accessor.
		 */
		static ProfileManager& instance();

		/** \brief Exports the samples to a csv file.
		 */
		void exportSamples(const std::string& outputFile);

		/** \brief Stores a profile sample.
		 */
		void storeSample(const char* name, int64 elapsedTime);

	private:

		/** \brief ProfileManager default constructor.
		 */
		ProfileManager();

		/** \brief ProfileManager destructor.
		 */
		~ProfileManager();

	private:

		std::map<const char*, std::vector<int64>> _storedSamples;  ///< Stored profile samples.
		std::mutex _sampleMutex;  ///< Mutex to protect the storedSamples vector.
	};
}

#endif // !PROFILE_MANAGER_H_