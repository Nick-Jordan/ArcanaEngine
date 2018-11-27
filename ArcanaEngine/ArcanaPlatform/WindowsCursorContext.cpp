#include "WindowsCursorContext.h"

#include "ArcanaLog.h"

namespace Arcana
{

	CursorContext::CursorContext(Cursor::Type type) : _cursor(0)
	{
		initialize(type);
	}


	CursorContext::~CursorContext()
	{
		destroy();
	}

	void CursorContext::destroy()
	{
		if (_cursor)
		{
			DestroyCursor(_cursor);
			_cursor = 0;
		}
	}

	bool CursorContext::initialize(Cursor::Type type)
	{
		destroy();

		LPCTSTR shape;

		switch (type)
		{
		case Cursor::None:
			break;
		case Cursor::Default:
			shape = IDC_ARROW;
			break;
		case Cursor::TextEditBeam:
			shape = IDC_IBEAM;
			break;
		case Cursor::ResizeLeftRight:
			shape = IDC_SIZEWE;
			break;
		case Cursor::ResizeUpDown:
			shape = IDC_SIZENS;
			break;
		case Cursor::ResizeSouthEast:
			shape = IDC_SIZENWSE;
			break;
		case Cursor::ResizeSouthWest:
			shape = IDC_SIZENESW;
			break;
		case Cursor::CardinalCross:
			shape = IDC_SIZEALL;
			break;
		case Cursor::Crosshairs:
			shape = IDC_CROSS;
			break;
		case Cursor::Hand:
			shape = IDC_HAND;
			break;
		case Cursor::GrabHand:
			break;
		case Cursor::GrabHandClosed:
			break;
		case Cursor::SlashedCircle:
			shape = IDC_NO;
			break;
		case Cursor::EyeDropper:
			break;
		case Cursor::Help:                   
			shape = IDC_HELP;        
			break;
		case Cursor::Wait:
			shape = IDC_WAIT;
			break;
		case Cursor::ArrowWait:
			shape = IDC_APPSTARTING;
			break;
		case Cursor::UpArrow:
			shape = IDC_UPARROW;
			break;
		default:
			break;
		}

		_cursor = CopyCursor(LoadCursor(NULL, shape));

		if (_cursor)
		{
			return true;
		}
		else
		{
			LOG(Error, CoreEngine, "Could not create copy of a system cursor");
			return false;
		}
	}
}