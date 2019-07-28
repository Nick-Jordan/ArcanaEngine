#ifndef ACTOR_COMPONENT_H_
#define ACTOR_COMPONENT_H_

#include "SceneDefines.h"

#include "Object.h"
#include "Array.h"
#include "Callback.h"
#include "Timeline.h"

namespace Arcana
{
	REGISTER_CALLBACK(ActivatedCallback);
	REGISTER_CALLBACK(DeactivatedCallback);
	REGISTER_CALLBACK(ActorComponentUpdateFunction, double);

	class ARCANA_SCENE_API Actor;
	class ARCANA_SCENE_API World;

	class ARCANA_SCENE_API ActorComponent : public Object
	{
		friend class Actor;

	public:

		ActorComponent();

		ActorComponent(const ActorComponent& component);

		~ActorComponent();


		virtual void initialize();

		virtual void update(double elapsedTime);

		virtual void registered();

		virtual void unregistered();

		virtual void componentDestroyed();


		void setUserData(void* userData);

		void* getUserData() const;

		void setActive(bool active);

		bool isActive() const;
		
		Actor* getOwner() const;

		void setOwner(Actor* owner);

		bool hasTag(std::string tag) const;

		void addTag(std::string tag);

		void removeTag(std::string tag);

		void registerComponent();

		void unregisterComponent();

		World* getWorld() const;

		bool isInWorld(const World* world) const;

		Timeline& getTimeline();

		ActivatedCallback& activatedCallback();

		DeactivatedCallback& deactivatedCallback();

		ActorComponentUpdateFunction& updateFunction();


		ActorComponent& operator=(const ActorComponent& component);

	private:

		void* _userData;

		bool _active;
		bool _concurrentUpdate;
		bool _autoActivate;
		bool _autoRegister;
		bool _needsRenderUpdate;
		bool _physicsObjectCreated;
		bool _registered;
		bool _renderObjectCreated;

		Array<std::string> _tags;

		Actor* _owner;

		World* _world;

		Timeline _timeline;

		ActivatedCallback _activatedCallback;
		DeactivatedCallback _deactivatedCallback;
		ActorComponentUpdateFunction _updateFunction;
	};

}

#endif // !ACTOR_COMPONENT_H_

