#ifndef WINDOWS_CURSOR_CONTEXT_H_
#define WINDOWS_CURSOR_CONTEXT_H_

#include "PlatformDefines.h"

#include "Cursor.h"
#include <Windows.h>

namespace Arcana
{

	class ARCANA_PLATFORM_API CursorContext
	{
		friend class WindowsWindowContext;

	public:

		CursorContext(Cursor::Type type);

		~CursorContext();

	private:

		void destroy();

		bool initialize(Cursor::Type type);

	private:

		HCURSOR _cursor;
	};

}

#endif // !WINDOWS_CURSOR_CONTEXT_H_

