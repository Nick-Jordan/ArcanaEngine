#ifndef INPUT_COMPONENT_H_
#define INPUT_COMPONENT_H_

#include "SceneDefines.h"

#include "ActorComponent.h"
#include "InputBindings.h"
#include "Array.h"
#include "AxisMapping.h"
#include "ActionMapping.h"
#include "EventListener.h"

namespace Arcana
{
	class ARCANA_SCENE_API InputComponentManager : public EventListener, public Object
	{
	public:

		InputComponentManager();

		~InputComponentManager();

		void registerInputComponent(InputComponent* component);

		void removeInputComponent(InputComponent* component);

		virtual bool processEvent(Event& event, EventHandler& handler) override;

	private:

		std::vector<InputComponent*> _components;
	};

	class ARCANA_SCENE_API InputComponent : public ActorComponent
	{
		friend class InputComponentManager;

	public:

		static InputComponentManager* Manager;

	public:

		static void createInputComponentManager();

	public:

		InputComponent();

		virtual ~InputComponent();


		virtual void initialize() override;

		virtual void update(double elapsedTime) override;

		virtual void registered() override;

		virtual void unregistered() override;

		virtual void componentDestroyed() override;



		void addAxisBinding(const InputAxisBinding& axisBinding);

		void addKeyBinding(const InputKeyBinding& keyBinding);

		void addVectorAxisBinding(const InputVectorAxisBinding& vectorAxisBinding);


		template<class UserObject>
		void bindAxisMapping(const std::string& axis, UserObject* object, void(UserObject::*function)(float));

		template<class UserObject>
		void bindActionMapping(const std::string& action, KeyEvent::Type event, bool repeat, UserObject* object, void(UserObject::*function)());

	private:

		std::vector<InputAxisBinding> _inputAxisBindings;
		std::vector<InputKeyBinding> _inputKeyBindings;
		std::vector<InputVectorAxisBinding> _inputVectorAxisBindings;
	};

	template<class UserObject>
	void InputComponent::bindAxisMapping(const std::string& axis, UserObject* object, void(UserObject::*function)(float))
	{
		const AxisMapping* mapping = AxisMapping::get(axis);

		if (mapping)
		{
			InputAxisBinding axisBinding;
			axisBinding.axisCallback.bind(object, function);

			for (uint32 i = 0; i < mapping->_keyMappings.size(); i++)
			{
				axisBinding.axis.addKeyMapping(mapping->_keyMappings[i].key, mapping->_keyMappings[i].axisValue);
			}

			addAxisBinding(axisBinding);
		}
		else
		{
			LOGF(Warning, CoreEngine, "Could not find axis mapping: '%s'.", axis.c_str());
		}
	}

	template<class UserObject>
	void InputComponent::bindActionMapping(const std::string& action, KeyEvent::Type event, bool repeat, UserObject* object, void(UserObject::*function)())
	{
		const ActionMapping* mapping = ActionMapping::get(action);

		if (mapping)
		{
			InputKeyBinding actionBinding;
			actionBinding.keyCallback.bind(object, function);

			actionBinding.key = mapping->_key;

			addKeyBinding(actionBinding);
		}
		else
		{
			LOGF(Warning, CoreEngine, "Could not find action mapping: '%s'.", action.c_str());
		}
	}
}

#endif // !INPUT_COMPONENT_H_