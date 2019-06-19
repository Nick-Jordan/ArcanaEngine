#ifndef RESOURCE_LOGGERS_H_
#define RESOURCE_LOGGERS_H_

#include "ResourceDefines.h"
#include "ArcanaLog.h"

namespace Arcana
{
	REGISTER_CATEGORY_ENGINE(ARCANA_RESOURCE_API, ResourceLog, none);
	REGISTER_CATEGORY_ENGINE(ARCANA_RESOURCE_API, ResourceDatabaseLog, none);
}

#endif // !RESOURCE_LOGGERS_H_