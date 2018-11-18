#ifndef ACTOR_H_
#define ACTOR_H_

#include "SceneDefines.h"

#include "BaseObject.h"
#include "Transform.h"
#include "Matrix4.h"
#include "TimelineComponent.h"
#include "SceneComponent.h"
#include "ObjectRenderer.h"

//test
#include "Material.h"
#include "RenderState.h"

namespace Arcana
{
	REGISTER_CATEGORY(LogActor, none)

	class ARCANA_SCENE_API Actor : public BaseObject
	{
		friend class World;

	public:

		Actor();

		Actor(const std::string& name);

		Actor(const Actor& actor);

		virtual ~Actor();


		virtual void initialize(std::string name, const Actor* templateActor = nullptr);

		virtual void update(double elapsedTime);

		virtual void render(ObjectRenderer& renderer, Matrix4f view, Matrix4f projection);

		virtual void destroyed();


		Transform& getTransform();

		void setTransform(Transform* transform);

		Actor* getParent() const;

		void setParent(Actor* parent);

		bool isParentOf(Actor* actor) const;

		SceneComponent* getSceneComponent();

		double getLifetime() const;

		void setLifetime(double life);

		double getTimeDilation() const;

		void setTimeDilation(double timeDilation);

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
		bool hasComponent() const;

		template<typename ComponentType>
		bool hasActiveComponent() const;

		void getCameraMatrices(Matrix4f& view, Matrix4f& projection);


		virtual void destroy() override;

		//update
		//enable/disable input
		//get damage instigator
		//get transform
		//get transform local to world
		//get bounding box
		//get velocity
		//set position/translate
		//get distance to other actor
		//get horizontal/vertical distance to other actor
		//get dot product to other actor
		//get set visible
		//get set collision enabled
		//destroy
		//add remove component
		//clear component
		//get component array
		//add/has tag
		//get time dilation
		//has been initialized
		//initialize
		//is being destroyed
		//begin overlap with actor
		//end overlap with actor
		//begin cursor overlap
		//end cursor overlap
		//on click
		//on release
		//get overlapping actors
		//get overlapping GeometryComponents
		//set/get lifespan
		//is owned by actor
		//get owner
		//set/get label
		//lifespan expired function
		//destroyed callback
		//is overlapping actor
		//is in world
		//get world
		//damage actor
		//intialize

		Actor& operator=(const Actor& actor);

	private:

		void initializeDefault();

		void initializeTemplate(const Actor* templateActor);

	private:

		SceneComponent* _sceneComponent;
		TimelineComponent* _actorTimeline;
		Actor* _parent;
		World* _world;
		Array<Actor*> _children;
		Array<ActorComponent*> _components;

		double _lifetime;
		double _timeDilation;

		bool _initialized;
		bool _autoDestroy;
		bool _visible;
		Array<std::string> _tags;

		/*ActorUpdateFunction _updateFunction;
		bool _collisionsEnabled;
		// input stuff
		bool _canBeDamaged;
		bool _isBeingDestroyed;
		double _creationTime;
		//damage instigator
		//num uncached lightsE

		TakeDamageCallback _takeDamageCallback;
		TakePointDamageCallback _takePointDamageCallback;
		ActorOverlapCallback _overlapCallback;
		ActorEndOverlapCallback _endOverlapCallback;
		CursorOverlapCallback _cursorOverlapCallback;
		CursorEndOverlapCallback _cursorEndOverlapCallback;
		OnClickCallback _onClickCallback;
		OnReleaseCallback _onReleaseCallback;

		CollisionCallback _collisionCallback;
		DestroyCallback _destroyCallback;

		Array<ActorComponent*> _components;*/
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

