#ifndef PLAYER_CONTROLLER_H_
#define PLAYER_CONTROLLER_H_

#include "SceneDefines.h"

#include "ActorController.h"

namespace Arcana
{
	class ARCANA_SCENE_API PlayerController : public ActorController
	{
	public:

		PlayerController(const std::string& id);

		virtual ~PlayerController();


		/*virtual void initialize(std::string name, const Actor* templateActor = nullptr) override;

		virtual void update(double elapsedTime) override;

		virtual void destroyed() override;


		virtual void attach(Actor* actor) override;

		virtual void detach() override;

		virtual void setControllerRotation(const Quaterniond& rotation) override;

		virtual const Quaterniond& getControllerRotation() const override;

		virtual const Quaterniond& getDesiredRotation() const override;

		virtual void getControllerViewPoint(Vector3d& location, Quaterniond& rotation) override;

		virtual bool isPlayerController() const override;

		virtual void control(Actor* actor) override;

		virtual void releaseControl() override;*/
	};
}

#endif // !PLAYER_CONTROLLER_H_

