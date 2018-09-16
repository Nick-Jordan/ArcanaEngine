// XMLClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "XMLFile.h"

#include <vld.h>

using namespace Arcana;

/*void printNode(const XMLNode& node)
{
	LOGF(Info, CoreEngine, "Node: %s", node.getName().c_str());
	for (auto iter = node.getAttributes().createConstIterator(); iter; ++iter)
	{
		LOGF(Info, CoreEngine, "Attribute: %s, %s", (*iter).getName().c_str(), (*iter).getValue().c_str());
	}
	LOGF(Info, CoreEngine, "Num Children: %d", node.getChildren().size());
	for (auto iter = node.getChildren().createConstIterator(); iter; ++iter)
	{
		printNode((*iter));
	}
}*/

void printNode(const XMLNode& node)
{
	LOGF(Info, CoreEngine, "Node: %s", node.getName().c_str());
	//for (auto iter = node.getAttributes().begin(); iter != node.getAttributes().end(); ++iter)
	{
	//	LOGF(Info, CoreEngine, "Attribute: %s, %s", (*iter).getName().c_str(), (*iter).getValue().c_str());
	}
	for (auto iter = node.getChildren().begin(); iter != node.getChildren().end(); ++iter)
	{
		printNode((*iter));
	}
}

int main()
{
	XMLFile file("test_file.xml");

	//for (auto iter = file.getNodes().createConstIterator(); iter; ++iter)
	{
		//printNode((*iter));
	}


	XMLFile file2("resource_database.xml");
	//for (auto iter = file2.getNodes().createConstIterator(); iter; ++iter)
	{
		//printNode((*iter));
	}

	system("pause");

    return 0;
}

