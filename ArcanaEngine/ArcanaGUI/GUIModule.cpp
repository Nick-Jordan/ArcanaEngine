#include "GUIModule.h"

namespace Arcana
{

	GUIModule::GUIModule()
	{
	}


	GUIModule::~GUIModule()
	{
	}

	bool GUIModule::startUp()
	{
		return true;
	}

	bool GUIModule::shutDown()
	{
		return true;
	}

	bool GUIModule::isGameModule()
	{
		return false;
	}

}
