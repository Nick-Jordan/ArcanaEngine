#include "Actor.h"

#include "World.h"
#include "GeometryComponent.h"
#include "CameraComponent.h"

//test movement
#include "Controller.h"
#include "Input.h"

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
		_inputComponent = nullptr;
	}

	void Actor::initializeDefault()
	{
		_sceneComponent = nullptr;
		_actorTimeline = nullptr;
		_inputComponent = nullptr;
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

	//test

	void Actor::moveForward(float input)
	{
		movement.z = input;
	}
	void Actor::moveRight(float input)
	{
		movement.x = input;
	}
	void Actor::moveUp(float input)
	{
		movement.y = input;
	}

	void Actor::pitch(float input)
	{
		rotation.x = input;
	}
	void Actor::yaw(float input)
	{
		rotation.y = input;
	}
	void Actor::roll(float input)
	{
		rotation.z = input;
	}
	void Actor::mousePitch(float input)
	{
		float delta = 720.0f / 2.0f - input;
		rotation.x = delta;
	}
	void Actor::mouseYaw(float input)
	{
		float delta = input - 1280.0f / 2.0f;
		rotation.y = delta;
	}

	//test

	void Actor::update(double elapsedTime)
	{
		const double actorElapsedTime = elapsedTime * _timeDilation;

		for (auto i = _components.createIterator(); i; i++)
		{
			ActorComponent* component = *i;
			component->update(actorElapsedTime);
		}

		Input::setMousePosition(Vector2i(1280, 720) / 2);

		//test movement

		Array<CameraComponent*> cameraComponents;
		getComponents(cameraComponents);
		if (cameraComponents.size() > 0)
		{
			//getSceneComponent()->getRelativeTransform().setTranslationZ(12.0);
			//CAMERA CONTROLLING ------------------------------------
			/*float xAxis = 0.0f;
			float yAxis = 0.0f;
			float upAxis = 0.0f;
			float yRotation = 0.0f;
			float xRotation = 0.0f;
			float zRotation = 0.0f;
			if (Controller::isConnected(0))
			{
				xAxis = Controller::getFloatAxis(0, Keys::ControllerLeftAnalogX);
				//yAxis = Controller::getFloatAxis(0, Keys::ControllerLeftAnalogY);
				upAxis = Controller::getFloatAxis(0, Keys::ControllerRightTriggerAxis) - Controller::getFloatAxis(0, Keys::ControllerLeftTriggerAxis);
				yRotation = Controller::getFloatAxis(0, Keys::ControllerRightAnalogX) * elapsedTime * 100.0;
				xRotation = Controller::getFloatAxis(0, Keys::ControllerRightAnalogY) * elapsedTime * 100.0;
				zRotation = (float)(Controller::isButtonPressed(0, Keys::ControllerLeftShoulder) - Controller::isButtonPressed(0, Keys::ControllerRightShoulder)) * elapsedTime * 60.0f;
			}
			else
			{
				xAxis = (float)(Input::isKeyPressed(Keys::D) - Input::isKeyPressed(Keys::A));
				yAxis = (float)(Input::isKeyPressed(Keys::W) - Input::isKeyPressed(Keys::S));
				upAxis = (float)(Input::isKeyPressed(Keys::Space) - Input::isKeyPressed(Keys::LeftControl));
				//yRotation = (float)(Input::isKeyPressed(Keys::L) - Input::isKeyPressed(Keys::J)) * elapsedTime * 100.0;
				//xRotation = (float)(Input::isKeyPressed(Keys::I) - Input::isKeyPressed(Keys::K)) * elapsedTime * 100.0;
				zRotation = (float)(Input::isKeyPressed(Keys::Q) - Input::isKeyPressed(Keys::E)) * elapsedTime * 60.0f;
			
				Vector2i position = Input::getMousePosition();
				Vector2i rel = position - Vector2i(1920, 1080) / 2;

				Input::setMousePosition(Vector2i(1920, 1080) / 2);


				yRotation = (float)rel.x;
				xRotation = (float)-rel.y;
			}*/

			float xAxis = movement.x;
			float yAxis = movement.z;
			float upAxis = movement.y;
			float yRotation = rotation.y;
			float xRotation = rotation.x;
			float zRotation = rotation.z;
			rotation.x = rotation.y = 0.0f;

			if (abs(xAxis) > 0.05 || abs(yAxis) > 0.05 || abs(upAxis) > 0.05)
			{
				xAxis = abs(xAxis) < 0.05 ? 0.0f : xAxis;
				yAxis = abs(yAxis) < 0.05 ? 0.0f : yAxis;
				upAxis = abs(upAxis) < 0.05 ? 0.0f : upAxis;

				Vector3d finalVector = cameraComponents[0]->getForwardVector() * -yAxis
					+ cameraComponents[0]->getUpVector() * upAxis
					+ cameraComponents[0]->getRightVector() * xAxis;

				finalVector.normalize();

				double speed = 10.0;

				getSceneComponent()->translate(finalVector * elapsedTime * speed);
			}

			if (abs(xRotation) > 0.02 || abs(yRotation) > 0.02 || abs(zRotation) > 0.02)
			{
				xRotation = abs(xRotation) < 0.02 ? 0.0f : xRotation;
				yRotation = abs(yRotation) < 0.02 ? 0.0f : yRotation;
				zRotation = abs(zRotation) < 0.02 ? 0.0f : zRotation;

				double speed = 100.0;

				Quaterniond quatY;
				quatY.fromAxisAngle(Vector3d::unitY(), -yRotation * elapsedTime * speed);
				cameraComponents[0]->rotate(quatY);

				Quaterniond quatX;
				quatX.fromAxisAngle(Vector3d::unitX(), xRotation * elapsedTime * speed);
				cameraComponents[0]->rotate(quatX);

				Quaterniond quatZ;
				quatZ.fromAxisAngle(Vector3d::unitZ(), zRotation * elapsedTime * speed);
				cameraComponents[0]->rotate(quatZ);
			}

			//CAMERA CONTROLLING ------------------------------------
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

	void Actor::render(ObjectRenderer& renderer, Matrix4f view, Matrix4f projection, Vector3d eyePosition)
	{
		if (isVisible())
		{
			for (auto iter = _components.createIterator(); iter; iter++)
			{
				GeometryComponent* renderComponent = dynamic_cast<GeometryComponent*>(*iter);

				if (renderComponent && renderComponent->hasRenderProcedure())
				{
					renderComponent->render(renderer, view, projection, eyePosition);
				}
			}
		}
	}

	void Actor::destroyed()
	{

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

	void Actor::getCameraView(Matrix4f& view, Matrix4f& projection, Vector3d& position)
	{
		for (auto iter = _components.createIterator(); iter; iter++)
		{
			CameraComponent* cameraComponent = dynamic_cast<CameraComponent*>(*iter);
			if (cameraComponent && cameraComponent->isActive())
			{
				view = cameraComponent->getWorldTransform().getMatrix().cast<float>().inverse();
				projection = cameraComponent->getProjectionMatrix();
				position = cameraComponent->getWorldPosition();
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
