#include "BaseObject.h"

namespace Arcana
{

	BaseObject::BaseObject() : Object("BaseObject"), _active(true), _name("")
	{

	}

	BaseObject::BaseObject(const BaseObject& object) : Object(object), _active(object._active), _name(object._name)
	{
	}

	BaseObject::~BaseObject()
	{

	}

	bool BaseObject::isActive() const
	{
		return _active;
	}

	void BaseObject::setActive(bool active)
	{
		_active = active;
	}

	const std::string& BaseObject::getName() const
	{
		return _name;
	}

	void BaseObject::setName(const std::string& name)
	{
		_name = name;
	}

	BaseObject& BaseObject::operator=(const BaseObject& object)
	{
		Object::operator=(object);

		_active = object._active;
		_name = object._name;

		return *this;
	}
}
