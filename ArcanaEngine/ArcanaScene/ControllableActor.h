#ifndef CONTROLLABLE_ACTOR_H_
#define CONTROLLABLE_ACTOR_H_

#include "SceneDefines.h"

#include "Actor.h"
#include "ActorMovementComponent.h"
#include "InputComponent.h"

namespace Arcana
{
	class ARCANA_SCENE_API ControllableActor : public Actor
	{
		friend class ActorMovementComponent;

	public:

		ControllableActor();

		ControllableActor(const std::string& id);

		virtual ~ControllableActor();

		
		virtual void update(double elapsedTime) override;


		virtual void addPitchInput(float input);

		virtual void addRollInput(float input);

		virtual void addYawInput(float input);

		virtual void addMovementInput(Vector3f direction, float input);//bool force???

		Vector3f resetMovementInput();

		InputComponent* createInputComponent();

		ActorMovementComponent* getMovementComponent() const;

		ActorController* getController() const;

		void setController(ActorController* controller);

		virtual void setRotation(Quaterniond rotation, double elapsedTime);

	private:

		void defaultAddMovementInput(Vector3f input);//bool force???

		Vector3f defaultGetCurrentMovementInput() const;

		Vector3f defaultGetLastMovementInput() const;

		Vector3f defaultResetMovementInput();

	private:

		Vector3f _movementInput;
		Vector3f _lastMovementInput;

		float _yawAllowedError;

		//auto control ai/player /// nagivation generation?

		float _eyeHeight;

		bool _useControllerPitch;
		bool _useControllerYaw;
		bool _useControllerRoll;

		ActorController* _controller;

		//last hit by controller?
	};
}

#endif // !CONTROLLABLE_ACTOR_H_

