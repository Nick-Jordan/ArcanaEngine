#ifndef ACTOR_H_
#define ACTOR_H_

#include "SceneDefines.h"

#include "BaseObject.h"
#include "Transform.h"
#include "Shape.h"

namespace Arcana
{
	class ARCANA_SCENE_API Actor : public BaseObject
	{
	public:

		Actor();

		Actor(const Actor& actor);

		virtual ~Actor();

	private:

		Shape* _shape;

		Transform _transform;
	};

}

#endif // !ACTOR_H_

