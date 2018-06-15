#include "ModifierKeysState.h"

namespace Arcana
{

	ModifierKeysState::ModifierKeysState() :
		_isLeftShiftDown(false),
		_isRightShiftDown(false),
		_isLeftControlDown(false),
		_isRightControlDown(false),
		_isLeftAltDown(false),
		_isRightAltDown(false),
		_isLeftSystemDown(false),
		_isRightSystemDown(false),
		_areCapsLocked(false)
	{

	}

	ModifierKeysState::ModifierKeysState(const bool leftShiftDown,
		const bool rightShiftDown,
		const bool leftControlDown,
		const bool rightControlDown,
		const bool leftAltDown,
		const bool rightAltDown,
		const bool leftSystemDown,
		const bool rightSystemDown,
		const bool capsLocked) : 
		_isLeftShiftDown(leftShiftDown),
		_isRightShiftDown(rightShiftDown),
		_isLeftControlDown(leftControlDown),
		_isRightControlDown(rightControlDown),
		_isLeftAltDown(leftAltDown),
		_isRightAltDown(rightAltDown),
		_isLeftSystemDown(leftSystemDown),
		_isRightSystemDown(rightSystemDown),
		_areCapsLocked(capsLocked)
	{

	}

	bool ModifierKeysState::isShiftDown() const
	{
		return _isLeftShiftDown || _isRightShiftDown;
	}

	bool ModifierKeysState::isLeftShiftDown() const
	{
		return _isLeftShiftDown;
	}

	bool ModifierKeysState::isRightShiftDown() const
	{
		return _isRightShiftDown;
	}

	bool ModifierKeysState::isControlDown() const
	{
		return _isLeftControlDown || _isRightControlDown;
	}

	bool ModifierKeysState::isLeftControlDown() const
	{
		return _isLeftControlDown;
	}

	bool ModifierKeysState::isRightControlDown() const
	{
		return _isRightControlDown;
	}

	bool ModifierKeysState::isAltDown() const
	{
		return _isLeftAltDown || _isRightAltDown;
	}

	bool ModifierKeysState::isLeftAltDown() const
	{
		return _isLeftAltDown;
	}

	bool ModifierKeysState::isRightAltDown() const
	{
		return _isRightAltDown;
	}

	bool ModifierKeysState::isSystemDown() const
	{
		return _isLeftSystemDown || _isRightSystemDown;
	}

	bool ModifierKeysState::isLeftSystemDown() const
	{
		return _isLeftSystemDown;
	}

	bool ModifierKeysState::isRightSystemDown() const
	{
		return _isRightSystemDown;
	}

	bool ModifierKeysState::areCapsLocked() const
	{
		return _areCapsLocked;
	}

	bool ModifierKeysState::areModifiersDown(ModifierKey keys) const
	{
		bool allModifersDown = true;

		if ((keys & ModifierKey::Shift) == ModifierKey::Shift)
		{
			allModifersDown &= isShiftDown();
		}
		if ((keys & ModifierKey::System) == ModifierKey::System)
		{
			allModifersDown &= isSystemDown();
		}
		if ((keys & ModifierKey::Control) == ModifierKey::Control)
		{
			allModifersDown &= isControlDown();
		}
		if ((keys & ModifierKey::Alt) == ModifierKey::Alt)
		{
			allModifersDown &= isAltDown();
		}

		return allModifersDown;
	}


	ModifierKey ModifierKeysState::flagFromBools(const bool control, const bool alt, const bool shift, const bool system)
	{
		uint8 modifierMask = ModifierKey::None;
		if (control) modifierMask |= ModifierKey::Control;
		if (alt) modifierMask |= ModifierKey::Alt;
		if (shift) modifierMask |= ModifierKey::Shift;
		if (system)	modifierMask |= ModifierKey::System;

		return (ModifierKey)modifierMask;
	}

}
