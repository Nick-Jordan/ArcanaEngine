#ifndef INPUT_BINDINGS_H_
#define INPUT_BINDINGS_H_

#include "SceneDefines.h"

#include "Key.h"
#include "KeyEvent.h"
#include "Vector2.h"

namespace Arcana
{
	REGISTER_CALLBACK(InputAxisCallback, float)

	REGISTER_CALLBACK(InputKeyCallback)

	REGISTER_CALLBACK(InputVectorAxisCallback, Vector2f)


	class ARCANA_SCENE_API InputAxisKeyBinding
	{
	public:

		InputAxisKeyBinding();

		InputAxisKeyBinding(Key key);

		InputAxisKeyBinding(const InputAxisKeyBinding& copy);

	public:

		InputAxisCallback axisCallback;

		Key axisKey;

		float axisValue;
	};

	class ARCANA_SCENE_API InputKeyBinding
	{
	public:

		InputKeyBinding();

		InputKeyBinding(Key key, KeyEvent::Type type = KeyEvent::Type::Pressed);

		InputKeyBinding(const InputKeyBinding& copy);

	public:

		InputKeyCallback keyCallback;

		Key key;

		KeyEvent::Type eventType;
	};

	class ARCANA_SCENE_API InputVectorAxisBinding
	{
	public:

		InputVectorAxisBinding();

		InputVectorAxisBinding(Key key);

		InputVectorAxisBinding(const InputVectorAxisBinding& copy);

	public:

		InputVectorAxisCallback vectorAxisCallback;

		Key vectorAxisKey;

		Vector2f vectorAxisValue;
	};
}

#endif // !INPUT_BINDINGS_H_