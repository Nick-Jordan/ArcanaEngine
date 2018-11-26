#ifndef INPUT_COMPONENT_H_
#define INPUT_COMPONENT_H_

#include "SceneDefines.h"

#include "ActorComponent.h"
#include "Key.h"
#include "EventListener.h"
#include "Vector2.h"

namespace Arcana
{
	REGISTER_CALLBACK(MouseMovementCallback, int32, int32, int32, int32);

	class ARCANA_SCENE_API InputComponent : public ActorComponent, public EventListener
	{
	public:

		InputComponent();

		virtual ~InputComponent();

		bool isKeyPressed(Key key);

		int32 getMouseX() const;

		int32 getMouseY() const;

		bool isControllerConnected(uint32 controllerId);

		float getControllerFloatAxis(uint32 controllerId, Key key);

		Vector2f getControllerVectorAxis(uint32 controllerId, Key axis);

		MouseMovementCallback& getMouseMovementCallback();


		virtual bool processEvent(Event& event, EventHandler& handler) override;

		virtual void initialize() override;

		virtual void update(double elapsedTime) override;

		virtual void registered() override;

		virtual void unregistered() override;

		virtual void componentDestroyed() override;

	private:

		bool _pressedKeys[KeyCode::NumKeys];

		int32 _mouseX;
		int32 _mouseY;
		int32 _lastMouseX;
		int32 _lastMouseY;

		MouseMovementCallback _mouseMovementCallback;
	};

}

#endif // !INPUT_COMPONENT_H_