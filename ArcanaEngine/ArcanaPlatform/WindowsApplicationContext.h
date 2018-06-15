//test
#define ARCANA_PLATFORM_WINDOWS

#ifdef ARCANA_PLATFORM_WINDOWS

#ifndef WINDOWS_APPLICATION_CONTEXT_H_
#define WINDOWS_APPLICATION_CONTEXT_H_

#include "PlatformDefines.h"

#include "ApplicationContext.h"

namespace Arcana
{
	class ARCANA_PLATFORM_API WindowsApplicationContext : public ApplicationContext
	{
	public:

		WindowsApplicationContext();

		virtual ~WindowsApplicationContext();


		virtual bool create(const ApplicationDefinition& definition) override;
	};
}

#endif // !WINDOWS_APPLICATION_CONTEXT_H_

#endif // ARCANA_PLATFORM_WINDOWS
