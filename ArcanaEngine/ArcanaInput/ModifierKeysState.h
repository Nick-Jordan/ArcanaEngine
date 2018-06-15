#ifndef MODIFIER_KEYS_STATE
#define MODIFIER_KEYS_STATE

#include "InputDefines.h"

#include "Key.h"

namespace Arcana
{

	enum ARCANA_INPUT_API ModifierKey : uint8
	{
		None = 0,
		Control = 1 << 0,
		Alt = 1 << 1,
		Shift = 1 << 2,
		System = 1 << 3
	};

	class ARCANA_INPUT_API ModifierKeysState
	{
	public:

		ModifierKeysState();

		ModifierKeysState(const bool leftShiftDown,
			const bool rightShiftDown,
			const bool leftControlDown,
			const bool rightControlDown,
			const bool leftAltDown,
			const bool rightAltDown,
			const bool leftSystemDown,
			const bool rightSystemDown,
			const bool capsLocked);

		bool isShiftDown() const;

		bool isLeftShiftDown() const;

		bool isRightShiftDown() const;

		bool isControlDown() const;

		bool isLeftControlDown() const;

		bool isRightControlDown() const;

		bool isAltDown() const;

		bool isLeftAltDown() const;

		bool isRightAltDown() const;

		bool isSystemDown() const;

		bool isLeftSystemDown() const;

		bool isRightSystemDown() const;

		bool areCapsLocked() const;

		bool areModifiersDown(ModifierKey keys) const;


		static ModifierKey flagFromBools(const bool control, const bool alt, const bool shift, const bool system);

	private:

		bool _isLeftShiftDown;

		bool _isRightShiftDown;

		bool _isLeftControlDown;

		bool _isRightControlDown;

		bool _isLeftAltDown;

		bool _isRightAltDown;

		bool _isLeftSystemDown;

		bool _isRightSystemDown;

		bool _areCapsLocked;
	};

}

#endif // !MODIFIER_KEYS_STATE

