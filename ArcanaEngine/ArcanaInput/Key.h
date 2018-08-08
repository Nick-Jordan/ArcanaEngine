#ifndef KEY_H_
#define KEY_H_

#include "InputDefines.h"

#include "GlobalObjectID.h"

namespace Arcana
{

	enum ARCANA_INPUT_API KeyCode : int32
	{
		Unknown = -1,

		A = 0,
		B = 1,
		C = 2,
		D = 3,
		E = 4,
		F = 5,
		G = 6,
		H = 7,
		I = 8,
		J = 9,
		K = 10,
		L = 11,
		M = 12,
		N = 13,
		O = 14,
		P = 15,
		Q = 16,
		R = 17,
		S = 18,
		T = 19,
		U = 20,
		V = 21,
		W = 22,
		X = 23,
		Y = 24,
		Z = 25,

		Num_0 = 26,
		Num_1 = 27,
		Num_2 = 28,
		Num_3 = 29,
		Num_4 = 30,
		Num_5 = 31,
		Num_6 = 32,
		Num_7 = 33,
		Num_8 = 34,
		Num_9 = 35,

		Escape = 36,
		LeftControl = 37,
		LeftShift = 38,
		LeftAlt = 39,
		LeftSystem = 40,
		RightControl = 41,
		RightShift = 42,
		RightAlt = 43,
		RightSystem = 44,
		Menu = 45,

		LeftBracket = 46,
		RightBracket = 47,
		Semicolon = 48,
		Comma = 49,
		Period = 50,
		Quote = 51,
		Slash = 52,
		Backslash = 53,
		Tilde = 54,
		Equal = 55,
		Hyphen = 56,
		Space = 57,
		CapsLock = 58,
		Enter = 59,
		Back = 60,
		Tab = 61,
		PageUp = 62,
		PageDown = 63,
		End = 64,
		Home = 65,
		Insert = 66,
		Delete = 67,
		Add = 68,
		Subtract = 69,
		Multiply = 70,
		Divide = 71,
		Left = 72,
		Right = 73,
		Up = 74,
		Down = 75,

		Numpad_0 = 76,
		Numpad_1 = 77,
		Numpad_2 = 78,
		Numpad_3 = 79,
		Numpad_4 = 80,
		Numpad_5 = 81,
		Numpad_6 = 82,
		Numpad_7 = 83,
		Numpad_8 = 84,
		Numpad_9 = 85,
		F1 = 86,
		F2 = 87,
		F3 = 88,
		F4 = 89,
		F5 = 90,
		F6 = 91,
		F7 = 92,
		F8 = 93,
		F9 = 94,
		F10 = 95,
		F11 = 96,
		F12 = 97,
		F13 = 98,
		F14 = 99,
		F15 = 100,
		Pause = 101,

		LeftMouseButton = 102,
		RightMouseButton = 103,
		MiddleMouseButton = 104,
		ThumbMouseButton1 = 105,
		ThumbMouseButton2 = 106,

		GamepadLeftAnalogY = 107,
		GamepadLeftAnalogX = 108,

		GamepadRightAnalogY = 109,
		GamepadRightAnalogX = 110,

		GamepadLeftTriggerAxis = 111,
		GamepadRightTriggerAxis = 112,

		GamepadFaceButtonBottom = 113,
		GamepadFaceButtonRight = 114,
		GamepadFaceButtonLeft = 115,
		GamepadFaceButtonTop = 116,
		GamepadLeftShoulder = 117,
		GamepadRightShoulder = 118,
		GamepadSpecialLeft = 119,
		GamepadSpecialRight = 120,
		GamepadLeftThumbstick = 121,
		GamepadRightThumbstick = 122,
		GamepadLeftTriggerThreshold = 123,
		GamepadRightTriggerThreshold = 124,
		GamepadDPadUp = 125,
		GamepadDPadDown = 126,
		GamepadDPadLeft = 127,
		GamepadDPadRight = 128,

		GamepadLeftStickUp = 129,
		GamepadLeftStickDown = 130,
		GamepadLeftStickLeft = 131,
		GamepadLeftStickRight = 132,

		GamepadRightStickUp = 133,
		GamepadRightStickDown = 134,
		GamepadRightStickLeft = 135,
		GamepadRightStickRight = 136,

		NumKeys
	};

	enum ARCANA_INPUT_API MouseButton : int32
	{
		MouseLeft,
		MouseRight,
		MouseMiddle,
		MouseThumb1,
		MouseThumb2,

		NumMouseButtons
	};

	enum ARCANA_INPUT_API ControllerButton : int32
	{
		LeftAnalogY,
		LeftAnalogX,

		RightAnalogY,
		RightAnalogX,

		LeftTriggerAxis,
		RightTriggerAxis,

		FaceButtonBottom,
		FaceButtonRight,
		FaceButtonLeft,
		FaceButtonTop,
		LeftShoulder,
		RightShoulder,
		SpecialLeft,
		SpecialRight,
		LeftThumbstick,
		RightThumbstick,
		LeftTriggerThreshold,
		RightTriggerThreshold,
		DPadUp,
		DPadDown,
		DPadLeft,
		DPadRight,

		LeftStickUp,
		LeftStickDown,
		LeftStickLeft,
		LeftStickRight,

		RightStickUp,
		RightStickDown,
		RightStickLeft,
		RightStickRight,

		/*// Steam Controller
		Touch0,
		Touch1,
		Touch2,
		Touch3,
		BackLeft,
		BackRight,

		// global speech commands
		GlobalMenu,
		GlobalView,
		GlobalPause,
		GlobalPlay,
		GlobalBack,

		AndroidBack,
		AndroidVolumeUp,
		AndroidVolumeDown,
		AndroidMenu,*/

		NumControllerButtons,
	};

	class ARCANA_INPUT_API Key
	{
	public:

		enum Flags : int32
		{
			None = 0,
			ModifierKey = 1 << 0,
			GamepadKey = 1 << 1,
			MouseButton = 1 << 2,
			FloatAxis = 1 << 3,
			VectorAxis = 1 << 4
		};


		Key();

		Key(const int32& code);

		Key(KeyCode code, const GlobalObjectID id, int32 keyFlags = None);

		Key(KeyCode code, std::string name, int32 keyFlags = None);

		~Key();


		bool isModifierKey() const;

		bool isGamepadKey() const;

		bool isMouseButton() const;

		bool isFloatAxis() const;

		bool isVectorAxis() const;

		GlobalObjectID getGlobalObjectID() const;

		KeyCode getKeyCode() const;


		Key& operator=(const Key& key);

		bool operator==(const int32& code);

		bool operator==(const KeyCode& code);

		bool operator!=(const int32& code);

		bool operator!=(const KeyCode& code);

	private:

		void readKeyFlags(int32 keyFlags);


		GlobalObjectID _id;

		KeyCode _code;

		bool _isModifierKey;
		bool _isGamepadKey;
		bool _isMouseButton;
		bool _isFloatAxis;
		bool _isVectorAxis;
	};


	class ARCANA_INPUT_API Keys
	{
		friend class Key;

	public:

		static const Key ErrorKey;

		static const Key A;
		static const Key B;
		static const Key C;
		static const Key D;
		static const Key E;
		static const Key F;
		static const Key G;
		static const Key H;
		static const Key I;
		static const Key J;
		static const Key K;
		static const Key L;
		static const Key M;
		static const Key N;
		static const Key O;
		static const Key P;
		static const Key Q;
		static const Key R;
		static const Key S;
		static const Key T;
		static const Key U;
		static const Key V;
		static const Key W;
		static const Key X;
		static const Key Y;
		static const Key Z;

		static const Key Num_0;
		static const Key Num_1;
		static const Key Num_2;
		static const Key Num_3;
		static const Key Num_4;
		static const Key Num_5;
		static const Key Num_6;
		static const Key Num_7;
		static const Key Num_8;
		static const Key Num_9;

		static const Key Escape;
		static const Key LeftControl;
		static const Key LeftShift;
		static const Key LeftAlt;
		static const Key LeftSystem;
		static const Key RightControl;
		static const Key RightShift;
		static const Key RightAlt;
		static const Key RightSystem;
		static const Key Menu;

		static const Key LeftBracket;
		static const Key RightBracket;
		static const Key Semicolon;
		static const Key Comma;
		static const Key Period;
		static const Key Quote;
		static const Key Slash;
		static const Key Backslash;
		static const Key Tilde;
		static const Key Equal;
		static const Key Hyphen;
		static const Key Space;
		static const Key CapsLock;
		static const Key Enter;
		static const Key Back;
		static const Key Tab;
		static const Key PageUp;
		static const Key PageDown;
		static const Key End;
		static const Key Home;
		static const Key Insert;
		static const Key Delete;
		static const Key Add;
		static const Key Subtract;
		static const Key Multiply;
		static const Key Divide;
		static const Key Left;
		static const Key Right;
		static const Key Up;
		static const Key Down;

		static const Key Numpad_0;
		static const Key Numpad_1;
		static const Key Numpad_2;
		static const Key Numpad_3;
		static const Key Numpad_4;
		static const Key Numpad_5;
		static const Key Numpad_6;
		static const Key Numpad_7;
		static const Key Numpad_8;
		static const Key Numpad_9;
		static const Key F1;
		static const Key F2;
		static const Key F3;
		static const Key F4;
		static const Key F5;
		static const Key F6;
		static const Key F7;
		static const Key F8;
		static const Key F9;
		static const Key F10;
		static const Key F11;
		static const Key F12;
		static const Key F13;
		static const Key F14;
		static const Key F15;
		static const Key Pause;

		static const Key LeftMouseButton;
		static const Key RightMouseButton;
		static const Key MiddleMouseButton;
		static const Key ThumbMouseButton1;
		static const Key ThumbMouseButton2;

		static const Key GamepadLeftAnalogX;
		static const Key GamepadLeftAnalogY;
		static const Key GamepadRightAnalogX;
		static const Key GamepadRightAnalogY;
		static const Key GamepadLeftTriggerAxis;
		static const Key GamepadRightTriggerAxis;

		static const Key GamepadLeftThumbstick;
		static const Key GamepadRightThumbstick;
		static const Key GamepadSpecialLeft;
		static const Key GamepadSpecialRight;
		static const Key GamepadFaceButtonBottom;
		static const Key GamepadFaceButtonRight;
		static const Key GamepadFaceButtonLeft;
		static const Key GamepadFaceButtonTop;
		static const Key GamepadLeftShoulder;
		static const Key GamepadRightShoulder;
		static const Key GamepadLeftTrigger;
		static const Key GamepadRightTrigger;
		static const Key GamepadDPadUp;
		static const Key GamepadDPadDown;
		static const Key GamepadDPadRight;
		static const Key GamepadDPadLeft;

		static const Key GamepadLeftStickUp;
		static const Key GamepadLeftStickDown;
		static const Key GamepadLeftStickRight;
		static const Key GamepadLeftStickLeft;

		static const Key GamepadRightStickUp;
		static const Key GamepadRightStickDown;
		static const Key GamepadRightStickRight;
		static const Key GamepadRightStickLeft;

		static Key get(int32 index);

	private:

		static const Key KeyArray[KeyCode::NumKeys];
	};

}

#endif // !KEY_H_

