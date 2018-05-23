#ifndef ENGINE_LOOP_H_
#define ENGINE_LOOP_H_

#include "ArcanaCoreAPIDefinition.h"

#include "Types.h"

namespace Arcana
{
	class EngineLoop
	{
	public:
		
		virtual int32 initialize() = 0;
		
		virtual void update() = 0;
		
		virtual void clearPendingCleanupObjects() = 0;
	};
}

#endif