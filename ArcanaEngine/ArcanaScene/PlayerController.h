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

		PlayerController(const std::string& id);

		virtual ~PlayerController();

		virtual void initialize(std::string name, const Actor* templateActor = nullptr) override;

		virtual void update(double elapsedTime) override;


		virtual bool isLocalPlayerController() const override;


		virtual void addPitchInput(float input);

		virtual void addYawInput(float input);

		virtual void addRollInput(float input);

	private:

		void updateRotation(double elapsedTime);

	private:

		Quaterniond _rotationPitchInput;
		Quaterniond _rotationYawInput;
		Quaterniond _rotationRollInput;

		float _pitchScale;
		float _yawScale;
		float _rollScale;
	};
}

#endif // !PLAYER_CONTROLLER_H_

