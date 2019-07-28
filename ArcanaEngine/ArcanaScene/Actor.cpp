#include "Actor.h"

#include "World.h"
#include "GeometryComponent.h"
#include "BaseLightComponent.h"
#include "CameraComponent.h"

#include "SceneLoggers.h"

#include "ActorController.h"

namespace Arcana
{

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

		//test
		AE_RELEASE(_sceneComponent);
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

		_sceneComponent = new SceneComponent();
		_sceneComponent->reference();
		_inputComponent = nullptr;
	}

	void Actor::initializeDefault()
	{
		_sceneComponent = nullptr;
		_inputComponent = nullptr;
		_parent = nullptr;

		_world = nullptr;

		_lifetime = 0.0;
		_timeDilation = 1.0;

		_autoDestroy = true;
		_visible = true;

		_damageEnabled = true;
		_inputEnabled = true;

		_mobility = Dynamic;
	}

	void Actor::initializeTemplate(const Actor* templateActor)
	{
		_sceneComponent = nullptr;
		_parent = nullptr;

		_world = nullptr;

		_lifetime = templateActor->getLifetime();
		_timeDilation = templateActor->getTimeDilation();

		_autoDestroy = templateActor->_autoDestroy;
		_mobility = templateActor->_mobility;
	
		_damageEnabled = templateActor->isDamageEnabled();
		_inputEnabled = templateActor->isInputEnabled();
	}
	
	void Actor::update(double elapsedTime)
	{
		const double actorElapsedTime = elapsedTime * _timeDilation;

		for (auto i = _components.createIterator(); i; i++)
		{
			ActorComponent* component = *i;
			component->update(actorElapsedTime);
		}

		_updateFunction.executeIfBound(actorElapsedTime);
		
		if (_sceneComponent)
		{
			if (_lifetime != 0.0)
			{
				if (_sceneComponent->getTimeline().getPlaybackPosition() >= _lifetime)
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

	void Actor::render(ObjectRenderer& renderer, Matrix4d view, Matrix4d projection, Vector3d eyePosition)
	{
		if (isVisible())
		{
			for (auto iter = _components.createIterator(); iter; iter++)
			{
				GeometryComponent* renderComponent = dynamic_cast<GeometryComponent*>(*iter);

				if (renderComponent)
				{
					if (renderComponent->hasRenderProcedure())
					{
						renderComponent->render(renderer, view, projection, eyePosition);
						continue;
					}
				}

				BaseLightComponent* lightComponent = dynamic_cast<BaseLightComponent*>(*iter);

				if (lightComponent)
				{
					renderer.addLight(lightComponent->createRenderLight());
				}
			}
		}
	}

	void Actor::destroyed()
	{

	}

	void Actor::destroy()
	{
		if (_world)
		{
			_world->destroyActor(this);
		}
	}

	void Actor::begin()
	{

	}

	void Actor::end()
	{

	}

	Transform Actor::getTransform() const
	{
		if (_sceneComponent != nullptr)
		{
			return _sceneComponent->getRelativeTransform();
		}
		else
		{
			LOGF(Info, LogActor, "Actor \'%s\' has no root SceneComponent!", getName().c_str());
		}

		return Transform();
	}

	Transform& Actor::getLocalTransform()
	{
		if (_sceneComponent != nullptr)
		{
			return _sceneComponent->getLocalRelativeTransform();
		}
		else
		{
			LOGF(Info, LogActor, "Actor \'%s\' has no root SceneComponent!", getName().c_str());
		}

		return Transform();
	}

	void Actor::setTransform(Transform* transform)
	{
		if (transform)
		{
			transform->reference();

			if (_sceneComponent != nullptr)
			{
				_sceneComponent->setTransform(*transform);
			}
			else
			{
				LOGF(Info, LogActor, "Actor \'%s\' has no root SceneComponent!", getName().c_str());
			}

			transform->release();
		}
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

		if (_sceneComponent)
		{
			_sceneComponent->getTimeline().setTimelineLength(_lifetime);
		}
	}

	double Actor::getTimeDilation() const
	{
		return _timeDilation;
	}

	void Actor::setTimeDilation(double timeDilation)
	{
		_timeDilation = timeDilation;

		if (_sceneComponent)
		{
			_sceneComponent->getTimeline().setTimeScale(_timeDilation);
		}
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

	bool Actor::isInWorld(World* world) const
	{
		if (world)
		{
			return getWorld() == world;
		}

		return false;
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

		component->setOwner(this);
		if (component->_autoRegister)
		{
			component->registerComponent();
		}
		if (component->_autoActivate)
		{
			component->setActive(true);
		}

		if (_world)
		{
			_world->componentAdded(this, component);
		}

		InputComponent* input = dynamic_cast<InputComponent*>(component);
		if (input)
		{
			_inputComponent = input;
		}
	}

	void Actor::markComponentsForDestruction()
	{
		Array<ActorComponent*> components;
		getComponents(components);

		for (auto i = components.createIterator(); i; i++)
		{
			ActorComponent* component = *i;

			component->componentDestroyed();
			component->markForDestruction();
		}
	}

	Actor* Actor::getChild(uint32 index) const
	{
		if (index < getNumChildren())
		{
			return _children[index];
		}

		return nullptr;
	}

	uint32 Actor::getNumChildren() const
	{
		return _children.size();
	}

	const Array<Actor*>& Actor::getChildren() const
	{
		return _children;
	}

	void Actor::getCameraView(Matrix4d& view, Matrix4d& projection, Vector3d& position)
	{
		for (auto iter = _components.createIterator(); iter; iter++)
		{
			CameraComponent* cameraComponent = dynamic_cast<CameraComponent*>(*iter);
			if (cameraComponent && cameraComponent->isActive())
			{
				view = cameraComponent->getWorldTransform().getMatrix().inverse();
				projection = cameraComponent->getProjectionMatrix();
				position = cameraComponent->getWorldPosition();
			}
		}
	}

	void Actor::getViewPoint(Vector3d& position, Quaterniond& rotation)
	{
		if (getSceneComponent())
		{
			position = getSceneComponent()->getWorldPosition();
			rotation = getSceneComponent()->getWorldRotation();
		}
	}

	void Actor::setMobility(Mobility mobility)
	{
		_mobility = mobility;
	}

	Actor::Mobility Actor::getMobility() const
	{
		return _mobility;
	}

	bool Actor::isInputEnabled() const
	{
		return _inputEnabled;
	}

	void Actor::setInputEnabled(bool enabled)
	{
		_inputEnabled = enabled;
	}

	void Actor::toggleInputEnabled()
	{
		_inputEnabled = !_inputEnabled;
	}

	bool Actor::isDamageEnabled() const
	{
		return _damageEnabled;
	}

	void Actor::setDamageEnabled(bool enabled)
	{
		_damageEnabled = enabled;
	}

	void Actor::toggleDamageEnabled()
	{
		_damageEnabled = !_damageEnabled;
	}

	Timeline& Actor::getTimeline()
	{
		if (_sceneComponent)
		{
			return _sceneComponent->getTimeline();
		}

		return Timeline();
	}

	ActorUpdateFunction& Actor::updateFunction()
	{
		return _updateFunction;
	}

	ActorDestroyCallback& Actor::destroyCallback()
	{
		return _destroyCallback;
	}


	Actor& Actor::operator=(const Actor& actor)
	{
		BaseObject::operator=(actor);

		initialize(actor.getName(), &actor);

		return *this;
	}

}
