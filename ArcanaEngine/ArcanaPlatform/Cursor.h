#ifndef CURSOR_H_
#define CURSOR_H_

#include "PlatformDefines.h"

#include "Types.h"
#include "Vector2.h"
#include "Rect.h"

namespace Arcana
{

	class ARCANA_PLATFORM_API CursorContext;

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
			Help,
			Wait,
			ArrowWait,
			UpArrow,

			NumCursors
		};

		Cursor(Type type);

		~Cursor();

		Type getType() const;

		const CursorContext& getContext() const;

	private:

		Type _type;

		CursorContext* _context;
	};

}

#endif // !CURSOR_H_