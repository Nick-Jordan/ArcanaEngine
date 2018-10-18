#include "BaseObject.h"

namespace Arcana
{

	BaseObject::BaseObject() : _active(true), _userData(nullptr), _name("")
	{

	}

	BaseObject::BaseObject(const BaseObject& object) : _active(object._active), _userData(object._userData), _name(object._name)
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

	void* BaseObject::getUserData() const
	{
		return _userData;
	}

	void BaseObject::setUserData(void* data)
	{
		_userData = data;
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
		_active = object._active;
		_userData = object._userData;
		_name = object._name;

		return *this;
	}
}
