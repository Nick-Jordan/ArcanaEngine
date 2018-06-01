#ifndef ARCANA_ENGINE_H_
#define ARCANA_ENGINE_H_

#include "CoreDefines.h"

#include "Object.h"

#include "EventHandler.h"
#include "MainEngineLoop.h"

namespace Arcana
{
	class ARCANA_CORE_API Engine : public Object
	{
	public:

		Engine();


		EventHandler eventHandler;
		MainEngineLoop mainEngineLoop;
	};
}

#endif // !ARCANA_ENGINE_H_