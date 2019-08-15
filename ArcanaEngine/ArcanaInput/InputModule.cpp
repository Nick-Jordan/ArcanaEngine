// InputModule.cpp : Defines the exported functions for the DLL application.
//

#include "InputModule.h"

namespace Arcana
{
	InputModule::InputModule()
	{

	}

	InputModule::~InputModule()
	{

	}

	bool InputModule::startUp()
	{
		return true;
	}

	bool InputModule::shutDown()
	{
		return true;
	}

	bool InputModule::isGameModule()
	{
		return false;
	}

	const char* InputModule::getModuleName() const
	{
		return "Input";
	}
}


