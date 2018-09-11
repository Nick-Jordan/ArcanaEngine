#include "XMLFile.h"

#include "rapidxml/include.h"
#include "ArcanaLog.h"

namespace Arcana
{
	XMLFile::XMLFile() : _initialized(false)
	{

	}

	XMLFile::XMLFile(const std::string& filename) : _initialized(false)
	{
		create(filename);
	}

	XMLFile::~XMLFile()
	{
	}

	bool XMLFile::create(const std::string& filename)
	{
		if (_initialized)
		{
			LOGF(Debug, CoreEngine, "Reinitializing XMLFile with new data from file '%s'", filename.c_str());
			_nodes.empty();
		}

		_initialized = true;

		try {
			rapidxml::file<> xmlFile(filename.c_str());
			rapidxml::xml_document<> doc;
			doc.parse<0>(xmlFile.data());

			rapidxml::xml_node<>* node = doc.first_node();
			for (rapidxml::xml_node<>* child = node->first_node(); child; child = child->next_sibling())
			{
				XMLNode n;
				n.setName(std::string(child->name()));
				//LOGF(Error, CoreEngine, "PARENT: %s", n.getName().c_str());
				loadXMLNodes(n, child);
				loadXMLAttributes(n, child);

				_nodes.add(n);
			}

		}
		catch (const std::exception&) 
		{
			LOGF(Error, CoreEngine, "Failed to open xml file '%s'.", filename.c_str());
			return false;
		}

		return true;
	}

	const Array<XMLNode>& XMLFile::getNodes() const
	{
		return _nodes;
	}

	void XMLFile::loadXMLNodes(XMLNode& parent, rapidxml::xml_node<>* node)
	{
		for (rapidxml::xml_node<>* child = node->first_node(); child; child = child->next_sibling())
		{			
			std::string name = std::string(child->name());
			if (name.size() > 0)
			{
				XMLNode n;
				n.setName(name);
				//LOGF(Error, CoreEngine, "CHILD: %s", name.c_str());
				loadXMLNodes(n, child);
				loadXMLAttributes(n, child);

				parent.addChild(n);
			}
		}
	}

	void XMLFile::loadXMLAttributes(XMLNode& parent, rapidxml::xml_node<>* node)
	{
		for (rapidxml::xml_attribute<>* attrib = node->first_attribute(); attrib; attrib = attrib->next_attribute())
		{
			XMLAttribute attribute;

			attribute.setName(std::string(attrib->name()));
			attribute.setValue(std::string(attrib->value()));

			parent.addAttribute(attribute);
		}
	}
}