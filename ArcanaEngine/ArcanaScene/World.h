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

		template<typename ActorType>
		ActorType* addActor();

		Actor* getActor(uint32 index) const;

		Actor* getActor(const std::string& name) const;

		uint32 getNumActors() const;

		void updateActors(double elapsedTime);

		virtual void renderActors() override;


		ObjectRenderer& getRenderer();

		const GlobalObjectID& getId() const;

	private:

		ObjectRenderer _renderer;

		Array<Actor*> _actors;

		GlobalObjectID _id;
	};

	template<typename ActorType>
	inline ActorType* World::addActor()
	{
		if (!IsBaseOf<Actor, ActorType>::Value)
		{
			LOG(Error, CoreEngine, "Object of type \'ActorType\' cannot be added to a world");
			LOG(Error, CoreEngine, "ActorType is not a derived class of \'Actor\'");

			return nullptr;
		}

		Actor* actor = new ActorType();
		_actors.add(actor);

		return actor;
	}
}

#endif // !WORLD_H_