#ifndef FPS_CHARACTER_H_
#define FPS_CHARACTER_H_

#include "Character.h"

namespace Arcana
{

	class FPSCharacter : public Character
	{
	public:

		FPSCharacter();

		~FPSCharacter();

		virtual void begin() override;

		virtual void setupInputComponent(InputComponent* inputComponent) override;

		void moveForward(float input);

		void moveRight(float input);

		void moveUp(float input);

		void lookHorizontal(float input);

		void lookVertical(float input);

		void roll(float input);

		void startJump();

		void stopJump();

	
	};

}

#endif // !FPS_CHARACTER_H_
