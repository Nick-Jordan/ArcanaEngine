// SceneModule.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SceneModule.h"

namespace Arcana
{
	SceneModule::SceneModule()
	{

	}

	SceneModule::~SceneModule()
	{

	}

	bool SceneModule::startUp()
	{
		return true;
	}

	bool SceneModule::shutDown()
	{
		return true;
	}

	bool SceneModule::isGameModule()
	{
		return false;
	}
}


