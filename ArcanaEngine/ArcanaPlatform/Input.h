#ifndef INPUT_H_
#define INPUT_H_

#include "PlatformDefines.h"

#include "Key.h"
#include "Vector2.h"
#include "Window.h"

namespace Arcana
{
	class ARCANA_PLATFORM_API Input
	{
	public:

		static bool isKeyPressed(Key key);

		static bool isControllerConnected(uint32 controllerId);

		static float getControllerFloatAxis(uint32 controllerId, Key key);

		static Vector2f getControllerVectorAxis(uint32 controllerId, Key axis);

		static Vector2i getMousePosition();

		static Vector2i getMousePosition(const Window& relativeWindow);

		static void setMousePosition(const Vector2i& position);

		static void setMousePosition(const Vector2i& position, const Window& relativeWindow);

		static float getMouseWheelPosition();

	private:

		static Vector2i __absolutePosition;
	};
}

#endif // !INPUT_H_