#include "World.h"

namespace Arcana
{
	World::World() : _id("world"), _cameraActor(nullptr)
	{

	}
	World::World(const std::string& id) : _id(id), _cameraActor(nullptr)
	{

	}
	World::~World()
	{
		for (auto i = _actors.createIterator(); i; i++)
		{
			AE_RELEASE(*i);
		}

		_actors.empty();

		_cameraActor = nullptr;
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

	void World::setCameraActor(Actor* actor)
	{
		_cameraActor = actor;
		addActor(actor);
	}

	Actor* World::getCameraActor() const
	{
		return _cameraActor;
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

		LOG(Info, CoreEngine, "Queuing Actor meshes");

		bool hasCamera = _cameraActor != nullptr;

		Matrix4f projection;
		Matrix4f view;

		if (hasCamera)
		{
			projection = _cameraActor->getProjectionMatrix();
			view = _cameraActor->getViewMatrix();
		}

		for (auto i = _actors.createConstIterator(); i; i++)
		{
			Actor* actor = *i;
			
			if (!hasCamera)
			{
				projection = actor->getProjectionMatrix();
				view = actor->getViewMatrix();
			}
			actor->render(_renderer, view, projection);
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