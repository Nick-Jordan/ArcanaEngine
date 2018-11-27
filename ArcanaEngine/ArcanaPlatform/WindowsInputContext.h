#ifndef WINDOWS_INPUT_CONTEXT_H_
#define WINDOWS_INPUT_CONTEXT_H_

#include "PlatformDefines.h"

#include "Key.h"
#include "Vector2.h"
#include "WindowHandle.h"

namespace Arcana
{
	class ARCANA_PLATFORM_API InputContext
	{
	public:
		
		static bool isKeyPressed(Key key);

		static Vector2i getMousePosition();

		static Vector2i getMousePosition(WindowHandle handle, const Vector2i& position);
		
		static void setMousePosition(const Vector2i& position);

		static Key keyConversion(WPARAM key, LPARAM flags);

		static int32 keyConversion(Key key);
	};
}

#endif // !WINDOWS_INPUT_CONTEXT_H_

