#include "Object.h"

#include "ArcanaLog.h"

namespace Arcana
{

	Object::Object() : _type("Object"), _userData(nullptr), _referenceCount((void*)0)
	{

	}

	Object::Object(const std::string& type) : _type(type), _userData(nullptr), _referenceCount((void*)0)
	{

	}

	Object::Object(const Object& object) : _type(object._type), _userData(object._userData), _referenceCount(object._referenceCount)
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
		LOGF(Error, CoreEngine, "References: %s, %d",_type.c_str(), references);
		if (!references)
		{
			delete this;
		}
		else
		{
			_referenceCount = (void*)references;
		}
	}
}