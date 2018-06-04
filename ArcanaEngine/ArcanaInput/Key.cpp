#include "Key.h"

namespace Arcana
{
	Key::Key() : _code(Unknown), _id(),
		_isModifierKey(false), _isGamepadKey(false), 
		_isMouseButton(false), _isFloatAxis(false), 
		_isVectorAxis(false)
	{

	}

	Key::Key(const int32& code)
	{
		if (code < 0 || code > NumKeys)
		{
			*this = Keys::ErrorKey;
		}
		else
		{
			*this = Keys::KeyArray[code];
		}
	}

	Key::Key(KeyCode code, const GlobalObjectID id, int32 keyFlags) : _code(code), _id(id)
	{
		readKeyFlags(keyFlags);
	}

	Key::Key(KeyCode code, std::string name, int32 keyFlags) : _code(code), _id(name)
	{
		readKeyFlags(keyFlags);
	}

	Key::~Key()
	{

	}


	bool Key::isModifierKey() const
	{
		return _isModifierKey;
	}

	bool Key::isGamepadKey() const
	{
		return _isGamepadKey;
	}

	bool Key::isMouseButton() const
	{
		return _isMouseButton;
	}

	bool Key::isFloatAxis() const
	{
		return _isFloatAxis;
	}

	bool Key::isVectorAxis() const
	{
		return _isVectorAxis;
	}

	GlobalObjectID Key::getGlobalObjectID() const
	{
		return _id;
	}


	Key& Key::operator=(const Key& key)
	{
		_id = key._id;

		_code = key._code;

		_isModifierKey = key._isModifierKey;
		_isGamepadKey = key._isGamepadKey;
		_isMouseButton = key._isMouseButton;
		_isFloatAxis = key._isFloatAxis;
		_isVectorAxis = key._isVectorAxis;

		return *this;
	}

	void Key::readKeyFlags(int32 keyFlags)
	{
		_isModifierKey = keyFlags & MODIFER_KEY;
		_isGamepadKey = keyFlags & GAMEPAD_KEY;
		_isMouseButton = keyFlags & MOUSE_BUTTON;
		_isFloatAxis = keyFlags & FLOAT_AXIS;
		_isVectorAxis = keyFlags & VECTOR_AXIS;
	}


	const Key Keys::ErrorKey = Key(KeyCode::Unknown, "Error");

	const Key Keys::A = Key(KeyCode::A, "A");
	const Key Keys::B = Key(KeyCode::B, "B");

	const Key Keys::C = Key(KeyCode::C, "C");
	const Key Keys::D = Key(KeyCode::D, "D");
	const Key Keys::E = Key(KeyCode::E, "E");
	const Key Keys::F = Key(KeyCode::F, "F");
	const Key Keys::G = Key(KeyCode::G, "G");
	const Key Keys::H = Key(KeyCode::H, "H");
	const Key Keys::I = Key(KeyCode::I, "I");
	const Key Keys::J = Key(KeyCode::J, "J");
	const Key Keys::K = Key(KeyCode::K, "K");
	const Key Keys::L = Key(KeyCode::L, "L");
	const Key Keys::M = Key(KeyCode::M, "M");
	const Key Keys::N = Key(KeyCode::N, "N");
	const Key Keys::O = Key(KeyCode::O, "O");
	const Key Keys::P = Key(KeyCode::P, "P");
	const Key Keys::Q = Key(KeyCode::Q, "Q");
	const Key Keys::R = Key(KeyCode::R, "R");
	const Key Keys::S = Key(KeyCode::S, "S");
	const Key Keys::T = Key(KeyCode::T, "T");
	const Key Keys::U = Key(KeyCode::U, "U");
	const Key Keys::V = Key(KeyCode::V, "V");
	const Key Keys::W = Key(KeyCode::W, "W");
	const Key Keys::X = Key(KeyCode::X, "X");
	const Key Keys::Y = Key(KeyCode::Y, "Y");
	const Key Keys::Z = Key(KeyCode::Z, "B");

	const Key Keys::Num_0 = Key(KeyCode::Num_0, "0");
	const Key Keys::Num_1 = Key(KeyCode::Num_1, "1");
	const Key Keys::Num_2 = Key(KeyCode::Num_2, "2");
	const Key Keys::Num_3 = Key(KeyCode::Num_3, "3");
	const Key Keys::Num_4 = Key(KeyCode::Num_4, "4");
	const Key Keys::Num_5 = Key(KeyCode::Num_5, "5");
	const Key Keys::Num_6 = Key(KeyCode::Num_6, "6");
	const Key Keys::Num_7 = Key(KeyCode::Num_7, "7");
	const Key Keys::Num_8 = Key(KeyCode::Num_8, "8");
	const Key Keys::Num_9 = Key(KeyCode::Num_9, "9");

	const Key Keys::Escape = Key(KeyCode::Escape, "Escape");
	const Key Keys::LeftControl = Key(KeyCode::LeftControl, "Left Control", Key::MODIFER_KEY);
	const Key Keys::LeftShift = Key(KeyCode::LeftShift, "Left Shift", Key::MODIFER_KEY);
	const Key Keys::LeftAlt = Key(KeyCode::LeftAlt, "Left Alt", Key::MODIFER_KEY);
	const Key Keys::LeftSystem = Key(KeyCode::LeftSystem, "Left System", Key::MODIFER_KEY);
	const Key Keys::RightControl = Key(KeyCode::RightControl, "Right Control", Key::MODIFER_KEY);
	const Key Keys::RightShift = Key(KeyCode::RightShift, "Right Shift", Key::MODIFER_KEY);
	const Key Keys::RightAlt = Key(KeyCode::RightAlt, "Right Alt", Key::MODIFER_KEY);
	const Key Keys::RightSystem = Key(KeyCode::RightSystem, "Right System", Key::MODIFER_KEY);
	const Key Keys::Menu = Key(KeyCode::Menu, "Menu");

	const Key Keys::LeftBracket = Key(KeyCode::LeftBracket, "Left Bracket");
	const Key Keys::RightBracket = Key(KeyCode::RightBracket, "Right Bracket");
	const Key Keys::Semicolon = Key(KeyCode::Semicolon, "Semicolon");
	const Key Keys::Comma = Key(KeyCode::Comma, "Comma");
	const Key Keys::Period = Key(KeyCode::Period, "Period");
	const Key Keys::Quote = Key(KeyCode::Quote, "Quote");
	const Key Keys::Slash = Key(KeyCode::Slash, "Slash");
	const Key Keys::Backslash = Key(KeyCode::Backslash, "Backslash");
	const Key Keys::Tilde = Key(KeyCode::Tilde, "Tilde");
	const Key Keys::Equal = Key(KeyCode::Equal, "Equal");
	const Key Keys::Hyphen = Key(KeyCode::Hyphen, "Hyphen");
	const Key Keys::Space = Key(KeyCode::Space, "Space");
	const Key Keys::CapsLock = Key(KeyCode::CapsLock, "Caps Lock");
	const Key Keys::Enter = Key(KeyCode::Enter, "Enter");
	const Key Keys::Back = Key(KeyCode::Back, "Back");
	const Key Keys::Tab = Key(KeyCode::Tab, "Tab");
	const Key Keys::PageUp = Key(KeyCode::PageUp, "Page Up");
	const Key Keys::PageDown = Key(KeyCode::PageDown, "Page Down");
	const Key Keys::End = Key(KeyCode::End, "End");
	const Key Keys::Home = Key(KeyCode::Home, "Home");
	const Key Keys::Insert = Key(KeyCode::Insert, "Insert");
	const Key Keys::Delete = Key(KeyCode::Delete, "Delete");
	const Key Keys::Add = Key(KeyCode::Add, "Add");
	const Key Keys::Subtract = Key(KeyCode::Subtract, "Subtract");
	const Key Keys::Multiply = Key(KeyCode::Multiply, "Multiply");
	const Key Keys::Divide = Key(KeyCode::Divide, "Divide");
	const Key Keys::Left = Key(KeyCode::Left, "Left");
	const Key Keys::Right = Key(KeyCode::Right, "Right");
	const Key Keys::Up = Key(KeyCode::Up, "Up");
	const Key Keys::Down = Key(KeyCode::Down, "Down");

	const Key Keys::Numpad_0 = Key(KeyCode::Numpad_0, "Numpad 0");
	const Key Keys::Numpad_1 = Key(KeyCode::Numpad_1, "Numpad 1");
	const Key Keys::Numpad_2 = Key(KeyCode::Numpad_2, "Numpad 2");
	const Key Keys::Numpad_3 = Key(KeyCode::Numpad_3, "Numpad 3");
	const Key Keys::Numpad_4 = Key(KeyCode::Numpad_4, "Numpad 4");
	const Key Keys::Numpad_5 = Key(KeyCode::Numpad_5, "Numpad 5");
	const Key Keys::Numpad_6 = Key(KeyCode::Numpad_6, "Numpad 6");
	const Key Keys::Numpad_7 = Key(KeyCode::Numpad_7, "Numpad 7");
	const Key Keys::Numpad_8 = Key(KeyCode::Numpad_8, "Numpad 8");
	const Key Keys::Numpad_9 = Key(KeyCode::Numpad_9, "Numpad 9");
	const Key Keys::F1 = Key(KeyCode::F1, "F1");
	const Key Keys::F2 = Key(KeyCode::F2, "F2");
	const Key Keys::F3 = Key(KeyCode::F3, "F3");
	const Key Keys::F4 = Key(KeyCode::F4, "F4");
	const Key Keys::F5 = Key(KeyCode::F5, "F5");
	const Key Keys::F6 = Key(KeyCode::F6, "F6");
	const Key Keys::F7 = Key(KeyCode::F7, "F7");
	const Key Keys::F8 = Key(KeyCode::F8, "F8");
	const Key Keys::F9 = Key(KeyCode::F9, "F9");
	const Key Keys::F10 = Key(KeyCode::F10, "F10");
	const Key Keys::F11 = Key(KeyCode::F11, "F11");
	const Key Keys::F12 = Key(KeyCode::F12, "F12");
	const Key Keys::F13 = Key(KeyCode::F13, "F13");
	const Key Keys::F14 = Key(KeyCode::F14, "F14");
	const Key Keys::F15 = Key(KeyCode::F15, "F15");
	const Key Keys::Pause = Key(KeyCode::Pause, "Pause");

	const Key Keys::LeftMouseButton = Key(KeyCode::LeftMouseButton, "Left Mouse Button", Key::MOUSE_BUTTON);
	const Key Keys::RightMouseButton = Key(KeyCode::RightMouseButton, "Right Mouse Button", Key::MOUSE_BUTTON);
	const Key Keys::MiddleMouseButton = Key(KeyCode::MiddleMouseButton, "Middle Mouse Button", Key::MOUSE_BUTTON);
	const Key Keys::ThumbMouseButton = Key(KeyCode::ThumbMouseButton, "Thumb Mouse Button", Key::MOUSE_BUTTON);
	const Key Keys::ThumbMouseButton2 = Key(KeyCode::ThumbMouseButton2, "Thumb Mouse Button2", Key::MOUSE_BUTTON);

	const Key Keys::KeyArray[NumKeys] = 
	{
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

		Num_0, Num_1, Num_2, Num_3, Num_4, Num_5, Num_6, Num_7, Num_8, Num_9,

		Escape, LeftControl, LeftShift, LeftAlt, LeftSystem, RightControl, RightShift, RightAlt, RightSystem, Menu,

		LeftBracket, RightBracket, Semicolon, Comma, Period, Quote, Slash, Backslash, Tilde, Equal, Hyphen, Space,
		CapsLock, Enter, Back, Tab, PageUp, PageDown, End, Home, Insert, Delete, Add, Subtract, Multiply, Divide, 
		Left, Right, Up, Down,

		Numpad_0, Numpad_1, Numpad_2, Numpad_3, Numpad_4, Numpad_5, Numpad_6, Numpad_7, Numpad_8, Numpad_9,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, Pause
	};
}