#include "PhysicsMaterial.h"

#include "ResourceManager.h"
#include "ResourceCreator.h"

namespace Arcana
{
	PhysicsMaterial::PhysicsMaterial()
		: _staticFriction(1.0), _dynamicFriction(1.0), _restitution(0.0)
	{
		_material = PhysicsController::instance().getPhysicsCore()->createMaterial(_staticFriction, _dynamicFriction, _restitution);
	}

	PhysicsMaterial::PhysicsMaterial(double staticFriction, double dynamicFriction, double restitution)
		: _staticFriction(staticFriction), _dynamicFriction(dynamicFriction), _restitution(restitution)
	{
		_material = PhysicsController::instance().getPhysicsCore()->createMaterial(_staticFriction, _dynamicFriction, _restitution);
	}

	PhysicsMaterial::~PhysicsMaterial()
	{
		AE_RELEASE(_material);
	}

	void PhysicsMaterial::setDynamicFriction(double friction)
	{
		_dynamicFriction = friction;

		_material->setDynamicFriction(friction);
	}

	double PhysicsMaterial::getDynamicFriction() const
	{
		return _dynamicFriction;
	}

	void PhysicsMaterial::setStaticFriction(double friction)
	{
		_staticFriction = friction;

		_material->setStaticFriction(friction);
	}

	double PhysicsMaterial::getStaticFriction() const
	{
		return _staticFriction;
	}

	void PhysicsMaterial::setRestitution(double restitution)
	{
		_restitution = restitution;

		_material->setRestitution(restitution);
	}

	double PhysicsMaterial::getRestitution() const
	{
		return _restitution;
	}

	physx::PxMaterial* PhysicsMaterial::getMaterial() const
	{
		return _material;
	}

	class PhysicsMaterialResource : public ResourceCreator<PhysicsMaterial>
	{
	public:

		PhysicsMaterialResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler)
			: ResourceCreator<PhysicsMaterial>(id, type, data, dependencyScheduler)
		{
			setStaticFriction(data.getDoubleParameter("staticFriction", 1.0));
			setDynamicFriction(data.getDoubleParameter("dynamicFriction", 1.0));
			setRestitution(data.getDoubleParameter("restitution", 0.0));
		}
	};

	Resource::Type<PhysicsMaterialResource> physicsMaterialResource("physicsMaterial");
}