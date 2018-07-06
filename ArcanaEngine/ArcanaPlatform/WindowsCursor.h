//test
#define ARCANA_PLATFORM_WINDOWS

#ifdef ARCANA_PLATFORM_WINDOWS

#ifndef WINDOWS_CURSOR_H_
#define WINDOWS_CURSOR_H_

#include "PlatformDefines.h"
#include "PlatformWindows.h"

#include "Cursor.h"
#include "WindowsWindowContext.h"

namespace Arcana
{
	class ARCANA_PLATFORM_API WindowsCursor : public Cursor
	{
	public:

		WindowsCursor(WindowsWindowContext* parentWindow);

		virtual ~WindowsCursor();


		virtual Vector2i getPosition() const override;

		virtual void setPosition(const int32 x, const int32 y) override;

		virtual void setType(const Type type) override;

		virtual Type getType() const override;

		virtual void getSize(int32& width, int32& height) override;

		virtual void display(bool display) override;

		virtual void lock(Recti rect) override;

		HCURSOR getHCursor();

	private:

		WindowsWindowContext* _parentWindow;
		LPWSTR _windowsType;
		Type _type;
		HCURSOR _cursor;
	};
}

#endif // !WINDOWS_CURSOR_H_

#endif // !ARCANA_PLATFORM_WINDOWS
