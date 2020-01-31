#ifndef COLLISION_OBJECT_H_
#define COLLISION_OBJECT_H_

#include "PhysicsDefines.h"

#include "Transform.h"

namespace Arcana
{
	/*class ARCANA_PHYSICS_API CollisionObject
	{
	public:

		enum Type
		{
			None,
			RigidBody,
			Character,
			GhostObject,
			Vehicle,
			VehicleWheel,
		};

		class CollisionPair
		{
		public:


			CollisionPair(CollisionObject* objectA, CollisionObject* objectB);

			bool operator < (const CollisionPair& collisionPair) const;

			CollisionObject* objectA;

			CollisionObject* objectB;
		};

		class CollisionListener
		{
		public:

			enum EventType
			{
				Colliding,
				NotColliding
			};

			virtual ~CollisionListener() { }

			virtual void collisionEvent(CollisionObject::CollisionListener::EventType type,
				const CollisionObject::CollisionPair& collisionPair,
				const Vector3d& contactPointA = Vector3d::zero(),
				const Vector3d& contactPointB = Vector3d::zero()) = 0;
		};

		virtual ~CollisionObject();

		virtual CollisionObject::Type getType() const = 0;

		CollisionShape::Type getShapeType() const;

		CollisionComponent* getCollisionCommponent() const;

		CollisionShape* getCollisionShape() const;

		bool isKinematic() const;

		bool isStatic() const;

		bool isDynamic() const;

		bool isEnabled() const;

		void setEnabled(bool enable);

		void addCollisionListener(CollisionListener* listener, CollisionObject* object = nullptr);

		void removeCollisionListener(CollisionListener* listener, CollisionObject* object = nullptr);

		bool collidesWith(CollisionObject* object) const;

		const Transform& getTransform() const;

		void setTransform(const Transform& transform);*/
}

#endif // !COLLISION_OBJECT_H_