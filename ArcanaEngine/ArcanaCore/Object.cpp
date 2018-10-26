#include "Object.h"

#include "ArcanaLog.h"

namespace Arcana
{

	Object::Object() : _type("Object"), _userData(nullptr)
	{
	}

	Object::Object(const std::string& type) : _type(type), _userData(nullptr)
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
		int32 references = int32(getUserData());
		references++;
		setUserData((void*)references);
		return references;
	}

	void Object::release()
	{
		int32 references = int32(getUserData());
		references--;
		if (!references)
		{
			delete this;
		}
		else
		{
			setUserData((void*)references);
		}
	}
}