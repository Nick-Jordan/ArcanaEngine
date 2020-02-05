#include "CollisionComponent.h"

#include "ResourceManager.h"
#include "ResourceCreator.h"

namespace Arcana
{

	CollisionComponent::CollisionComponent() : _type(Static), _shape(nullptr)
	{
	}


	CollisionComponent::~CollisionComponent()
	{
	}

	void CollisionComponent::registered()
	{
		if (_shape)
		{
			_shape->updateShape(getOwner());
		}
	}

	bool CollisionComponent::isKinematic() const
	{
		return _type == Kinematic;
	}

	bool CollisionComponent::isStatic() const
	{
		return _type == Static;
	}

	bool CollisionComponent::isDynamic() const
	{
		return _type == Dynamic;
	}

	CollisionShape* CollisionComponent::getShape() const
	{
		return _shape;
	}

	void CollisionComponent::setCollisionType(Type type)
	{
		_type = type;
	}

	void CollisionComponent::setShape(CollisionShape* shape)
	{
		AE_RELEASE(_shape);

		_shape = shape;

		AE_REFERENCE(_shape);
	}

	CollisionComponent::Type CollisionComponent::convertStringToCollisionType(const std::string& type)
	{
		std::string t = StringUtils::toLower(type);

		if (t == "dynamic")
		{
			return Dynamic;
		}
		else if (t == "kinematic")
		{
			return Kinematic;
		}
		
		return Static;
	}

	class CollisionComponentResource : public ResourceCreator<CollisionComponent>
	{
	public:

		CollisionComponentResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler)
			: ResourceCreator<CollisionComponent>(id, type, data, dependencyScheduler)
		{
			setCollisionType(CollisionComponent::convertStringToCollisionType(data.getStringParameter("collisionType")));
			
			CollisionShape* shape = nullptr;

			LoadResourceTask<CollisionShape>* shapeTask = ResourceManager::instance().loadResource<CollisionShape>(data.getResourceDependency("collisionShape"), dependencyScheduler);
			if (shapeTask)
			{
				shapeTask->wait();
				shape = shapeTask->get();
			}
			else
			{
				const ResourceData* shapeData = data.getAdditionalData("collisionShape");

				if (shapeData)
				{
					shapeTask = ResourceManager::instance().buildResource<CollisionShape>(GlobalObjectID(id.getName() + "::collisionShape"), "collisionShape", *shapeData, dependencyScheduler);
					if (shapeTask)
					{
						shapeTask->wait();
						shape = shapeTask->get();
					}
				}
			}

			setShape(shape);
		}
	};

	Resource::Type<CollisionComponentResource> collisionComponentResource("collisionComponent");
}