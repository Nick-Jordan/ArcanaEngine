#ifndef WORLD_H_
#define WORLD_H_

#include "SceneDefines.h"

#include "Actor.h"
#include "GlobalObjectID.h"
#include "ObjectRenderer.h"

#include "WorldRenderer.h"

namespace Arcana
{
	class ARCANA_SCENE_API World : public WorldRenderer
	{
	public:

		World();
		World(const std::string& id);
		~World();

		template<typename ActorType = Actor>
		ActorType* createActor(const std::string& name, Transform* transform, const Actor* templateActor = nullptr, bool collisionTest = false, Actor* parent = nullptr);

		bool destroyActor(Actor* actor);




		void addActor(Actor* actor);

		Actor* getActor(uint32 index) const;

		Actor* getActor(const std::string& name) const;

		uint32 getNumActors() const;

		void componentAdded(Actor* actor, ActorComponent* component);

		void updateActors(double elapsedTime);

		virtual void renderActors() override;


		ObjectRenderer& getRenderer();

		const GlobalObjectID& getId() const;

	private:

		ObjectRenderer _renderer;

		Array<Actor*> _actors;

		Actor* _cameraActor;

		GlobalObjectID _id;
	};

	template<typename ActorType = Actor>
	inline ActorType* World::createActor(const std::string& name, Transform* transform, const Actor* templateActor, bool collisionTest, Actor* parent)
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