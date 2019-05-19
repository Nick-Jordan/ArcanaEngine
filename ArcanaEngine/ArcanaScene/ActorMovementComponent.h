#ifndef ACTOR_MOVEMENT_COMPONENT_H_
#define ACTOR_MOVEMENT_COMPONENT_H_

#include "SceneDefines.h"

#include "MovementComponent.h"
#include "Vector3.h"

namespace Arcana
{
	class ARCANA_SCENE_API ControllableActor;

	class ARCANA_SCENE_API ActorMovementComponent : public MovementComponent
	{
	public:

		ActorMovementComponent();

		~ActorMovementComponent();

		void addMovementInput(Vector3f input); // bool force??

		Vector3f resetMovementInput();

		Vector3f getLastMovementInput() const;

		Vector3f getCurrentMovementInput() const;

		ControllableActor* getControllableActor() const;

		bool isInputEnabled() const;

		//other

	private:

		ControllableActor* _owner;
	};
}

#endif // !ACTOR_MOVEMENT_COMPONENT_H_

