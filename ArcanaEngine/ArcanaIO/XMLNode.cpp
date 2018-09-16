#include "XMLNode.h"

namespace Arcana
{
	XMLNode::XMLNode() : _name("node"), _value("value")
	{

	}

	XMLNode::XMLNode(const XMLNode& node) : _name(node._name), _value(node._value), _children(node._children), _attributes(node._attributes)
	{
	}

	XMLNode::~XMLNode()
	{

	}


	void XMLNode::setName(const std::string& name)
	{
		_name = name;
	}

	const std::string& XMLNode::getName() const
	{
		return _name;
	}

	void XMLNode::setValue(const std::string& value)
	{
		_value = value;
	}

	const std::string& XMLNode::getValue() const
	{
		return _value;
	}

	const  std::vector<XMLNode>& XMLNode::getChildren() const
	{
		return _children;
	}

	void XMLNode::addChild(const XMLNode& node)
	{
		_children.push_back(XMLNode(node));
	}

	const std::vector<XMLAttribute>& XMLNode::getAttributes() const
	{
		return _attributes;
	}

	void XMLNode::addAttribute(const XMLAttribute& attribute)
	{
		_attributes.push_back(XMLAttribute(attribute));
	}

	XMLNode& XMLNode::operator=(const XMLNode& node)
	{
		_name = node._name;
		_value = node._value;
		_children = node._children;
		_attributes = node._attributes;
		return *this;
	}
}