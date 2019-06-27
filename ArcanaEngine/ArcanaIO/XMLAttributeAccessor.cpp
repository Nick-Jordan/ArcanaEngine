#include "XMLAttributeAccessor.h"

#include "StringUtils.h"

namespace Arcana
{
	XMLAttributeAccessor::XMLAttributeAccessor(const XMLAttributeAccessor& copy)
		: _attributes(copy._attributes)
	{

	}

	XMLAttributeAccessor& XMLAttributeAccessor::operator=(const XMLAttributeAccessor& copy)
	{
		_attributes = copy._attributes;

		return *this;
	}

	uint8 XMLAttributeAccessor::getUint8Attribute(const std::string& name) const
	{
		const XMLAttribute* attr = getAttribute(name);

		if (attr)
		{
			return StringUtils::convertStringToUint8(attr->getValue());
		}

		return 0;
	}

	uint16 XMLAttributeAccessor::getUint16Attribute(const std::string& name) const
	{
		const XMLAttribute* attr = getAttribute(name);

		if (attr)
		{
			return StringUtils::convertStringToUint16(attr->getValue());
		}

		return 0;
	}
	uint32 XMLAttributeAccessor::getUint32Attribute(const std::string& name) const
	{
		const XMLAttribute* attr = getAttribute(name);

		if (attr)
		{
			return StringUtils::convertStringToUint32(attr->getValue());
		}

		return 0;
	}
	uint64 XMLAttributeAccessor::getUint64Attribute(const std::string& name) const
	{
		const XMLAttribute* attr = getAttribute(name);

		if (attr)
		{
			return StringUtils::convertStringToUint64(attr->getValue());
		}

		return 0;
	}

	int8 XMLAttributeAccessor::getInt8Attribute(const std::string& name) const
	{
		const XMLAttribute* attr = getAttribute(name);

		if (attr)
		{
			return StringUtils::convertStringToInt8(attr->getValue());
		}

		return 0;
	}
	int16 XMLAttributeAccessor::getInt16Attribute(const std::string& name) const
	{
		const XMLAttribute* attr = getAttribute(name);

		if (attr)
		{
			return StringUtils::convertStringToInt16(attr->getValue());
		}

		return 0;
	}
	int32 XMLAttributeAccessor::getInt32Attribute(const std::string& name) const
	{
		const XMLAttribute* attr = getAttribute(name);

		if (attr)
		{
			return StringUtils::convertStringToInt32(attr->getValue());
		}

		return 0;
	}
	int64 XMLAttributeAccessor::getInt64Attribute(const std::string& name) const
	{
		const XMLAttribute* attr = getAttribute(name);

		if (attr)
		{
			return StringUtils::convertStringToInt64(attr->getValue());
		}

		return 0;
	}

	float XMLAttributeAccessor::getFloatAttribute(const std::string& name) const
	{
		const XMLAttribute* attr = getAttribute(name);

		if (attr)
		{
			return StringUtils::convertStringToFloat(attr->getValue());
		}

		return 0;
	}
	double XMLAttributeAccessor::getDoubleAttribute(const std::string& name) const
	{
		const XMLAttribute* attr = getAttribute(name);

		if (attr)
		{
			return StringUtils::convertStringToDouble(attr->getValue());
		}

		return 0;
	}
	bool XMLAttributeAccessor::getBoolAttribute(const std::string& name) const
	{
		const XMLAttribute* attr = getAttribute(name);

		if (attr)
		{
			return StringUtils::convertStringToBool(attr->getValue());
		}

		return false;
	}

	const XMLAttribute* XMLAttributeAccessor::getAttribute(const std::string& name) const
	{
		std::vector<XMLAttribute>::const_iterator iter;
		for (iter = _attributes.begin(); iter != _attributes.end(); iter++)
		{
			const XMLAttribute& attr = *iter;

			if (attr.getName() == name)
			{
				return &attr;
			}
		}

		return nullptr;
	}
}