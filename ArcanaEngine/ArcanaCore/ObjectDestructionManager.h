#ifndef OBJECT_DESTRUCTION_MANAGER_H_
#define OBJECT_DESTRCUTION_MANAGER_H_

#include "CoreDefines.h"

#include "Array.h"
#include "Object.h"

namespace Arcana
{
	class ARCANA_CORE_API ObjectDestructionManager
	{
	public:

		static ObjectDestructionManager& instance();

		ObjectDestructionManager();

		~ObjectDestructionManager();


		void addPendingCleanupObject(Object* object);

		void cleanupObjects();

	private:

		std::vector<Object*> _pendingCleanupObjects;
	};
}

#endif // !OBJECT_DESTRCUTION_MANAGER_H_