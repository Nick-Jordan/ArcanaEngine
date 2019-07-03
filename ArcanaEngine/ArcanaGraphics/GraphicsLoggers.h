#ifndef GRAPHICS_LOGGERS_H_
#define GRAPHICS_LOGGERS_H_

#include "GraphicsDefines.h"

#include "ArcanaLog.h"

namespace Arcana
{
	REGISTER_CATEGORY_ENGINE(ARCANA_GRAPHICS_API, PostProcessorLog, none);
	REGISTER_CATEGORY_ENGINE(ARCANA_GRAPHICS_API, PostProcessQueueLog, none);
}

#endif // !GRAPHICS_LOGGERS_H_