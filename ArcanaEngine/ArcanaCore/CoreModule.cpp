#include "CoreModule.h"

namespace Arcana
{
	CoreModule::CoreModule()
	{

	}

	CoreModule::~CoreModule()
	{

	}

	bool CoreModule::startUp()
	{
		return true;
	}

	bool CoreModule::shutDown()
	{
		return true;
	}

	bool CoreModule::isGameModule()
	{
		return false;
	}

	const char* CoreModule::getModuleName() const
	{
		return "Core";
	}
}