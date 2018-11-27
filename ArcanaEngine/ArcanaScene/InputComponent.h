#ifndef INPUT_COMPONENT_H_
#define INPUT_COMPONENT_H_

#include "SceneDefines.h"

#include "ActorComponent.h"
#include "InputBindings.h"
#include "Array.h"

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


		void addAxisBinding(const InputAxisKeyBinding& axisBinding);

		void addKeyBinding(const InputKeyBinding& keyBinding);

		void addVectorAxisBinding(const InputVectorAxisBinding& vectorAxisBinding);

	private:

		Array<InputAxisKeyBinding> _inputAxisBindings;
		Array<InputKeyBinding> _inputKeyBindings;
		Array<InputVectorAxisBinding> _inputVectorAxisBindings;
	};

}

#endif // !INPUT_COMPONENT_H_