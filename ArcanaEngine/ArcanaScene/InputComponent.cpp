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
		for(auto i = _inputKeyBindings.createIterator(); i; i++)
		{
			InputKeyBinding& keyBinding = *i;

			if(Input::isKeyPressed(keyBinding.key))
			{
				keyBinding.keyCallback.executeIfBound();
			}
		}

		for(auto i = _inputAxisBindings.createIterator(); i; i++)
		{
			InputAxisKeyBinding& axisBinding = *i;

			float value = Input::getFloatAxis(axisBinding.axisKey);
			axisBinding.axisValue = value;

			//if(abs(value) > 0.05f)
			{
				axisBinding.axisCallback.executeIfBound(value);
			}
		}

		for(auto i = _inputVectorAxisBindings.createIterator(); i; i++)
		{
			InputVectorAxisBinding& vectorAxisBinding = *i;

			Vector2f value = Input::getVectorAxis(vectorAxisBinding.vectorAxisKey);
			vectorAxisBinding.vectorAxisValue = value;

			if(value.magnitudeSq() > 0.0f)
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


	void InputComponent::addAxisBinding(const InputAxisKeyBinding& axisBinding)
	{
		_inputAxisBindings.add(axisBinding);
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
