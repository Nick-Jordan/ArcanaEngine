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

		void addActor(Actor* actor);

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
}

#endif // !WORLD_H_