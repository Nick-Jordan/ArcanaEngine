#include "WindowsCursor.h"

#include "Window.h"

namespace Arcana
{

	WindowsCursor::WindowsCursor(WindowsWindowContext* parentWindow) : _parentWindow(parentWindow)
	{
	}


	WindowsCursor::~WindowsCursor()
	{
	}


	Vector2i WindowsCursor::getPosition() const
	{
		POINT p;
		if (GetCursorPos(&p))
		{
			ScreenToClient(_parentWindow->getWindowHandle(), &p);
			return Vector2i(p.x, p.y);
		}

		return Vector2i::zero();
	}

	void WindowsCursor::setPosition(const int32 x, const int32 y)
	{
		SetCursorPos(x, y);
	}

	void WindowsCursor::setType(const Type type)
	{

		switch (type)
		{
		case None:
			_windowsType = NULL;
			break;
		case Default:
			_windowsType = IDC_ARROW;
			break;
		case TextEditBeam:
			_windowsType = IDC_IBEAM;
			break;
		case ResizeLeftRight:
			_windowsType = IDC_SIZEWE;
			break;
		case ResizeUpDown:
			_windowsType = IDC_SIZENS;
			break;
		case ResizeSouthEast:
			_windowsType = IDC_SIZENWSE;
			break;
		case ResizeSouthWest:
			_windowsType = IDC_SIZENESW;
			break;
		case CardinalCross:
			_windowsType = IDC_CROSS;
			break;
		case Crosshairs:
			_windowsType = IDC_SIZEALL;
			break;
		case Hand:
			_windowsType = IDC_HAND;
			break;
		case GrabHand:
			_windowsType = NULL;
			break;
		case GrabHandClosed:
			_windowsType = NULL;
			break;
		case SlashedCircle:
			_windowsType = NULL;
			break;
		case EyeDropper:
			_windowsType = NULL;
			break;
		};

		_type = type;


		if (_windowsType)
		{
			LOG(Error, WindowLog, "Cursor Type is not null");

			_cursor = LoadCursor(_parentWindow->getInstance(), _windowsType);
		}
		else
		{
			LOG(Error, WindowLog, "Cursor Type is null");

			_cursor = NULL;
		}
	}

	Cursor::Type WindowsCursor::getType() const
	{
		return _type;
	}

	void WindowsCursor::getSize(int32& width, int32& height)
	{
		if (_cursor)
		{
			ICONINFO info = { 0 };
			if (GetIconInfo(_cursor, &info) != 0)
			{
				bool c = info.hbmColor == NULL;
				BITMAP bmpinfo = { 0 };
				if (GetObject(info.hbmMask, sizeof(BITMAP), &bmpinfo))
				{
					width = bmpinfo.bmWidth;
					height = abs(bmpinfo.bmHeight) / (c ? 2 : 1);
				}

				DeleteObject(info.hbmColor);
				DeleteObject(info.hbmMask);
			}
		}
	}

	void WindowsCursor::display(bool display)
	{
		if (_cursor)
		{
			ShowCursor(display);
		}
	}

	void WindowsCursor::lock(Recti rect)
	{
		RECT r;
		r.left = rect.getLeft();
		r.right = rect.getRight();
		r.top = rect.getTop();
		r.bottom = rect.getBottom();
		ClipCursor(&r);
	}

	HCURSOR WindowsCursor::getHCursor()
	{
		return _cursor;
	}
}
