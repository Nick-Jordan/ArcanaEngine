#ifndef PHYSICS_MATERIAL_H_
#define PHYSICS_MATERIAL_H_

#include "PhysicsDefines.h"

#include "Object.h"
#include "PhysicsController.h"

namespace Arcana
{
	class ARCANA_PHYSICS_API PhysicsMaterial : public Object
	{
		friend class CollisionShape;

	public:

		PhysicsMaterial();

		PhysicsMaterial(double staticFriction, double dynamicFriction, double restitution);

		~PhysicsMaterial();

		void setDynamicFriction(double friction);

		double getDynamicFriction() const;

		void setStaticFriction(double friction);

		double getStaticFriction() const;

		void setRestitution(double restitution);

		double getRestitution() const;

	private:

		physx::PxMaterial* getMaterial() const;

		double _dynamicFriction;
		double _staticFriction;
		double _restitution;

		physx::PxMaterial* _material;
	};
}

#endif // !PHYSICS_MATERIAL_H_