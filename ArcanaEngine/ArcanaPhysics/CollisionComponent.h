#ifndef COLLISION_COMPONENT_H_
#define COLLISION_COMPONENT_H_

#include "PhysicsDefines.h"

#include "GeometryComponent.h"
#include "CollisionShape.h"

namespace Arcana
{

	//Move to ArcanaPhysics
	//add CollisionObject/CollisionShape

	class ARCANA_PHYSICS_API CollisionComponent : public GeometryComponent
	{
	public:

		enum Type
		{
			Dynamic,
			Static,
			Kinematic
		};

		CollisionComponent();

		virtual ~CollisionComponent();

		virtual void registered() override;

		//Type getShapeType() const;

		bool isKinematic() const;

		bool isStatic() const;

		bool isDynamic() const;

		CollisionShape* getShape() const;

		void setCollisionType(Type type);

		void setShape(CollisionShape* shape);

	protected:

		Type convertStringToCollisionType(const std::string& type);

	private:

		Type _type;
		CollisionShape* _shape;
	};

}

#endif // !COLLISION_COMPONENT_H_

