#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "SceneDefines.h"

#include "PlayerController.h"
#include "InputComponent.h"

namespace Arcana
{

	class ARCANA_SCENE_API Character : public ControllableActor
	{
	public:

		Character();

		virtual ~Character();

		virtual void initialize(std::string name, const Actor* templateActor = nullptr) override;

		virtual void update(double elapsedTime) override;

		virtual void begin() override;


		virtual void setupInputComponent(InputComponent* inputComponent);


		virtual void addPitchInput(float input);

		virtual void addRollInput(float input);

		virtual void addYawInput(float input);

	private:

		PlayerController* _playerController;
	};

}

#endif // !CHARACTER_H_

