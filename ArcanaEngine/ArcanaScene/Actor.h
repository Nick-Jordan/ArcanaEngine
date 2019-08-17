#ifndef ACTOR_H_
#define ACTOR_H_

#include "SceneDefines.h"

#include "BaseObject.h"
#include "Transform.h"
#include "Matrix4.h"
#include "SceneComponent.h"
#include "ObjectRenderer.h"
#include "InputComponent.h"

//test
#include "Material.h"
#include "RenderState.h"

namespace Arcana
{
	REGISTER_CALLBACK(ActorUpdateFunction, double);
	REGISTER_CALLBACK(ActorDestroyCallback);

	class ARCANA_SCENE_API ActorController;

	class ARCANA_SCENE_API Actor : public BaseObject
	{
		friend class World;

	public:

		enum Mobility
		{
			Static,
			Stationary,
			Dynamic
		};

		Actor();

		Actor(const std::string& name);

		Actor(const Actor& actor);

		virtual ~Actor();


		virtual void initialize(std::string name, const Actor* templateActor = nullptr);

		virtual void update(double elapsedTime);

		virtual void render(ObjectRenderer& renderer, Matrix4d view, Matrix4d projection, Vector3d eyePosition);

		virtual void destroyed();

		virtual void destroy() override;

		virtual void begin();

		virtual void end();


		Transform getTransform() const;

		Transform& getLocalTransform();

		void setTransform(const Transform& transform);

		Actor* getParent() const;

		void setParent(Actor* parent);

		bool isParentOf(Actor* actor) const;

		SceneComponent* getSceneComponent();

		double getLifetime() const;

		void setLifetime(double life);

		double getTimeScale() const;

		void setTimeScale(double scale);

		bool hasTag(std::string tag) const;

		void addTag(std::string tag);

		void removeTag(std::string tag);

		World* getWorld() const;

		bool isInWorld(World* world) const;

		bool isVisible() const;

		void setVisible(bool visible);

		void addComponent(ActorComponent* component);

		void markComponentsForDestruction();

		uint32 getNumChildren() const;

		Actor* getChild(uint32 index) const;

		const Array<Actor*>& getChildren() const;

		template<typename ComponentType>
		bool getComponents(Array<ComponentType*>& components);

		template<typename ComponentType>
		ComponentType* getComponent() const;

		template<typename ComponentType>
		bool hasComponent() const;

		template<typename ComponentType>
		bool hasActiveComponent() const;

		void getCameraView(Matrix4d& view, Matrix4d& projection, Vector3d& position);

		void getViewPoint(Vector3d& position, Quaterniond& rotation);

		void setMobility(Mobility mobility);

		Mobility getMobility() const;

		bool isInputEnabled() const;

		void setInputEnabled(bool enabled);

		void toggleInputEnabled();

		bool isDamageEnabled() const;

		void setDamageEnabled(bool enabled);

		void toggleDamageEnabled();

		Timeline& getTimeline();

		const AxisAlignedBoundingBoxd& getBoundingBox();

		const Sphered& getBoundingSphere();

		void setBoundingBox(const AxisAlignedBoundingBoxd& box);

		void setBoundingSphere(const Sphered& sphere);

		//void dirtyBounds(bool box);

		virtual void allowDestruction() override;

		ActorUpdateFunction& updateFunction();

		ActorDestroyCallback& destroyCallback();

		//get damage instigator
		//get transform local to world
		//get bounding box
		//get velocity
		//set position/translate
		//get distance to other actor
		//get horizontal/vertical distance to other actor
		//get dot product to other actor
		//get set collision enabled
		//has been initialized
		//is being destroyed
		//begin overlap with actor
		//end overlap with actorFst
		//begin cursor overlap
		//end cursor overlap
		//on click
		//on release
		//get overlapping actors
		//get overlapping GeometryComponents
		//set/get label
		//lifespan expired function
		//destroyed callback
		//is overlapping actor
		//damage actor

		Actor& operator=(const Actor& actor);

	private:

		void initializeDefault();

		void initializeTemplate(const Actor* templateActor);

	protected:

		SceneComponent* _sceneComponent;
		InputComponent* _inputComponent;

	private:

		Actor* _parent;
		World* _world;
		Array<Actor*> _children;
		Array<ActorComponent*> _components;

		double _lifetime;

		bool _initialized;
		bool _autoDestroy;
		bool _visible;
		Array<std::string> _tags;
		Mobility _mobility;

		bool _inputEnabled;

		ActorUpdateFunction _updateFunction;
		bool _collisionsEnabled;
		bool _damageEnabled;
		double _creationTime;
		bool _overrideBoundingBox;
		bool _overrideBoundingSphere;
		//int8 _dirtyBounds;
		AxisAlignedBoundingBoxd _boundingBox;
		Sphered _boundingSphere;
		//damage instigator
		//num uncached lightsE

		//TakeDamageCallback _takeDamageCallback;
		//TakePointDamageCallback _takePointDamageCallback;
		//ActorOverlapCallback _overlapCallback;
		//ActorEndOverlapCallback _endOverlapCallback;
		//CursorOverlapCallback _cursorOverlapCallback;
		//CursorEndOverlapCallback _cursorEndOverlapCallback;
		//OnClickCallback _onClickCallback;
		//OnReleaseCallback _onReleaseCallback;

		//CollisionCallback _collisionCallback;
		ActorDestroyCallback _destroyCallback;
	};

	template<typename ComponentType = ActorComponent>
	bool Actor::getComponents(Array<ComponentType*>& components)
	{
		if (!IsBaseOf<ActorComponent, ComponentType>::Value)
		{
			LOG(Error, CoreEngine, "ComponentType is not a derived class of \'ActorComponent\'");

			return false;
		}

		bool returnValue = false;

		for (auto iter = _components.createConstIterator(); iter; iter++)
		{
			ComponentType* component = dynamic_cast<ComponentType*>(*iter);

			if (component)
			{
				components.add(component);
				returnValue = true;
			}
		}

		return returnValue;
	}

	template<typename ComponentType>
	ComponentType* Actor::getComponent() const
	{
		if (!IsBaseOf<ActorComponent, ComponentType>::Value)
		{
			LOG(Error, CoreEngine, "ComponentType is not a derived class of \'ActorComponent\'");

			return false;
		}

		for (auto iter = _components.createConstIterator(); iter; iter++)
		{
			ComponentType* component = dynamic_cast<ComponentType*>(*iter);

			if (component)
			{
				return component;
			}
		}

		return nullptr;
	}

	template<typename ComponentType>
	bool Actor::hasComponent() const
	{
		if (!IsBaseOf<ActorComponent, ComponentType>::Value)
		{
			LOG(Error, CoreEngine, "ComponentType is not a derived class of \'ActorComponent\'");

			return false;
		}

		for (auto iter = _components.createConstIterator(); iter; iter++)
		{
			ComponentType* component = dynamic_cast<ComponentType*>(*iter);

			if (component)
			{
				return true;
			}
		}

		return false;
	}

	template<typename ComponentType>
	bool Actor::hasActiveComponent() const
	{
		if (!IsBaseOf<ActorComponent, ComponentType>::Value)
		{
			LOG(Error, CoreEngine, "ComponentType is not a derived class of \'ActorComponent\'");

			return false;
		}

		for (auto iter = _components.createConstIterator(); iter; iter++)
		{
			ComponentType* component = dynamic_cast<ComponentType*>(*iter);

			if (component && component->isActive())
			{
				return true;
			}
		}

		return false;
	}
}

#endif // !ACTOR_H_

