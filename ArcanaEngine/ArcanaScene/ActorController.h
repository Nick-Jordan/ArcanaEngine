#ifndef ACTOR_CONTROLLER_H_
#define ACTOR_CONTROLLER_H_

#include "SceneDefines.h"

#include "Actor.h"
#include "Callback.h"

namespace Arcana
{
	REGISTER_CALLBACK(ControlActorCallback, Actor*)

	class ARCANA_SCENE_API ActorController : public Actor
	{
	public:

		ActorController(const std::string& id);

		virtual ~ActorController();


		virtual void initialize(std::string name, const Actor* templateActor = nullptr) override;

		virtual void update(double elapsedTime) override;

		virtual void destroyed() override;


		virtual void attach(Actor* actor);

		virtual void detach();

		virtual void setControllerRotation(const Quaterniond& rotation);

		virtual const Quaterniond& getControllerRotation() const;

		virtual const Quaterniond& getDesiredRotation() const;

		ControlActorCallback& getOnControlActorCallback();

		virtual void getControllerViewPoint(Vector3d& location, Quaterniond& rotation);

		Actor* getActor() const;

		bool isUsingLookInput() const;

		void setUseLookInput(bool useLookInput);

		bool isUsingMovementInput() const;

		void setUseMovementInput(bool useMovementInput);

		bool isPlayerController() const;

		virtual void control(Actor* actor);

		virtual void releaseControl();

	private:

		Actor* _controllingActor;

		bool _attachToActor;
		bool _isPlayerController;

		bool _useLookInput;
		bool _useMovementInput;

		Quaterniond _controllerRotation;

		ControlActorCallback _controlActorCallback;
	};

}

#endif // !ACTOR_CONTROLLER_H_

