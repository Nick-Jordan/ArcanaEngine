#include "ContextCreator.h"

namespace Arcana
{
	ApplicationContext* ContextCreator::createApplicationContext()
	{
#ifdef ARCANA_PLATFORM_WINDOWS
		return new WindowsApplicationContext(); //test
#else
		return nullptr;
#endif
		//include linux in elif
	}

	WindowContext* ContextCreator::createWindowContext()
	{
#ifdef ARCANA_PLATFORM_WINDOWS
		return new WindowsWindowContext();//test
#else
		return nullptr;
#endif
	}
}
