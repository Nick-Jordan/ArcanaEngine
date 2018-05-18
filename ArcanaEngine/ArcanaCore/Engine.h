#ifndef ARCANA_ENGINE_H_
#define ARCANA_ENGINE_H_

#include "ArcanaCoreAPIDefinition.h"

#include "Object.h"

#include "EventHandler.h"

namespace Arcana
{
	class ARCANA_CORE_API Engine : public Object
	{
	public:

		Engine();


		EventHandler eventHandler;
	};
}

#endif // !ARCANA_ENGINE_H_