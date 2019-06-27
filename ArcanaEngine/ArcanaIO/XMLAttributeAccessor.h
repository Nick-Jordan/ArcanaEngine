#ifndef XML_ATTRIBUTE_ACCESSOR_H_
#define XML_ATTRIBUTE_ACCESSOR_H_

#include "IODefines.h"

#include "XMLAttribute.h"
#include "Types.h"

#include <vector>

namespace Arcana
{

	class ARCANA_IO_API XMLAttributeAccessor
	{
	public:

		XMLAttributeAccessor() {};

		XMLAttributeAccessor(const XMLAttributeAccessor& copy);

		XMLAttributeAccessor& operator=(const XMLAttributeAccessor& copy);

		uint8 getUint8Attribute(const std::string& name) const;
		uint16 getUint16Attribute(const std::string& name) const;
		uint32 getUint32Attribute(const std::string& name) const;
		uint64 getUint64Attribute(const std::string& name) const;

		int8 getInt8Attribute(const std::string& name) const;
		int16 getInt16Attribute(const std::string& name) const;
		int32 getInt32Attribute(const std::string& name) const;
		int64 getInt64Attribute(const std::string& name) const;

		float getFloatAttribute(const std::string& name) const;
		double getDoubleAttribute(const std::string& name) const;
		bool getBoolAttribute(const std::string& name) const;

	private:

		const XMLAttribute* getAttribute(const std::string& name) const;
		
	protected:

		std::vector<XMLAttribute> _attributes;
	};

}

#endif // !XML_ATTRIBUTE_ACCESSOR_H_
