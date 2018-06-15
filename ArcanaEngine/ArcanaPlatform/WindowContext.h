#ifndef WINDOW_CONTEXT_H_
#define WINDOW_CONTEXT_H_

#include "PlatformDefines.h"

#include "WindowHandle.h"
#include "WindowDefinition.h"

#include "ArcanaLog.h"

namespace Arcana
{

	class ARCANA_PLATFORM_API WindowContext
	{
	public:

		WindowContext() { };

		virtual ~WindowContext() { };

		virtual bool create(const WindowDefinition& definition) = 0;

		virtual bool destroy() = 0;
	};
}

#endif // !WINDOW_CONTEXT_H_

