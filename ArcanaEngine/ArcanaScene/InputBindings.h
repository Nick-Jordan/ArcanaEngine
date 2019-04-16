#ifndef INPUT_BINDINGS_H_
#define INPUT_BINDINGS_H_

#include "SceneDefines.h"

#include "Key.h"
#include "KeyEvent.h"
#include "Vector2.h"

namespace Arcana
{
	REGISTER_CALLBACK(InputAxisCallback, float);

	REGISTER_CALLBACK(InputKeyCallback);

	REGISTER_CALLBACK(InputVectorAxisCallback, Vector2f);


	struct ARCANA_SCENE_API KeyMapping
	{
		Key key;
		float axisValue;

		KeyMapping() {};
		KeyMapping(Key key, float axisValue) : key(key), axisValue(axisValue) {};
	};

	class ARCANA_SCENE_API InputAxis
	{
		friend class InputComponent;

	public:

		void addKeyMapping(Key key, float axisValue = 1.0);

	private:

		std::vector<KeyMapping> _keyMappings;
	};

	class ARCANA_SCENE_API InputAxisBinding
	{
	public:

		InputAxisBinding();

		InputAxisBinding(InputAxis axis);

		InputAxisBinding(const InputAxisBinding& copy);

	public:

		InputAxisCallback axisCallback;

		InputAxis axis;

		float axisValue;
	};

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

		InputKeyBinding(Key key, bool repeat, KeyEvent::Type type = KeyEvent::Type::Pressed);

		InputKeyBinding(const InputKeyBinding& copy);

	public:

		InputKeyCallback keyCallback;

		Key key;

		bool repeat;

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