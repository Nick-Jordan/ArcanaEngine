#include "XMLAttribute.h"

namespace Arcana
{
	XMLAttribute::XMLAttribute() : _name("attribute"), _value("")
	{

	}

	XMLAttribute::XMLAttribute(const XMLAttribute& attribute) : _name(attribute._name), _value(attribute._value)
	{

	}

	XMLAttribute::~XMLAttribute()
	{

	}


	const std::string& XMLAttribute::getName() const
	{
		return _name;
	}

	void XMLAttribute::setName(const std::string& name)
	{
		_name = name;
	}

	const std::string& XMLAttribute::getValue() const
	{
		return _value;
	}

	void XMLAttribute::setValue(const std::string& value)
	{
		_value = value;
	}


	XMLAttribute& XMLAttribute::operator=(const XMLAttribute& attribute)
	{
		_name = attribute._name;
		_value = attribute._value;

		return *this;
	}
}