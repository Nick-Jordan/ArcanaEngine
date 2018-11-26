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

		static Input& instance();

		bool isKeyPressed(Key key);

		bool isControllerConnected(uint32 controllerId);

		float getControllerFloatAxis(uint32 controllerId, Key key);

		Vector2f getControllerVectorAxis(uint32 controllerId, Key axis);

		void setMouseSmoothingEnabled(bool smoothing);

		bool isMouseSmoothingEnabled() const;

		Vector2i getMousePosition() const;

		Vector2i getMousePosition(const Window& relativeWindow) const;

		void setMousePosition(const Vector2i& position);

		void setMousePosition(const Vector2i& position, const Window& relativeWindow);

		Vector2f getRelativeMousePosition() const;

		float getMouseWheelPosition() const;

	private:

		Input();

	private:

		Vector2i _absolutePosition;
		Vector2f _relativePosition;
		bool _filterMouse;
	};
}

#endif // !INPUT_H_