#include "WindowsWindowContext.h"

#ifdef ARCANA_PLATFORM_WINDOWS

#include "WindowsWindowDefinition.h"

namespace Arcana
{

	WindowsWindowContext::WindowsWindowContext() : WindowContext()
	{
	}


	WindowsWindowContext::~WindowsWindowContext()
	{
	}

	bool WindowsWindowContext::create(const WindowDefinition& definition)
	{
		LOG(Info, CoreEngine, "Creating Windows Window Context...");

		try {
			const WindowsWindowDefinition &def = dynamic_cast<const WindowsWindowDefinition&>(definition);

			LOGF(Error, CoreEngine, "Some Data: %s", def.someData.c_str());
		}
		catch (const std::bad_cast& e) {
			LOGF(Error, CoreEngine, "Bad Cast");
			return false;
		}
	}

	bool WindowsWindowContext::destroy()
	{
		return true;
	}
}

#endif // ARCANA_PLATFORM_WINDOWS
