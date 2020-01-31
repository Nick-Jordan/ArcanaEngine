#ifndef COLLISION_COMPONENT_H_
#define COLLISION_COMPONENT_H_

#include "SceneDefines.h"

#include "GeometryComponent.h"

namespace Arcana
{

	class ARCANA_SCENE_API CollisionComponent : public GeometryComponent
	{
	public:

		CollisionComponent(bool staticObject = false);

		virtual ~CollisionComponent();

		//Type getShapeType() const;

		bool isKinematic() const;

		bool isStatic() const;

		bool isDynamic() const;

	public:

		bool _kinematic;
		bool _static;
	};

}

#endif // !COLLISION_COMPONENT_H_

