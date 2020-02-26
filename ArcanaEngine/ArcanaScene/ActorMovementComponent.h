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

		virtual void addMovementInput(Vector3f input); // bool force??

		virtual Vector3f resetMovementInput();

		virtual Vector3f getLastMovementInput() const;

		virtual Vector3f getCurrentMovementInput() const;

		ControllableActor* getControllableActor() const;

		bool isInputEnabled() const;

		//other

	private:

		ControllableActor* _owner;
	};
}

#endif // !ACTOR_MOVEMENT_COMPONENT_H_

