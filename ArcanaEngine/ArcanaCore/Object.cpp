#include "Object.h"

#include "ArcanaLog.h"
//#include "ObjectDestructionManager.h"

namespace Arcana
{

	Object::Object() : _type("Object"), _userData(nullptr), _referenceCount((void*)0), 
		_markedForDestruction(false), _canBeDestroyed(false)
	{

	}

	Object::Object(const std::string& type) : _type(type), _userData(nullptr), _referenceCount((void*)0), 
		_markedForDestruction(false), _canBeDestroyed(false)
	{

	}

	Object::Object(const Object& object) : _type(object._type), _userData(object._userData), _referenceCount(object._referenceCount), 
		_markedForDestruction(false), _canBeDestroyed(false)
	{

	}

	Object::~Object()
	{
	}

	const std::string& Object::getType() const
	{
		return _type;
	}

	void* Object::getUserData() const
	{
		return _userData;
	}

	void Object::setUserData(void* data)
	{
		_userData = data;
	}

	int32 Object::reference()
	{
		int32 references = int32(_referenceCount);
		references++;
		_referenceCount = (void*)references;
		return references;
	}

	void Object::release()
	{
		int32 references = int32(_referenceCount);
		references--;

		if (references < 1)
		{
			delete this;
		}
		else
		{
			_referenceCount = (void*)references;
		}
	}

	int32 Object::referenceCount()
	{
		return int32(_referenceCount);
	}

	void Object::markForDestruction()
	{
		_markedForDestruction = true;
		//ObjectDestructionManager::instance().addPendingCleanupObject(this);
	}

	bool Object::isPendingDestroy() const
	{
		return _markedForDestruction;
	}

	void Object::allowDestruction()
	{
		LOG(Warning, CoreEngine, "allow destruction called");
		_canBeDestroyed = true;
	}

	bool Object::canBeDestroyed() const
	{
		return _canBeDestroyed;
	}
}