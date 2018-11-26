#include "WindowsInputContext.h"

namespace Arcana
{
	bool InputContext::isKeyPressed(Key key)
	{
		return false;
	}

	Vector2i InputContext::getMousePosition(WindowHandle handle, const Vector2i& position)
	{
		if (handle)
		{
			POINT point = { position.x, position.y };
			ScreenToClient(handle, &point);
			return Vector2i(point.x, point.y);
		}
		else
		{
			return position;
		}
	}

	void InputContext::setMousePosition(const Vector2i& position)
	{
		SetCursorPos(position.x, position.y);
	}
}