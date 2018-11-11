#ifndef INPUT_H_
#define INPUT_H_

#include "InputDefines.h"

#include "Key.h"
#include "Types.h"
#include "Vector2.h"

namespace Arcana
{

	class ARCANA_INPUT_API Input
	{
	public:

		static bool isControllerConnected(uint32 controllerId);

		static bool isControllerButtonPressed(uint32 controllerId, Key button);

		static float getControllerFloatAxis(uint32 controllerId, Key axis);

		static Vector2f getControllerVectorAxis(uint32 controllerId, Key axis);

		static void update();
	};

}

#endif // !INPUT_H_

