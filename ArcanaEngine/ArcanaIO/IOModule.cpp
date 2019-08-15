#include "IOModule.h"

namespace Arcana
{

	IOModule::IOModule()
	{
	}


	IOModule::~IOModule()
	{
	}

	bool IOModule::startUp()
	{
		return true;
	}

	bool IOModule::shutDown()
	{
		return true;
	}

	bool IOModule::isGameModule()
	{
		return false;
	}

	const char* IOModule::getModuleName() const
	{
		return "IO";
	}
}
