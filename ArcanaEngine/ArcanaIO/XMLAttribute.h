#ifndef XML_ATTRIBUTE_H_
#define XML_ATTRIBUTE_H_

#include "IODefines.h"

#include <string>

namespace Arcana
{

	class ARCANA_IO_API XMLAttribute
	{
	public:


		XMLAttribute();

		XMLAttribute(const XMLAttribute& attribute);

		~XMLAttribute();


		const std::string& getName() const;

		void setName(const std::string& name);

		const std::string& getValue() const;

		void setValue(const std::string& value);


		XMLAttribute& operator=(const XMLAttribute& attribute);

	private:

		std::string _name;
		std::string _value;
	};

}

#endif // !XML_ATTRIBUTE_H_
