#ifndef WORLD_H_
#define WORLD_H_

#include "SceneDefines.h"

#include "Actor.h"
#include "GlobalObjectID.h"
#include "ObjectRenderer.h"

//test
#include "Mutex.h"

#include "WorldRenderer.h"

namespace Arcana
{
	class ARCANA_SCENE_API World : public WorldRenderer
	{
	public:

		class Listener : public Object
		{
		public:

			virtual ~Listener() {}

			virtual void worldUpdated(World* world, double elapsedTime) = 0;

			virtual void actorAdded(World* world, Actor* actor) = 0;

			virtual void actorDestroyed(World* world, Actor* actor) = 0;
		};

		World();
		World(const std::string& id);
		~World();

		template<typename ActorType = Actor>
		ActorType* createActor(const std::string& name, const Transform& transform, const Actor* templateActor = nullptr, bool collisionTest = false, Actor* parent = nullptr);

		bool destroyActor(Actor* actor);


		void addActor(Actor* actor);

		Actor* getActor(uint32 index) const;

		Actor* getActor(const std::string& name) const;

		uint32 getNumActors() const;

		void componentAdded(Actor* actor, ActorComponent* component);

		void updateActors(double elapsedTime);

		void addListener(Listener* listener);

		void removeListener(Listener* listener);

		virtual void renderActors() override;

		void start();


		ObjectRenderer& getRenderer();

		const GlobalObjectID& getId() const;

	protected:

		void setId(const GlobalObjectID& id);
		void initialize();

	private:

		ObjectRenderer _renderer;

		Array<Actor*> _actors;

		Array<Listener*> _listeners;

		Actor* _cameraActor;

		GlobalObjectID _id;

		bool _started;

		//test mutex for actors
		Mutex _actorMutex;
	};

	template<typename ActorType = Actor>
	inline ActorType* World::createActor(const std::string& name, const Transform& transform, const Actor* templateActor, bool collisionTest, Actor* parent)
	{
		if (!IsBaseOf<Actor, ActorType>::Value)
		{
			LOG(Error, CoreEngine, "Object of type \'ActorType\' cannot be added to a world");
			LOG(Error, CoreEngine, "ActorType is not a derived class of \'Actor\'");

			return nullptr;
		}

		ActorType* actor = new ActorType();
		actor->initialize(name, templateActor);
		actor->setActive(true);
		actor->setTransform(transform);
		actor->setParent(parent);

		addActor(actor);

		return actor;
	}
}

#endif // !WORLD_H_