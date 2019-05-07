#include "WindowsInputContext.h"

namespace Arcana
{
	bool InputContext::isKeyPressed(Key key)
	{
		int32 windowsKey = keyConversion(key);

		return (GetAsyncKeyState(windowsKey) & 0x8000) != 0;
	}

	Vector2i InputContext::getMousePosition()
	{
		POINT point;
		GetCursorPos(&point);
		return Vector2i(point.x, point.y);
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

	Key InputContext::keyConversion(WPARAM key, LPARAM flags)
	{
		switch (key)
		{
		case 'A':			return Keys::A;
		case 'B':			return Keys::B;
		case 'C':			return Keys::C;
		case 'D':			return Keys::D;
		case 'E':			return Keys::E;
		case 'F':			return Keys::F;
		case 'G':			return Keys::G;
		case 'H':			return Keys::H;
		case 'I':			return Keys::I;
		case 'J':			return Keys::J;
		case 'K':			return Keys::K;
		case 'L':			return Keys::L;
		case 'M':			return Keys::M;
		case 'N':			return Keys::N;
		case 'O':			return Keys::O;
		case 'P':			return Keys::P;
		case 'Q':			return Keys::Q;
		case 'R':			return Keys::R;
		case 'S':			return Keys::S;
		case 'T':			return Keys::T;
		case 'U':			return Keys::U;
		case 'V':			return Keys::V;
		case 'W':			return Keys::W;
		case 'X':			return Keys::X;
		case 'Y':			return Keys::Y;
		case 'Z':			return Keys::Z;

		case '0':			return Keys::Num_0;
		case '1':			return Keys::Num_1;
		case '2':			return Keys::Num_2;
		case '3':			return Keys::Num_3;
		case '4':			return Keys::Num_4;
		case '5':			return Keys::Num_5;
		case '6':			return Keys::Num_6;
		case '7':			return Keys::Num_7;
		case '8':			return Keys::Num_8;
		case '9':			return Keys::Num_9;

		case VK_ESCAPE:		return Keys::Escape;
		case VK_LMENU:
		case VK_RMENU:
		case VK_APPS:		return Keys::Menu;

		case VK_OEM_4:		return Keys::LeftBracket;
		case VK_OEM_6:		return Keys::RightBracket;
		case VK_OEM_1:		return Keys::Semicolon;
		case VK_OEM_COMMA:	return Keys::Comma;
		case VK_DECIMAL:
		case VK_OEM_PERIOD: return Keys::Period;
		case VK_OEM_7:		return Keys::Quote;
		case VK_OEM_2:		return Keys::Slash;
		case VK_OEM_5:		return Keys::Backslash;
		case VK_OEM_3:		return Keys::Tilde;
		case VK_OEM_PLUS:	return Keys::Equal;
		case VK_OEM_MINUS:	return Keys::Hyphen;
		case VK_SPACE:		return Keys::Space;
		case VK_CAPITAL:	return Keys::CapsLock;
		case VK_RETURN:		return Keys::Enter;

		case VK_BACK:		return Keys::Back;
		case VK_TAB:		return Keys::Tab;
		case VK_PRIOR:		return Keys::PageUp;
		case VK_NEXT:		return Keys::PageDown;
		case VK_END:		return Keys::End;
		case VK_HOME:		return Keys::Home;
		case VK_INSERT:		return Keys::Insert;
		case VK_DELETE:		return Keys::Delete;
		case VK_ADD: 		return Keys::Add;
		case VK_SUBTRACT:	return Keys::Subtract;
		case VK_MULTIPLY:	return Keys::Multiply;
		case VK_DIVIDE:		return Keys::Divide;
		case VK_LEFT:		return Keys::Left;
		case VK_RIGHT:		return Keys::Right;
		case VK_UP:			return Keys::Up;
		case VK_DOWN:		return Keys::Down;

		case VK_NUMPAD0:	return Keys::Numpad_0;
		case VK_NUMPAD1:	return Keys::Numpad_1;
		case VK_NUMPAD2:	return Keys::Numpad_2;
		case VK_NUMPAD3:	return Keys::Numpad_3;
		case VK_NUMPAD4:	return Keys::Numpad_4;
		case VK_NUMPAD5:	return Keys::Numpad_5;
		case VK_NUMPAD6:	return Keys::Numpad_6;
		case VK_NUMPAD7:	return Keys::Numpad_7;
		case VK_NUMPAD8:	return Keys::Numpad_8;
		case VK_NUMPAD9:	return Keys::Numpad_9;

		case VK_F1:			return Keys::F1;
		case VK_F2:			return Keys::F2;
		case VK_F3:			return Keys::F3;
		case VK_F4:			return Keys::F4;
		case VK_F5:			return Keys::F5;
		case VK_F6:			return Keys::F6;
		case VK_F7:			return Keys::F7;
		case VK_F8:			return Keys::F8;
		case VK_F9:			return Keys::F9;
		case VK_F10:		return Keys::F10;
		case VK_F11:		return Keys::F11;
		case VK_F12:		return Keys::F12;
		case VK_F13:		return Keys::F13;
		case VK_F14:		return Keys::F14;
		case VK_F15:		return Keys::F15;
		case VK_PAUSE:		return Keys::Pause;

		case VK_LBUTTON: return KeyCode::LeftMouseButton;
		case VK_RBUTTON: return KeyCode::RightMouseButton;
		case VK_MBUTTON: return KeyCode::MiddleMouseButton;
		case VK_XBUTTON1: return KeyCode::ThumbMouseButton1;
		case VK_XBUTTON2: return KeyCode::ThumbMouseButton2;

		case VK_SHIFT:
		{
			static UINT lShift = MapVirtualKeyW(VK_LSHIFT, MAPVK_VK_TO_VSC);
			UINT code = static_cast<UINT>((flags & (0xFF << 16)) >> 16);
			return code == lShift ? Keys::LeftShift : Keys::RightShift;
		}
		case VK_MENU: return (HIWORD(flags) & KF_EXTENDED) ? Keys::RightAlt : Keys::LeftAlt;

		case VK_CONTROL: return (HIWORD(flags) & KF_EXTENDED) ? Keys::RightControl : Keys::LeftControl;

		case VK_LWIN: return Keys::LeftSystem;
		case VK_RWIN: return Keys::RightSystem;

		}

		return Keys::ErrorKey;
	}

	int32 InputContext::keyConversion(Key key)
	{
		int32 windowsKey = 0;

		switch (key.getKeyCode())
		{
			case KeyCode::A:				windowsKey = 'A';           break;
			case KeyCode::B:				windowsKey = 'B';           break;
			case KeyCode::C:				windowsKey = 'C';           break;
			case KeyCode::D:				windowsKey = 'D';           break;
			case KeyCode::E:				windowsKey = 'E';           break;
			case KeyCode::F:				windowsKey = 'F';           break;
			case KeyCode::G:				windowsKey = 'G';           break;
			case KeyCode::H:				windowsKey = 'H';           break;
			case KeyCode::I:				windowsKey = 'I';           break;
			case KeyCode::J:				windowsKey = 'J';           break;
			case KeyCode::K:				windowsKey = 'K';           break;
			case KeyCode::L:				windowsKey = 'L';           break;
			case KeyCode::M:				windowsKey = 'M';           break;
			case KeyCode::N:				windowsKey = 'N';           break;
			case KeyCode::O:				windowsKey = 'O';           break;
			case KeyCode::P:				windowsKey = 'P';           break;
			case KeyCode::Q:				windowsKey = 'Q';           break;
			case KeyCode::R:				windowsKey = 'R';           break;
			case KeyCode::S:				windowsKey = 'S';           break;
			case KeyCode::T:				windowsKey = 'T';           break;
			case KeyCode::U:				windowsKey = 'U';           break;
			case KeyCode::V:				windowsKey = 'V';           break;
			case KeyCode::W:				windowsKey = 'W';           break;
			case KeyCode::X:				windowsKey = 'X';           break;
			case KeyCode::Y:				windowsKey = 'Y';           break;
			case KeyCode::Z:				windowsKey = 'Z';           break;

			case KeyCode::Num_0:			windowsKey = '0';           break;
			case KeyCode::Num_1:			windowsKey = '1';           break;
			case KeyCode::Num_2:			windowsKey = '2';           break;
			case KeyCode::Num_3:			windowsKey = '3';           break;
			case KeyCode::Num_4:			windowsKey = '4';           break;
			case KeyCode::Num_5:			windowsKey = '5';           break;
			case KeyCode::Num_6:			windowsKey = '6';           break;
			case KeyCode::Num_7:			windowsKey = '7';           break;
			case KeyCode::Num_8:			windowsKey = '8';           break;
			case KeyCode::Num_9:			windowsKey = '9';           break;

			case KeyCode::Escape:			windowsKey = VK_ESCAPE;     break;
			case KeyCode::Menu:				windowsKey = VK_APPS;       break;

			case KeyCode::LeftBracket:		windowsKey = VK_OEM_4;      break;
			case KeyCode::RightBracket: 	windowsKey = VK_OEM_6;      break;
			case KeyCode::Semicolon:		windowsKey = VK_OEM_1;      break;
			case KeyCode::Comma:			windowsKey = VK_OEM_COMMA;  break;
			case KeyCode::Period:			windowsKey = VK_OEM_PERIOD; break;
			case KeyCode::Quote:			windowsKey = VK_OEM_7;      break;
			case KeyCode::Slash:			windowsKey = VK_OEM_2;      break;
			case KeyCode::Backslash:		windowsKey = VK_OEM_5;      break;
			case KeyCode::Tilde:			windowsKey = VK_OEM_3;      break;
			case KeyCode::Equal:			windowsKey = VK_OEM_PLUS;   break;
			case KeyCode::Hyphen:			windowsKey = VK_OEM_MINUS;  break;
			case KeyCode::Space:			windowsKey = VK_SPACE;      break;
			case KeyCode::CapsLock:			windowsKey = VK_CAPITAL;	break;
			case KeyCode::Enter:			windowsKey = VK_RETURN;     break;

			case KeyCode::Back:				windowsKey = VK_BACK;       break;
			case KeyCode::Tab:				windowsKey = VK_TAB;        break;
			case KeyCode::PageUp:			windowsKey = VK_PRIOR;      break;
			case KeyCode::PageDown:			windowsKey = VK_NEXT;       break;
			case KeyCode::End:				windowsKey = VK_END;        break;
			case KeyCode::Home:				windowsKey = VK_HOME;       break;
			case KeyCode::Insert:			windowsKey = VK_INSERT;     break;
			case KeyCode::Delete:			windowsKey = VK_DELETE;     break;
			case KeyCode::Add:				windowsKey = VK_ADD;        break;
			case KeyCode::Subtract:			windowsKey = VK_SUBTRACT;   break;
			case KeyCode::Multiply:			windowsKey = VK_MULTIPLY;   break;
			case KeyCode::Divide:			windowsKey = VK_DIVIDE;     break;
			case KeyCode::Left:				windowsKey = VK_LEFT;       break;
			case KeyCode::Right:			windowsKey = VK_RIGHT;      break;
			case KeyCode::Up:				windowsKey = VK_UP;         break;
			case KeyCode::Down:				windowsKey = VK_DOWN;       break;

			case KeyCode::Numpad_0:			windowsKey = VK_NUMPAD0;    break;
			case KeyCode::Numpad_1:			windowsKey = VK_NUMPAD1;    break;
			case KeyCode::Numpad_2:			windowsKey = VK_NUMPAD2;    break;
			case KeyCode::Numpad_3:			windowsKey = VK_NUMPAD3;    break;
			case KeyCode::Numpad_4:			windowsKey = VK_NUMPAD4;    break;
			case KeyCode::Numpad_5:			windowsKey = VK_NUMPAD5;    break;
			case KeyCode::Numpad_6:			windowsKey = VK_NUMPAD6;    break;
			case KeyCode::Numpad_7:			windowsKey = VK_NUMPAD7;    break;
			case KeyCode::Numpad_8:			windowsKey = VK_NUMPAD8;    break;
			case KeyCode::Numpad_9:			windowsKey = VK_NUMPAD9;    break;
			case KeyCode::F1:				windowsKey = VK_F1;         break;
			case KeyCode::F2:				windowsKey = VK_F2;         break;
			case KeyCode::F3:				windowsKey = VK_F3;         break;
			case KeyCode::F4:				windowsKey = VK_F4;         break;
			case KeyCode::F5:				windowsKey = VK_F5;         break;
			case KeyCode::F6:				windowsKey = VK_F6;         break;
			case KeyCode::F7:				windowsKey = VK_F7;         break;
			case KeyCode::F8:				windowsKey = VK_F8;         break;
			case KeyCode::F9:				windowsKey = VK_F9;         break;
			case KeyCode::F10:				windowsKey = VK_F10;        break;
			case KeyCode::F11:				windowsKey = VK_F11;        break;
			case KeyCode::F12:				windowsKey = VK_F12;        break;
			case KeyCode::F13:				windowsKey = VK_F13;        break;
			case KeyCode::F14:				windowsKey = VK_F14;        break;
			case KeyCode::F15:				windowsKey = VK_F15;        break;
			case KeyCode::Pause:			windowsKey = VK_PAUSE;      break;

			case KeyCode::LeftControl:		windowsKey = VK_LCONTROL;   break;
			case KeyCode::RightControl:		windowsKey = VK_RCONTROL;   break;
			case KeyCode::LeftShift:		windowsKey = VK_LSHIFT;     break;
			case KeyCode::RightShift:		windowsKey = VK_RSHIFT;     break;
			case KeyCode::LeftAlt:			windowsKey = VK_LMENU;      break;
			case KeyCode::RightAlt:			windowsKey = VK_RMENU;      break;
			case KeyCode::LeftSystem:		windowsKey = VK_LWIN;       break;
			case KeyCode::RightSystem:		windowsKey = VK_RWIN;       break;

			case KeyCode::LeftMouseButton:	windowsKey = VK_LBUTTON;	break;
			case KeyCode::RightMouseButton: windowsKey = VK_RBUTTON;	break;
			case KeyCode::MiddleMouseButton: windowsKey = VK_MBUTTON;	break;
			case KeyCode::ThumbMouseButton1: windowsKey = VK_XBUTTON1;	break;
			case KeyCode::ThumbMouseButton2: windowsKey = VK_XBUTTON2;	break;

		default:
			break;
		}

		return windowsKey;
	}
}