#include "InputBindings.h"

namespace Arcana
{
	void InputAxis::addKeyMapping(Key key, float axisValue)
	{
		KeyMapping keyMapping;
		keyMapping.key = key;
		keyMapping.axisValue = axisValue;
		_keyMappings.push_back(keyMapping);
	}


	InputAxisBinding::InputAxisBinding() : axis(), axisValue(0.0f)
	{

	}

	InputAxisBinding::InputAxisBinding(InputAxis axis) : axis(axis), axisValue(0.0f)
	{

	}

	InputAxisBinding::InputAxisBinding(const InputAxisBinding& copy) : axis(copy.axis), axisValue(copy.axisValue), axisCallback(copy.axisCallback)
	{

	}


	InputAxisKeyBinding::InputAxisKeyBinding() : axisKey(Keys::ErrorKey), axisValue(0.0f)
	{

	}

	InputAxisKeyBinding::InputAxisKeyBinding(Key key) : axisKey(key), axisValue(0.0f)
	{

	}

	InputAxisKeyBinding::InputAxisKeyBinding(const InputAxisKeyBinding& copy) : axisKey(copy.axisKey), axisValue(copy.axisValue), axisCallback(copy.axisCallback)
	{

	}



	InputKeyBinding::InputKeyBinding() : key(Keys::ErrorKey), eventType(KeyEvent::Type::Pressed)
	{

	}

	InputKeyBinding::InputKeyBinding(Key key, KeyEvent::Type type) : key(key), eventType(type)
	{

	}

	InputKeyBinding::InputKeyBinding(const InputKeyBinding& copy) : key(copy.key), eventType(copy.eventType), keyCallback(copy.keyCallback)
	{

	}



	InputVectorAxisBinding::InputVectorAxisBinding() : vectorAxisKey(Keys::ErrorKey), vectorAxisValue(Vector2f::zero())
	{

	}

	InputVectorAxisBinding::InputVectorAxisBinding(Key key) : vectorAxisKey(key), vectorAxisValue(Vector2f::zero())
	{

	}

	InputVectorAxisBinding::InputVectorAxisBinding(const InputVectorAxisBinding& copy) : vectorAxisKey(Keys::ErrorKey), vectorAxisValue(Vector2f::zero()), vectorAxisCallback(copy.vectorAxisCallback)
	{

	}
}