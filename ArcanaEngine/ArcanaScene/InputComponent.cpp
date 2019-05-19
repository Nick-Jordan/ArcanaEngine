#include "InputComponent.h"

#include "Input.h"

namespace Arcana
{
	InputComponentManager* InputComponent::Manager = nullptr;

	void InputComponent::createInputComponentManager()
	{
		if (!Manager)
		{
			Manager = new InputComponentManager();
			Manager->reference();
		}
	}

	InputComponent::InputComponent()
	{
		createInputComponentManager();

		if (Manager)
		{
			Manager->reference();

			Manager->registerInputComponent(this);
		}
	}

	InputComponent::~InputComponent()
	{
		AE_RELEASE(Manager);
	}


	void InputComponent::initialize()
	{
	}

	void InputComponent::update(double elapsedTime)
	{
		//for (auto i = _inputKeyBindings.createIterator(); i; i++)
		for (std::vector<InputKeyBinding>::iterator i = _inputKeyBindings.begin();
			i != _inputKeyBindings.end(); i++)
		{
			InputKeyBinding& keyBinding = *i;

			if (keyBinding.repeat && Input::isKeyPressed(keyBinding.key))
			{
				keyBinding.keyCallback.executeIfBound();
			}
		}

		//for (auto i = _inputAxisBindings.createIterator(); i; i++)
		for (std::vector<InputAxisBinding>::iterator i = _inputAxisBindings.begin();
			i != _inputAxisBindings.end(); i++)
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

		//for (auto i = _inputVectorAxisBindings.createIterator(); i; i++)
		for (std::vector<InputVectorAxisBinding>::iterator i = _inputVectorAxisBindings.begin();
			i != _inputVectorAxisBindings.end(); i++)
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
		_inputAxisBindings.push_back(axisBinding);
	}

	void InputComponent::addKeyBinding(const InputKeyBinding& keyBinding)
	{
		_inputKeyBindings.push_back(keyBinding);
	}

	void InputComponent::addVectorAxisBinding(const InputVectorAxisBinding& vectorAxisBinding)
	{
		_inputVectorAxisBindings.push_back(vectorAxisBinding);
	}



	InputComponentManager::InputComponentManager()
	{
		listenForEvent(EventID::MouseEventID);
		listenForEvent(EventID::KeyEventID);
	}

	InputComponentManager::~InputComponentManager()
	{

	}

	void InputComponentManager::registerInputComponent(InputComponent* component)
	{
		if (component)
		{
			component->reference();

			_components.push_back(component);
		}
	}

	void InputComponentManager::removeInputComponent(InputComponent* component)
	{
		if (component)
		{
			_components.erase(std::remove(_components.begin(), _components.end(), component), _components.end());

			AE_RELEASE(component);
		}
	}

	bool InputComponentManager::processEvent(Event& event, EventHandler& handler)
	{
		if (event.getEventId() == EventID::KeyEventID)
		{
			for (std::vector<InputComponent*>::iterator iter = _components.begin();
				iter != _components.end(); iter++)
			{
				InputComponent* component = *iter;

				for (std::vector<InputKeyBinding>::iterator i = component->_inputKeyBindings.begin();
					i != component->_inputKeyBindings.end(); i++)
				{
					InputKeyBinding& keyBinding = *i;

					if (!keyBinding.repeat && keyBinding.key.getKeyCode() == event.getInt("keyCode")
						&& keyBinding.eventType == event.getInt("event"))
					{
						keyBinding.keyCallback.executeIfBound();
					}
				}
			}
		}

		return true;
	}
}
