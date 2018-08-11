#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "InputDefines.h"

#include "Types.h"
#include "ControllerUtils.h"

#include "Key.h"
#include "Vector2.h"

namespace Arcana
{

	class ARCANA_INPUT_API Controller
	{
	public:

		static const uint32 MaxControllers = 8;
		static const uint32 ButtonCount = 16;
		static const uint32 FloatAxisCount = 6;

		struct ARCANA_INPUT_API Specification
		{
			Specification();

			std::string name;
			uint32 manufacturer;
			uint32 productId;
		};

		static bool isConnected(uint32 controllerId);

		static bool isButtonPressed(uint32 controllerId, Key button);

		static float getFloatAxis(uint32 controllerId, Key axis);

		static Vector2f getVectorAxis(uint32 controllerId, Key axis);

		static Specification getSpecification(uint32 controllerId);

		static void update();
	};

}

#endif // !CONTROLLER_H_

