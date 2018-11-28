#include "InputComponent.h"

#include "Input.h"

namespace Arcana
{
	InputComponent::InputComponent()
	{
	}

	InputComponent::~InputComponent()
	{

	}


	void InputComponent::initialize()
	{

	}

	void InputComponent::update(double elapsedTime)
	{
		for (auto i = _inputKeyBindings.createIterator(); i; i++)
		{
			InputKeyBinding& keyBinding = *i;

			if (Input::isKeyPressed(keyBinding.key))
			{
				keyBinding.keyCallback.executeIfBound();
			}
		}

		for (auto i = _inputAxisBindings.createIterator(); i; i++)
		{
			InputAxisBinding& axisBinding = *i;

			float value = 0.0;

			std::vector<KeyMapping>::const_iterator j;
			for (j = axisBinding.axis._keyMappings.begin(); j != axisBinding.axis._keyMappings.end(); j++)
			{
				const KeyMapping& keyMapping = *j;

				if (keyMapping.key.isFloatAxis())
				{
					value += keyMapping.axisValue * Input::getFloatAxis(keyMapping.key);
				}
				else if (Input::isKeyPressed(keyMapping.key))
				{
					value += keyMapping.axisValue;
				}
			}

			axisBinding.axisValue = value;
			axisBinding.axisCallback.executeIfBound(value);
		}

		for (auto i = _inputAxisKeyBindings.createIterator(); i; i++)
		{
			InputAxisKeyBinding& axisBinding = *i;

			float value = Input::getFloatAxis(axisBinding.axisKey);
			axisBinding.axisValue = value;
			axisBinding.axisCallback.executeIfBound(value);
		}

		for (auto i = _inputVectorAxisBindings.createIterator(); i; i++)
		{
			InputVectorAxisBinding& vectorAxisBinding = *i;

			Vector2f value = Input::getVectorAxis(vectorAxisBinding.vectorAxisKey);
			vectorAxisBinding.vectorAxisValue = value;

			if (value.magnitudeSq() > 0.0f)
			{
				vectorAxisBinding.vectorAxisCallback.executeIfBound(value);
			}
		}
	}

	void InputComponent::registered()
	{

	}

	void InputComponent::unregistered()
	{

	}

	void InputComponent::componentDestroyed()
	{

	}


	void InputComponent::addAxisBinding(const InputAxisBinding& axisBinding)
	{
		_inputAxisBindings.add(axisBinding);
	}

	void InputComponent::addAxisKeyBinding(const InputAxisKeyBinding& axisBinding)
	{
		_inputAxisKeyBindings.add(axisBinding);
	}

	void InputComponent::addKeyBinding(const InputKeyBinding& keyBinding)
	{
		_inputKeyBindings.add(keyBinding);
	}

	void InputComponent::addVectorAxisBinding(const InputVectorAxisBinding& vectorAxisBinding)
	{
		_inputVectorAxisBindings.add(vectorAxisBinding);
	}
}
