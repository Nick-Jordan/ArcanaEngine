#ifndef INPUT_COMPONENT_H_
#define INPUT_COMPONENT_H_

#include "SceneDefines.h"

#include "ActorComponent.h"
#include "Key.h"
#include "EventListener.h"
#include "Vector2.h"

namespace Arcana
{
	class ARCANA_SCENE_API InputComponent : public ActorComponent, public EventListener
	{
	public:

		InputComponent();

		virtual ~InputComponent();


		virtual bool processEvent(Event& event, EventHandler& handler) override;

		virtual void initialize() override;

		virtual void update(double elapsedTime) override;

		virtual void registered() override;

		virtual void unregistered() override;

		virtual void componentDestroyed() override;
	};

}

#endif // !INPUT_COMPONENT_H_