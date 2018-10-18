#include "Object.h"

#include "ArcanaLog.h"

namespace Arcana
{

	Object::Object() : _type("Object")
	{
	}

	Object::Object(const std::string& type) : _type(type)
	{

	}

	Object::~Object()
	{
	}

	const std::string& Object::getType() const
	{
		return _type;
	}
}