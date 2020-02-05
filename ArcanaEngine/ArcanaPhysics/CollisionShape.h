#ifndef COLLISION_SHAPE_H_
#define COLLISION_SHAPE_H_

#include "PhysicsDefines.h"

#include "Object.h"
#include "Vector3.h"
#include "PhysicsController.h"
#include "PhysicsMaterial.h"

namespace Arcana
{
	class Actor;

	class ARCANA_PHYSICS_API CollisionShape : public Object
	{
		friend class PhysicsWorld;
		friend class CollisionComponent;

	public:

		enum Type
		{
			None = -1,
			Sphere,
			Plane,
			Capsule,
			Box,
			ConvexMesh,
			TriangleMesh,
			HeightField,
		};

		class Properties
		{
			friend class CollisionShape;

		public:

			Properties();
			Properties(const Properties& properties);
			~Properties();

			Properties& operator=(const Properties& properties);

			bool isEmpty() const;

		protected:

			CollisionShape::Type _collisionType;

			struct BoxData 
			{ 
				double extents[3];
			};
			struct SphereData
			{
				double radius; 
			};
			struct CapsuleData
			{ 
				double radius;
				double height;
			};

			union
			{
				BoxData box;
				SphereData sphere;
				CapsuleData capsule;
				//heightfield
				//mesh
			} _shapeData;

			bool _isExplicit;
		};

		static Properties box();

		static Properties box(const Vector3d& extents);

		static Properties sphere();

		static Properties sphere(double radius);

		static Properties capsule();

		static Properties capsule(double radius, double height);

		//heightfield and mesh

		CollisionShape();

		CollisionShape(const Properties& properties, const Vector3d& scale, bool dynamic, PhysicsMaterial* material, const Vector3d& centerOfMassOffset = Vector3d::zero());

		CollisionShape(const CollisionShape& copy);

		~CollisionShape();

		Type getType() const;

		static Type convertStringToShapeType(const std::string& shape);

	protected:

		void initialize(const Properties& properties, const Vector3d& scale, bool dynamic, PhysicsMaterial* material, const Vector3d& centerOfMassOffset = Vector3d::zero());

	private:

		void updateShape(Actor* actor);

		physx::PxShape* getShape() const;

	private:

		physx::PxShape* _shape;
		PhysicsMaterial* _material;

		Properties _properties;
	};
}

#endif // !COLLISION_SHAPE_H_

