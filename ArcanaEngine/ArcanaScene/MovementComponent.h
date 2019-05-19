#ifndef MOVEMENT_COMPONENT_H_
#define MOVEMENT_COMPONENT_H_

#include "SceneDefines.h"

#include "SceneComponent.h"

namespace Arcana
{
	class ARCANA_SCENE_API MovementComponent : public ActorComponent
	{
	public:

		MovementComponent();

		~MovementComponent();

		SceneComponent* getMovingSceneComponent() const;

	private:

		SceneComponent* _sceneComponent;
	};
}

#endif // !MOVEMENT_COMPONENT_H_

