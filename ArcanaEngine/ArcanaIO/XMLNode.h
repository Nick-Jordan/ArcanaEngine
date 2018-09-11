#ifndef XML_NODE_H_
#define XML_NODE_H_

#include "IODefines.h"

#include "Array.h"
#include "XMLAttribute.h"
#include <string>
#include <vector>

namespace Arcana
{

	class ARCANA_IO_API XMLNode
	{
	public:

		XMLNode();

		XMLNode(const XMLNode& node);

		~XMLNode();


		void setName(const std::string& name);

		const std::string& getName() const;

		const std::vector<XMLNode>& getChildren() const; //SWITCH TO ARRAY FIX COPYING

		void addChild(const XMLNode& node);

		const Array<XMLAttribute>& getAttributes() const; //SWITCH TO ARRAY FIX COPYING

		void addAttribute(const XMLAttribute& attribute);

		XMLNode& operator=(const XMLNode& node);

	private:

		std::string _name;

		std::vector<XMLNode> _children;   //SWITCH TO ARRAY FIX COPYING
		Array<XMLAttribute> _attributes; //SWITCH TO ARRAY FIX COPYING
	};

}

#endif // !XML_NODE_H_

