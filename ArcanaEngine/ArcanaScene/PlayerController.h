#ifndef PLAYER_CONTROLLER_H_
#define PLAYER_CONTROLLER_H_

#include "SceneDefines.h"

#include "ActorController.h"

namespace Arcana
{
	class ARCANA_SCENE_API PlayerController : public ActorController
	{
	public:
		PlayerController();
		~PlayerController();
	};
}

#endif // !PLAYER_CONTROLLER_H_

