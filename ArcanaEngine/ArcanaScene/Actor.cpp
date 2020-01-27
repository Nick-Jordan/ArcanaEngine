#include "Actor.h"

#include "World.h"
#include "GeometryComponent.h"
#include "BaseLightComponent.h"
#include "CameraComponent.h"

#include "SceneLoggers.h"

#include "ActorController.h"
#include "ResourceManager.h"
#include "ResourceCreator.h"

//#define BOX (1 << 0)
//#define SPHERE (1 << 1)

namespace Arcana
{

	Actor::Actor() : BaseObject(), _initialized(false), _sceneComponent(nullptr), _parent(nullptr)//, _dirtyBounds(BOX | SPHERE)
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
		/*for (auto iter = _components.createIterator(); iter; iter++)
		{
			AE_RELEASE(*iter);
		}

		//test
		AE_RELEASE(_sceneComponent);*/
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

		if (templateActor)
		{
			setMobility(templateActor->getMobility());
			setTimeScale(templateActor->getTimeScale());
		}

		//_dirtyBounds = (BOX | SPHERE);
	}

	void Actor::initializeDefault()
	{
		_sceneComponent = nullptr;
		_inputComponent = nullptr;
		_parent = nullptr;

		_world = nullptr;

		_lifetime = 0.0;

		_autoDestroy = true;
		_visible = true;

		_damageEnabled = true;
		_inputEnabled = true;
		_overrideBoundingBox = false;
	}

	void Actor::initializeTemplate(const Actor* templateActor)
	{
		_sceneComponent = nullptr;
		_parent = nullptr;

		_world = nullptr;

		_lifetime = templateActor->getLifetime();
		//_timec = templateActor->getTimeScale();

		_autoDestroy = templateActor->_autoDestroy;

		_damageEnabled = templateActor->isDamageEnabled();
		_inputEnabled = templateActor->isInputEnabled();
		_overrideBoundingBox = templateActor->_overrideBoundingBox;
	}

	void Actor::update(double elapsedTime)
	{
		const double actorElapsedTime = elapsedTime * getTimeScale();

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

	void Actor::render(ObjectRenderer& renderer)
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
						renderComponent->render(renderer);
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

	void Actor::setTransform(const Transform& transform)
	{
		if (_sceneComponent != nullptr)
		{
			_sceneComponent->setTransform(transform);
		}
		else
		{
			LOGF(Info, LogActor, "Actor \'%s\' has no root SceneComponent!", getName().c_str());
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

	double Actor::getTimeScale() const
	{
		if (_sceneComponent)
		{
			_sceneComponent->getTimeline().getTimeScale();
		}
		return 1.0;
	}

	void Actor::setTimeScale(double timeScale)
	{
		if (_sceneComponent)
		{
			_sceneComponent->getTimeline().setTimeScale(timeScale);
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
		if (!component)
			return;

		component->reference();
		_components.add(component);

		component->setOwner(this);
		if (component->_autoRegister)
		{
			component->registerComponent();
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

	void Actor::addChild(Actor* actor)
	{
		if (actor)
		{
			actor->setParent(this);
		}
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
		if (_sceneComponent)
		{
			_sceneComponent->setMobility(mobility);
		}
	}

	Mobility Actor::getMobility() const
	{
		if (_sceneComponent)
		{
			return _sceneComponent->getMobility();
		}

		return Mobility::Dynamic;
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

	const AxisAlignedBoundingBoxd& Actor::getBoundingBox()
	{
		if (!_overrideBoundingBox)
		{
			//if (_dirtyBounds & BOX)
			{
				_boundingBox.set(Vector3d::zero(), Vector3d::zero());
				for (auto i = _components.createConstIterator(); i; i++)
				{
					ActorComponent* comp = (*i);
					if (comp)
						_boundingBox.merge(comp->getBoundingBox());
				}
				//_dirtyBounds &= ~BOX;
			}
		}

		return _boundingBox;
	}

	const Sphered& Actor::getBoundingSphere()
	{
		if (!_overrideBoundingSphere)
		{
			//if (_dirtyBounds & SPHERE)
			{
				_boundingSphere.set(Vector3d::zero(), 0.0);
				for (auto i = _components.createConstIterator(); i; i++)
				{
					ActorComponent* comp = (*i);
					if (comp)
						_boundingSphere.merge((*i)->getBoundingSphere());
				}
				//_dirtyBounds &= ~SPHERE;
			}
		}

		return _boundingSphere;
	}

	void Actor::setBoundingBox(const AxisAlignedBoundingBoxd& box) // need way to clear overrideBoundingBox
	{
		if (_boundingBox != box)
		{
			_overrideBoundingBox = true;
			_boundingBox = box;
		}
	}

	void Actor::setBoundingSphere(const Sphered& sphere)
	{
		if (_boundingSphere != sphere)
		{
			_overrideBoundingBox = true;
			_boundingSphere = sphere;
		}
	}

	/*void Actor::dirtyBounds(bool box)
	{
		_dirtyBounds |= (1 << ((int32)!box));
	}*/

	void Actor::allowDestruction()
	{
		Object::allowDestruction();

		for (uint32 i = 0; i < _children.size(); i++)
		{
			Actor* child = getChild(i);
			if (child)
			{
				child->allowDestruction();
			}
		}

		for (auto i = _components.createIterator(); i; i++)
		{
			ActorComponent* component = *i;

			if (component)
			{
				component->allowDestruction();
			}
		}
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

	Mobility Actor::convertStringToMobility(const std::string& str)
	{
		std::string s = StringUtils::toLower(str);

		if (s == "static")
		{
			return Static;
		}
		else if (s == "stationary")
		{
			return Stationary;
		}

		return Dynamic;
	}

	class ActorResource : public ResourceCreator<Actor>
	{
	public:

		ActorResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler)
			: ResourceCreator<Actor>(id, type, data, dependencyScheduler)
		{
			initialize(id.getName());

			//actor parameters

			setMobility(convertStringToMobility(data.getStringParameter("mobility")));
			setActive(true);//test

			for (auto iter = data.getAdditionalData().begin(); iter != data.getAdditionalData().end(); iter++)
			{
				auto dataPoint = *iter;

				const ResourceData& dataPointResourceData = dataPoint.value;

				if (dataPoint.key == "child")
				{
					std::string actorName = "test_actorchild_" + id.getName();

					LoadResourceTask<Actor>* task = ResourceManager::instance().buildResource<Actor>(GlobalObjectID(actorName), dataPoint.key, dataPointResourceData, dependencyScheduler);

					if (task)
					{
						task->wait();
						childTasks.add(task);
					}
				}
				else if (dataPoint.key == "transform")
				{
					LoadResourceTask<Transform>* buildTask = ResourceManager::instance().buildResource<Transform>(GlobalObjectID(id.getName() + "::transform"), dataPoint.key, dataPointResourceData, dependencyScheduler);
					buildTask->wait();
					setTransform(*buildTask->get());
				}
				else
				{
					std::string actorCompName = "test_actorcomponent_" + id.getName();

					LoadResourceTask<ActorComponent>* task = ResourceManager::instance().buildResource<ActorComponent>(GlobalObjectID(actorCompName), dataPoint.key, dataPointResourceData, dependencyScheduler);

					if (task)
					{
						task->wait();
						componentTasks.add(task);
					}
				}
			}

			for (auto iter = data.getResourceDependencies().begin(); iter != data.getResourceDependencies().end(); iter++)
			{
				auto dataPoint = *iter;

				if (dataPoint.Type == "actor")
				{
					std::string actorName = "test_actorchild_" + id.getName();

					LoadResourceTask<Actor>* task = ResourceManager::instance().loadResource<Actor>(data.getResourceDependency(dataPoint.Name), dependencyScheduler);

					if (task)
					{
						task->wait();
						childTasks.add(task);
					}
				}
				else
				{
					std::string actorCompName = "test_actorcomponent_" + id.getName();

					LoadResourceTask<ActorComponent>* task = ResourceManager::instance().loadResource<ActorComponent>(data.getResourceDependency(dataPoint.Name), dependencyScheduler);

					if (task)
					{
						task->wait();
						componentTasks.add(task);
					}
				}
			}
		}

		void syncInitialize() override
		{
			for (auto i = componentTasks.createConstIterator(); i; i++)
			{
				auto task = *i;

				ActorComponent* comp = task->get();
				if (comp)
				{
					addComponent(comp);
				}
			}

			for (auto i = childTasks.createConstIterator(); i; i++)
			{
				auto task = *i;

				Actor* child = task->get();
				if (child)
				{
					addChild(child);
				}
			}
		}

	private:

		Array<LoadResourceTask<ActorComponent>*> componentTasks;
		Array<LoadResourceTask<Actor>*> childTasks;
	};

	Resource::Type<ActorResource, true> actorResource("actor");
}
