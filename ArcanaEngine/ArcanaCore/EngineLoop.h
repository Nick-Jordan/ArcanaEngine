#ifndef ENGINE_LOOP_H_
#define ENGINE_LOOP_H_

#include "CoreDefines.h"

#include "Types.h"

namespace Arcana
{
	/** \brief An abstract class defining a looping structure.
	 *  All methods are pure virtual functions; therefore, they must be implemented in derived classes.
	 *  This interface provides only functions for initialization, updating, and clearing objects that are pending cleanup.
	 */

	class EngineLoop
	{
	public:
		
		/** \brief Initialize the engine loop.
		 */

		virtual int32 initialize() = 0;
		
		/** \brief Updates the engine loop.
		 */

		virtual void update() = 0;
		
		/** \brief Clears the cache of engine objects pending cleanup.
		 */

		virtual void clearPendingCleanupObjects() = 0;
	};
}

#endif