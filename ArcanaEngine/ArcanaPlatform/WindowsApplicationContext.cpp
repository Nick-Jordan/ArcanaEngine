#include "WindowsApplicationContext.h"

#ifdef ARCANA_PLATFORM_WINDOWS

namespace Arcana
{

	WindowsApplicationContext::WindowsApplicationContext()
	{
	}


	WindowsApplicationContext::~WindowsApplicationContext()
	{
	}

	bool WindowsApplicationContext::create(const ApplicationDefinition& definition)
	{
		LOG(Info, CoreEngine, "Creating Windows Application Context...");

		try {
			const WindowsApplicationDefinition &def = dynamic_cast<const WindowsApplicationDefinition&>(definition);

			LOGF(Debug, CoreEngine, "Show Command: %i", def.getShowCommand());

		}
		catch (const std::bad_cast& e) {
			LOGF(Error, CoreEngine, "Bad Cast");
			return false;
		}

		LOG(Info, CoreEngine, "Done creating Windows Application Context.");

		return true;
	}
}

#endif // ARCANA_PLATFORM_WINDOWS
