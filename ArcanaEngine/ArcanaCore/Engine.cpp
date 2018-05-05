// ArcanaCore.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "Engine.h"
#include "ArcanaLog.h"

namespace Arcana
{
	Engine::Engine() : Object("Engine")
	{
		LOG(Debug, CoreEngine, "Engine created");
	}
}
