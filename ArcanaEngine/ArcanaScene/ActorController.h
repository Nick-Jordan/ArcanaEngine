/*#ifndef ACTOR_CONTROLLER_H_
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

		virtual void attach(Actor* actor);

		virtual void detach();

		//get character?

		virtual void setControllerRotation(const Quaterniond& rotation);

		virtual const Quaterniond& getControllerRotation() const;

		virtual const Quaterniond& getDesiredRotation() const;

		ControlActorCallback& getOnControlActorCallback();

		Actor* getActor() const;

		//get player state?

		//get player viewpoint

		virtual void getControllerViewPoint(Vector3d& location, Quaterniond& rotation);

		//get state name?

		//get scene component? (root scene component from actor class)

		virtual Actor* getViewTarget() const;

		//nagivation control?

		//state management (is in state)

		//is local controller / is local player controller

		virtual bool usesLookInput() const;

		virtual bool usesMovementInput() const;

		bool isPlayerController() const;

		virtual bool hasClearLineOfSight(const Actor* actor, Vector3d position, bool alternateChecks);

		virtual void control(Actor* actor);

		virtual void setControllerRotation(const Quaterniond& rotation);

		virtual void setUseLookInput(bool input);

		virtual void setUseMovementInput(bool input);

		//stop movement?

		virtual void releaseControl();

		virtual void update(double elapsedTime) override;

	private:

		Actor* _actor;

		//Character* _character; // possibly same as actor

		Quaterniond _controllerRotation;

		bool _isPlayerController;
		bool _attachToActor;
		bool _useLookInput;
		bool _useMovementInput;

		//state?
		//state name?

		ControlActorCallback _controlActorCallback;
	};

}

#endif // !ACTOR_CONTROLLER_H_
*/

