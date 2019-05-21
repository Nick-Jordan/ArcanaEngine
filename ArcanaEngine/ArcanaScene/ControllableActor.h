#ifndef CONTROLLABLE_ACTOR_H_
#define CONTROLLABLE_ACTOR_H_

#include "SceneDefines.h"

#include "Actor.h"
#include "ActorMovementComponent.h"

namespace Arcana
{
	class ARCANA_SCENE_API ActorController;

	REGISTER_CALLBACK(OnControlledCallback, ActorController*);

	class ARCANA_SCENE_API ControllableActor : public Actor
	{
		friend class ActorController;
		friend class ActorMovementComponent;

	public:

		ControllableActor();

		ControllableActor(const std::string& id);

		~ControllableActor();


		virtual void update(double elapsedTime) override;


		ActorController* getController() const;

		OnControlledCallback& getOnControlledCallback();


		Vector3f resetMovementInput();

		virtual void addMovementInput(Vector3f direction, float input);

		ActorMovementComponent* getMovementComponent() const;

	private:

		void setController(ActorController* controller);

		void defaultAddMovementInput(Vector3f input);//bool force???

		Vector3f defaultResetMovementInput();

	private:

		ActorController* _controller;

		OnControlledCallback _controlledCallback;

		Vector3f _movementInput;
		Vector3f _lastMovementInput;
	};
}

#endif // !CONTROLLABLE_ACTOR_H_