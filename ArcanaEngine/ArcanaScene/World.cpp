#include "World.h"

namespace Arcana
{
	World::World()
	{

	}
	World::World(const std::string& id) : _id(id)
	{

	}
	World::~World()
	{
	}

	void World::addActor(Actor* actor)
	{
		_actors.add(actor);
	}

	Actor* World::getActor(uint32 index) const
	{
		if (index < getNumActors())
		{
			return _actors[index];
		}

		return nullptr;
	}

	Actor* World::getActor(const std::string& name) const
	{
		return nullptr;
	}

	uint32 World::getNumActors() const
	{
		return _actors.size();
	}

	void World::updateActors(double elapsedTime)
	{
		for (auto i = _actors.createConstIterator(); i; i++)
		{
			Actor* actor = *i;

			if (actor->isActive())
			{
				actor->update(elapsedTime);
			}
		}
	}

	void World::renderActors()
	{
		LOG(Info, CoreEngine, "Render Actors Called");
		if (_renderer.numQueued != getNumActors())
		{
			LOG(Info, CoreEngine, "Queuing Actor meshes");
			for (auto i = _actors.createConstIterator(); i; i++)
			{
				Actor* actor = *i;

				actor->render(_renderer);
			}
		}

		_renderer.render();
	}


	ObjectRenderer& World::getRenderer()
	{
		return _renderer;
	}

	const GlobalObjectID& World::getId() const
	{
		return _id;
	}
}