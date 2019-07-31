#include "ObjectDestructionManager.h"

namespace Arcana
{
	ObjectDestructionManager& ObjectDestructionManager::instance()
	{
		static ObjectDestructionManager managerInstance;
		return managerInstance;
	}

	ObjectDestructionManager::ObjectDestructionManager()
	{

	}

	ObjectDestructionManager::~ObjectDestructionManager()
	{
	}

	void ObjectDestructionManager::addPendingCleanupObject(Object* object)
	{
		if (object)
		{
			DestructionMutex.lock();
			_pendingCleanupObjects.push_back(object);
			DestructionMutex.unlock();
		}
	}

	void ObjectDestructionManager::cleanupObjects()
	{
		//LOGF(Warning, CoreEngine, "Cleanup objects: %d", _pendingCleanupObjects.size());
		
		DestructionMutex.lock();

		std::vector<Object*>::iterator it;
		for (it = _pendingCleanupObjects.begin(); it != _pendingCleanupObjects.end();)
		{
			Object* object = *it;
			if (object && object->canBeDestroyed())
			{
				it = _pendingCleanupObjects.erase(it);
				AE_DELETE(object);
			}
			else
			{
				it++;
			}
		}

		DestructionMutex.unlock();
	}
}