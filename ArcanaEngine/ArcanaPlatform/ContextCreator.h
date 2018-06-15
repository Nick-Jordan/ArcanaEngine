//test
#define ARCANA_PLATFORM_WINDOWS

#ifndef CONTEXT_CREATOR_H_
#define CONTEXT_CREATOR_H_

#include "PlatformDefines.h"

#include "ApplicationContext.h"
#include "WindowContext.h"

#ifdef ARCANA_PLATFORM_WINDOWS
#include "WindowsApplicationContext.h"
#include "WindowsWindowContext.h"
#endif

//include linux contexts

namespace Arcana
{

	class ContextCreator
	{
	public:
		
		static ApplicationContext* createApplicationContext();

		static WindowContext* createWindowContext();
	};
}

#endif // !CONTEXT_CREATOR_H_

