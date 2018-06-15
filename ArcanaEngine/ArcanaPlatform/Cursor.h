#ifndef CURSOR_H_
#define CURSOR_H_

#include "PlatformDefines.h"

#include "Types.h"

namespace Arcana
{

	class Vector2i;

	class ARCANA_PLATFORM_API Cursor
	{
	public:

		enum Type
		{
			None,
			Default,
			TextEditBeam,
			ResizeLeftRight,
			ResizeUpDown,
			ResizeSouthEast,
			ResizeSouthWest,
			CardinalCross,
			Crosshairs,
			Hand,
			GrabHand,
			GrabHandClosed,
			SlashedCircle,
			EyeDropper,

			NumCursors
		};

		virtual Vector2i getPosition() const = 0;

		virtual void setPosition(const int32 x, const int32 y) = 0;

		virtual void setType(const Type type) = 0;

		virtual Type getType() const = 0;

		virtual void getSize(int32& width, int32& height) = 0;

		virtual void display(bool display) = 0;

		virtual void lock() = 0; //rect as args
	};

}

#endif // !CURSOR_H_