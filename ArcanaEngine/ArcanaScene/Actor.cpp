#include "Actor.h"

#include "World.h"
#include "GeometryComponent.h"

namespace Arcana
{
	INITIALIZE_CATEGORY(Arcana, LogActor)

	Actor::Actor() : BaseObject(), _initialized(false), _sceneComponent(nullptr), _parent(nullptr)
	{

	}

	Actor::Actor(const std::string& name) : BaseObject()
	{
		initialize(name);
	}

	Actor::Actor(const Actor& actor) : BaseObject(actor)
	{
		initialize(actor.getName(), &actor);
	}

	Actor::~Actor()
	{
		//test
		for (auto iter = _components.createIterator(); iter; iter++)
		{
			AE_RELEASE(*iter);
		}
	}


	void Actor::initialize(std::string name, const Actor* templateActor)
	{
		setName(name);

		if (templateActor)
		{
			initializeTemplate(templateActor);
		}
		else
		{
			initializeDefault();
		}

		_initialized = true;
	}

	void Actor::initializeDefault()
	{
		_sceneComponent = nullptr;
		_actorTimeline = nullptr;
		_parent = nullptr;

		_world = nullptr;

		_lifetime = 0.0;
		_timeDilation = 1.0;

		_autoDestroy = true;
		_visible = true;
	}

	void Actor::initializeTemplate(const Actor* templateActor)
	{
		_sceneComponent = nullptr;
		_actorTimeline = nullptr;
		_parent = nullptr;

		_world = nullptr;

		_lifetime = templateActor->getLifetime();
		_timeDilation = templateActor->getTimeDilation();

		_autoDestroy = templateActor->_autoDestroy;
	}

	void Actor::update(double elapsedTime)
	{
		if (isActive())
		{
			const double actorElapsedTime = elapsedTime * _timeDilation;

			if (_actorTimeline)
			{
				_actorTimeline->update(actorElapsedTime);

				if (_lifetime != 0.0)
				{
					if (_actorTimeline->getCurrentPosition() >= _lifetime)
					{
						setActive(false);

						if (_autoDestroy)
						{
							destroy();
						}
					}
				}
			}
		}
	}

	void Actor::render(ObjectRenderer& renderer, Matrix4f view, Matrix4f projection)
	{
		if (isVisible())
		{
			for (auto iter = _components.createIterator(); iter; iter++)
			{
				GeometryComponent* renderComponent = dynamic_cast<GeometryComponent*>(*iter);

				if (renderComponent && renderComponent->hasRenderObject())
				{
					renderComponent->render(renderer, view, projection);
				}
			}
		}
	}


	Transform& Actor::getTransform()
	{
		Transform transform;
		if (_sceneComponent != nullptr)
		{
			transform = Transform();// _sceneComponent->getComponentTransform();
		}
		else
		{
			LOGF(Info, LogActor, "Actor \'%s\' has no root SceneComponent!", getName().c_str());
		}

		return transform;
	}

	Actor* Actor::getParent() const
	{
		return _parent;
	}

	void Actor::setParent(Actor* parent)
	{
		if (_parent != parent)
		{
			if (parent != nullptr && parent->isParentOf(this))
			{
				LOGF(Error, LogActor, "Failed to set '%s' parent of '%s' because it would cause circular ownership", parent->getName().c_str(), getName().c_str());
				return;
			}

			Actor* oldparent = _parent;
			if (_parent != nullptr)
			{
				_parent->_children.remove(this);
			}

			_parent = parent;

			if (_parent != nullptr)
			{
				if (!_parent->_children.contains(this))
				{
					_parent->_children.add(this);
				}
			}

			// mark all components for which Owner is relevant for visibility to be updated
			//MarkOwnerRelevantComponentsDirty(this);
		}
	}

	bool Actor::isParentOf(Actor* actor) const
	{
		for (const Actor* a = this; a; a = a->getParent())
		{
			if (a == actor)
			{
				return true;
			}
		}
		return false;
	}

	SceneComponent* Actor::getSceneComponent()
	{
		return _sceneComponent;
	}

	double Actor::getLifetime() const
	{
		return _lifetime;
	}

	void Actor::setLifetime(double life)
	{
		_lifetime = life;
	}

	double Actor::getTimeDilation() const
	{
		return _timeDilation;
	}

	void Actor::setTimeDilation(double timeDilation)
	{
		_timeDilation = timeDilation;
	}

	bool Actor::hasTag(std::string tag) const
	{
		return _tags.contains(tag);
	}

	void Actor::addTag(std::string tag)
	{
		_tags.add(tag);
	}

	void Actor::removeTag(std::string tag)
	{
		_tags.remove(tag);
	}

	World* Actor::getWorld() const
	{
		return _world;
	}

	bool Actor::isVisible() const
	{
		return _visible;
	}

	void Actor::setVisible(bool visible)
	{
		_visible = visible;
	}

	void Actor::addComponent(ActorComponent* component)
	{
		component->reference();
		_components.add(component);
	}
	
	void Actor::destroy()
	{
		if (_world)
		{
			_world->destroyActor(this);
		}
	}


	Actor& Actor::operator=(const Actor& actor)
	{
		BaseObject::operator=(actor);

		initialize(actor.getName(), &actor);

		return *this;
	}

}
