#include "ActorComponent.h"

#include "Actor.h"
#include "World.h"

#include "SceneLoggers.h"

namespace Arcana
{
	ActorComponent::ActorComponent()
	{
		initialize();
	}

	ActorComponent::ActorComponent(const ActorComponent& component)
	{
		//TODO
	}

	ActorComponent::~ActorComponent()
	{
		//TODO
	}


	void ActorComponent::initialize()
	{
		_userData = nullptr;

		_active = false;
		_concurrentUpdate = false;
		_autoActivate = true;
		_autoRegister = true;
		_needsRenderUpdate = false;
		_physicsObjectCreated = false;
		_registered = false;
		_renderObjectCreated = false;

		_owner = nullptr;
		_world = nullptr;
	}

	void ActorComponent::update(double elapsedTime)
	{

	}

	void ActorComponent::registered()
	{

	}

	void ActorComponent::unregistered()
	{
	}

	void ActorComponent::componentDestroyed()
	{

	}

	void ActorComponent::setUserData(void* userData)
	{
		_userData = userData;
	}

	void* ActorComponent::getUserData() const
	{
		return _userData;
	}

	void ActorComponent::setActive(bool active)
	{
		//TODO
		_active = active;
	}

	bool ActorComponent::isActive() const
	{
		return _active;
	}

	Actor* ActorComponent::getOwner() const
	{
		return _owner;
	}
	
	void ActorComponent::setOwner(Actor* owner)
	{
		if (_owner != owner)
		{
			//TODO
			_owner = owner;
		}
	}

	bool ActorComponent::hasTag(std::string tag) const
	{
		return _tags.contains(tag);
	}

	void ActorComponent::addTag(std::string tag)
	{
		_tags.add(tag);
	}

	void ActorComponent::removeTag(std::string tag)
	{
		_tags.remove(tag);
	}

	void ActorComponent::registerComponent()
	{
			Actor* owner = getOwner();
			if (owner)
			{
				_world = owner->getWorld();
			}

			if (_registered)
			{
				LOG(Warning, LogActor, "Actor component already registered");
				return;
			}

			if (!_world)
			{
				LOG(Error, LogActor, "Actor component world is nullptr");
				return;
			}

			_registered = true;

			//ExecuteRegisterEvents();
			//RegisterAllComponentTickFunctions(true);

			if (_autoActivate)
			{
				setActive(true);
			}

			registered();
		
	}

	void ActorComponent::unregisterComponent()
	{
		if (!_registered)
		{
			LOG(Warning, LogActor, "Actor component already unregistered");
			return;
		}
		_registered = false;

		unregistered();

		//ExecuteUnregisterEvents();
		//RegisterAllComponentTickFunctions(false);

		_world = nullptr;
	}

	World* ActorComponent::getWorld() const
	{
		World* world = _world;
		if (world == nullptr)
		{
			Actor* owner = getOwner();

			if (owner)
			{
				world = owner->getWorld();
			}
		}
		return world;
	}

	bool ActorComponent::isInWorld(const World* world) const
	{
		return getWorld() == world;
	}


	ActorComponent& ActorComponent::operator=(const ActorComponent& component)
	{
		//TODO

		return *this;
	}
}
