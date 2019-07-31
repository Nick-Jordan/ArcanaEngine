#ifndef OBJECT_DESTRUCTION_MANAGER_H_
#define OBJECT_DESTRCUTION_MANAGER_H_

#include "CoreDefines.h"

#include "Array.h"
#include "Object.h"
#include <mutex>

namespace Arcana
{
	/** \brief Manages automatic garbage collection.
	 */
	class ARCANA_CORE_API ObjectDestructionManager
	{
	public:

		/** \brief Singleton instance accessor.
		 */
		static ObjectDestructionManager& instance();

		/** \brief ObjectDestructionManager default constructor.
		 */
		ObjectDestructionManager();

		/** \brief ObjectDestructionManager destructor.
		 */
		~ObjectDestructionManager();

		/** \brief Adds an Object to the pending cleanup queue.
		 */
		void addPendingCleanupObject(Object* object);

		/** \brief Cleans up any objects that can be destroyed.
		 * 
		 *  Called every update cycle by the main engine loop.
		 */
		void cleanupObjects();

		std::mutex DestructionMutex;

	private:

		std::vector<Object*> _pendingCleanupObjects; ///< Vector of objects pending destruction.
	};
}

#endif // !OBJECT_DESTRCUTION_MANAGER_H_