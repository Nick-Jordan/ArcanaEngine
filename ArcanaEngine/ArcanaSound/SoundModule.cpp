// SoundModule.cpp : Defines the exported functions for the DLL application.
//

#include "SoundModule.h"

namespace Arcana
{
	SoundModule::SoundModule()
	{

	}

	SoundModule::~SoundModule()
	{

	}

	bool SoundModule::startUp()
	{
		return true;
	}

	bool SoundModule::shutDown()
	{
		return true;
	}

	bool SoundModule::isGameModule()
	{
		return false;
	}
}


