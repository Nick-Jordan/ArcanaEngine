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

	private:

		void loadXMLNodes(XMLNode& parent, rapidxml::xml_node<>* node);

		void loadXMLAttributes(XMLNode& parent, rapidxml::xml_node<>* node);


		bool _initialized;

		std::vector<XMLNode> _nodes;  //SWITCH TO ARRAY FIX COPYING
	};
}

#endif // !XML_FILE_H_