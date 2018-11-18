#ifndef COLLISION_COMPONENT_H_
#define COLLISION_COMPONENT_H_

#include "SceneDefines.h"

#include "GeometryComponent.h"

namespace Arcana
{

	class ARCANA_SCENE_API CollisionComponent : public GeometryComponent
	{
	public:

		CollisionComponent();

		virtual ~CollisionComponent();
	};

}

#endif // !COLLISION_COMPONENT_H_

