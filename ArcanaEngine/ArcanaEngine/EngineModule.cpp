#include "EngineModule.h"

namespace Arcana
{
	EngineModule::EngineModule()
	{

	}

	EngineModule::~EngineModule()
	{

	}

	bool EngineModule::startUp()
	{
		return true;
	}

	bool EngineModule::shutDown()
	{
		return true;
	}

	bool EngineModule::isGameModule()
	{
		return false;
	}

	const char* EngineModule::getModuleName() const
	{
		return "Engine";
	}
}