#ifndef XML_FILE_H_
#define XML_FILE_H_

#include "IODefines.h"

#include "Types.h"
#include "XMLNode.h"

#include "rapidxml/include.h"

namespace Arcana
{

	class ARCANA_IO_API XMLFile
	{
	public:

		XMLFile();

		XMLFile(const std::string& filename);

		~XMLFile();


		bool create(const std::string& filename);

		const std::vector<XMLNode>& getNodes() const; //SWITCH TO ARRAY FIX COPYING

		const XMLNode& getRoot() const;

	private:

		void loadXMLNodes(XMLNode& parent, rapidxml::xml_node<>* node, bool addToNodes);

		void loadXMLAttributes(XMLNode& parent, rapidxml::xml_node<>* node);

	private:

		bool _initialized;

		XMLNode _root;
		std::vector<XMLNode> _nodes;  //SWITCH TO ARRAY FIX COPYING
	};
}

#endif // !XML_FILE_H_