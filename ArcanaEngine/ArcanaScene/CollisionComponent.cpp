#include "CollisionComponent.h"

#include "ResourceManager.h"
#include "ResourceCreator.h"

namespace Arcana
{

	CollisionComponent::CollisionComponent(bool staticObject) : _kinematic(false), _static(staticObject)
	{
	}


	CollisionComponent::~CollisionComponent()
	{
	}

	bool CollisionComponent::isKinematic() const
	{
		return _kinematic;
	}

	bool CollisionComponent::isStatic() const
	{
		return _static;
	}

	bool CollisionComponent::isDynamic() const
	{
		return !_static;
	}

	class CollisionComponentResource : public ResourceCreator<CollisionComponent>
	{
	public:

		CollisionComponentResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler)
			: ResourceCreator<CollisionComponent>(id, type, data, dependencyScheduler)
		{
			_static = data.getBoolParameter("static", false);
		}
	};

	Resource::Type<CollisionComponentResource> collisionComponentResource("collisionComponent");
}