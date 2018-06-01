// ArcanaInput.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ArcanaInput.h"

namespace Arcana
{
	ArcanaInput::ArcanaInput()
	{

	}

	ArcanaInput::~ArcanaInput()
	{

	}

	bool ArcanaInput::startUp()
	{
		return true;
	}

	bool ArcanaInput::shutDown()
	{
		return true;
	}

	bool ArcanaInput::isGameModule()
	{
		return false;
	}
}


