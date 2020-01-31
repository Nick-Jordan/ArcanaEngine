#include "PhysicsModule.h"

namespace Arcana
{
	PhysicsModule::PhysicsModule()
	{

	}

	PhysicsModule::~PhysicsModule()
	{

	}

	bool PhysicsModule::startUp()
	{
		return true;
	}

	bool PhysicsModule::shutDown()
	{
		return true;
	}

	bool PhysicsModule::isGameModule()
	{
		return false;
	}

	const char* PhysicsModule::getModuleName() const
	{
		return "Physics";
	}
}


