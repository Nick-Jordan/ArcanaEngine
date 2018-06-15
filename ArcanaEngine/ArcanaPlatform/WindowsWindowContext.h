//test
#define ARCANA_PLATFORM_WINDOWS

#ifdef ARCANA_PLATFORM_WINDOWS

#ifndef WINDOWS_WINDOW_CONTEXT_H_
#define WINDOWS_WINDOW_CONTEXT_H_

#include "PlatformDefines.h"

#include "PlatformWindows.h"
#include "WindowContext.h"

namespace Arcana
{

	class ARCANA_PLATFORM_API WindowsWindowContext : public WindowContext
	{
	public:

		WindowsWindowContext();

		virtual ~WindowsWindowContext();

		virtual bool create(const WindowDefinition& definition) override;

		virtual bool destroy() override;

	private:


	};

}

#endif // !WINDOWS_WINDOW_CONTEXT_H_

#endif // ARCANA_PLATFORM_WINDOWS

