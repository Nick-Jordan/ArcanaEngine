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


	ObjectRenderer& World::getRenderer()
	{
		return _renderer;
	}

	const GlobalObjectID& World::getId() const
	{
		return _id;
	}
}