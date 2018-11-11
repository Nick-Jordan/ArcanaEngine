#include "Actor.h"

#include "World.h"
#include "GeometryComponent.h"
#include "CameraComponent.h"

//test movement
#include "Controller.h"

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

		//test
		AE_DELETE(_sceneComponent);
		AE_DELETE(_actorTimeline);
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
		_actorTimeline = new TimelineComponent();
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
		const double actorElapsedTime = elapsedTime * _timeDilation;

		//test movement

		Array<CameraComponent*> cameraComponents;
		getComponents(cameraComponents);
		if (cameraComponents.size() > 0)
		{
			if (Controller::isConnected(0))
			{
				float xAxis = Controller::getFloatAxis(0, Keys::ControllerLeftAnalogX);
				float yAxis = Controller::getFloatAxis(0, Keys::ControllerLeftAnalogY);
				float up = Controller::getFloatAxis(0, Keys::ControllerLeftTriggerAxis) - Controller::getFloatAxis(0, Keys::ControllerRightTriggerAxis);

				if (abs(xAxis) > 0.05 || abs(yAxis) > 0.05 || abs(up) > 0.05)
				{
					xAxis = abs(xAxis) < 0.05 ? 0.0f : xAxis;
					yAxis = abs(yAxis) < 0.05 ? 0.0f : yAxis;
					up = abs(up) < 0.05 ? 0.0f : up;

					getSceneComponent()->translate(Vector3d(-xAxis, up, yAxis) * elapsedTime * 10.0);
				}

				float yawRotation = Controller::getFloatAxis(0, Keys::ControllerRightAnalogX) * elapsedTime * 100.0;
				float pitchRotation = Controller::getFloatAxis(0, Keys::ControllerRightAnalogY) * elapsedTime * 100.0;

				//if (abs(yawRotation) > 0.05 || abs(pitchRotation) > 0.05)
				{
					//yawRotation = abs(yawRotation) < 0.05 ? 0.0f : yawRotation;
					//pitchRotation = abs(pitchRotation) < 0.05 ? 0.0f : pitchRotation;

					Quaterniond quat;
					quat.fromPitchYawRoll(pitchRotation, yawRotation, 0.0);

					cameraComponents[0]->rotate(quat);
				}
			}
		}

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

	void Actor::setTransform(Transform* transform)
	{
		if (transform)
		{
			transform->reference();

			if (_sceneComponent != nullptr)
			{
				_sceneComponent->getRelativeTransform().set(*transform);
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

		if (_actorTimeline)
		{
			_actorTimeline->setLength(_lifetime);
		}
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
	}

	void Actor::getCameraMatrices(Matrix4f& view, Matrix4f& projection)
	{
		for (auto iter = _components.createIterator(); iter; iter++)
		{
			CameraComponent* cameraComponent = dynamic_cast<CameraComponent*>(*iter);
			if (cameraComponent && cameraComponent->isActive())
			{
				view = cameraComponent->getWorldTransform().getMatrix().cast<float>();
				projection = cameraComponent->getProjectionMatrix();
			}
		}
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
