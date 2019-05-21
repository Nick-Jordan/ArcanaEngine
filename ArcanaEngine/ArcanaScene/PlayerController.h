#ifndef PLAYER_CONTROLLER_H_
#define PLAYER_CONTROLLER_H_

#include "SceneDefines.h"

#include "ActorController.h"
#include "Callback.h"

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

		virtual void destroyed() override;


		virtual void setControllerRotation(const Quaterniond& rotation) override;

		virtual const Quaterniond& getDesiredRotation() const override;

		virtual bool isPlayerController() const override; //local???


		double getRotationSpeed() const;

		void setRotationSpeed(double rotationSpeed);

		double getRotationSensitivity() const;

		void setRotationSensitivity(double rotationSensitivity);

		void addPitchInput(float input);

		void addRollInput(float input);

		void addYawInput(float input);

	private:

		void updateRotation(double elapsedTime);

		void updateDesiredRotation();

	private:

		Quaterniond _desiredRotation;

		Vector3f _tempRotation;

		double _rotationSpeed;
		double _rotationSensitivity;
	};

}

#endif // !PLAYER_CONTROLLER_H_

