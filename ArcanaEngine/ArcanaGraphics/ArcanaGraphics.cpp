// ArcanaGraphics.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ArcanaGraphics.h"

namespace Arcana
{
	ArcanaGraphics::ArcanaGraphics()
	{

	}

	ArcanaGraphics::~ArcanaGraphics()
	{

	}

	bool ArcanaGraphics::startUp()
	{
		return true;
	}

	bool ArcanaGraphics::shutDown()
	{
		return true;
	}

	bool ArcanaGraphics::isGameModule()
	{
		return false;
	}
}


