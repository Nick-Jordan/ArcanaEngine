#ifndef ACTOR_CONTROLLER_H_
#define ACTOR_CONTROLLER_H_

#include "SceneDefines.h"

#include "Actor.h"

namespace Arcana
{

	class ARCANA_SCENE_API ActorController : public Actor
	{
	public:
		ActorController();
		virtual ~ActorController();
	};

}

#endif // !ACTOR_CONTROLLER_H_

