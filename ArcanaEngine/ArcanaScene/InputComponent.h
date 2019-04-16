#ifndef INPUT_COMPONENT_H_
#define INPUT_COMPONENT_H_

#include "SceneDefines.h"

#include "ActorComponent.h"
#include "InputBindings.h"
#include "Array.h"
#include "AxisMapping.h"

namespace Arcana
{
	class ARCANA_SCENE_API InputComponent : public ActorComponent
	{
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

		//template<class UserObject>
		//void bindActionMapping(const std::string& action, UserObject* object, void(UserObject::*function)());

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
}

#endif // !INPUT_COMPONENT_H_