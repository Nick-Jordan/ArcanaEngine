#include "InputBindings.h"

namespace Arcana
{
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