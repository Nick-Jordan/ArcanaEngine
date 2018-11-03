#ifndef ACTOR_COMPONENT_H_
#define ACTOR_COMPONENT_H_

#include "SceneDefines.h"

#include "Object.h"
#include "Array.h"
#include "Callback.h"

namespace Arcana
{
	class ARCANA_SCENE_API Actor;
	class ARCANA_SCENE_API World;

	class ARCANA_SCENE_API ActorComponent : public Object
	{
	public:

		ActorComponent();

		ActorComponent(const ActorComponent& component);

		~ActorComponent();


		void initialize();

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

		
		virtual void update(double elapsedTime);


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

		//ActivatedCallback _activatedCallback;
		//DeactivatedCallback _deactivatedCallback;
		//UpdateFunction _updateFunction;
	};

}

#endif // !ACTOR_COMPONENT_H_

